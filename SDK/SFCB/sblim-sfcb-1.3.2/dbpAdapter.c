/*
 * dbpAdapter.c
 *
 * (C) Copyright IBM Corp. 2005
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:       Sebastian Seyrich <seyrich@de.ibm.com>
 *
 * Description:
 * 
 *
 * 
 *
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <ctype.h>
#include <time.h>

#include <sys/wait.h>
#include <sys/socket.h>
#include <netdb.h>

#include "cmpidt.h"
#include "msgqueue.h"
#include "utilft.h"
#include "trace.h"
#include "cimXmlRequest.h"
//#include "cimXmlParser.h"
#include "queryOperation.h"

#include <pthread.h>
#include <semaphore.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "httpComm.h"
#include "sfcVersion.h"
#include "sqlStatement.h"
#include "control.h"


//unsigned long exFlags = 0;
static char *name;

int sfcbSSLMode = 0;
static int doFork = 0;
static int doBa;
int noChunking = 0;
static int dbpProcSem;
static int dbpWorkSem;
static int running=0;
static int debug;
extern char * configfile;
static int stopAccepting=0;

static int hBase;
static int hMax;
static int dbpProcId;

#define PROTOCOL 1	
#define SQL 2
#define META 3
#define SPOOLIN 4
#define SPOOLOUT 5

//Meta Parameter
#define METADB 1
#define TABLES 2
#define STABLES 3
#define KEYS 4
#define COLS 5

//Protokoll Parameter
#define LOGOUT 2
#define CONNECT 1

#define HEADER 2
#define CONTINUE 0

#define test 1
//#define printd(x) if(test) printf("%s",x)
#define printd(...) if(test) printf(__VA_ARGS__)

static const char *metaDB = "version=0.1;name=sfcb;maxcon=1000;mathfunc=keine,nochnich;sqlkeywords=selbe,wie,in,db2;sqlstate=nochnichtstandartisiert;stringfunc=comma,list;systemfunc=noch,nichts;datetimefunc=noch,nichts$$\n";

static key_t dbpProcSemKey;
static key_t dbpWorkSemKey;
static int dbpProcSem;
static int dbpWorkSem;


extern int setupControl(char *);
extern int getControlNum(char *id, long *val);
extern int getControlBool(char *id, int *val);
extern int semGetValue(int semid, int semnum);

ResultSet * rs;

void initDbpProcCtl(int p)
{
   dbpProcSemKey=ftok(SFCB_BINARY,'D');
   dbpWorkSemKey=ftok(SFCB_BINARY,'E');
   char emsg[256];
   union semun sun;
   int i;

   printf("--- Max dbp procs: %d\n",p);
   if ((dbpProcSem=semget(dbpProcSemKey,1,0600))!=-1) 
      semctl(dbpProcSem,0,IPC_RMID,sun);
      
   if ((dbpProcSem=semget(dbpProcSemKey,1+p,IPC_CREAT | 0600))==-1) {
      sprintf(emsg,"Dbp Proc semaphore create %d",getpid());
      perror(emsg);
      abort();
   }
   sun.val=p;
   semctl(dbpProcSem,0,SETVAL,sun);
   
   sun.val=0;
   for (i=1; i<=p; i++)
      semctl(dbpProcSem,p,SETVAL,sun);

   if ((dbpWorkSem=semget(dbpWorkSemKey,1,0600))!=-1) 
      semctl(dbpWorkSem,0,IPC_RMID,sun);
      
   if ((dbpWorkSem=semget(dbpWorkSemKey,1,IPC_CREAT | 0600))==-1) {
      sprintf(emsg,"Dpb ProcWork semaphore create %d",getpid());
      perror(emsg);
      abort();
   }
   sun.val=1;
   semctl(dbpWorkSem,0,SETVAL,sun);
}


char* processSQLQuery(char *query,CommHndl conn_fd){
	int stat = 0;
	char *res = NULL;
	ResultSet *rs;

//printf("1\n");
	rs = createRS(query,&stat);	
	if(!stat){
		printd("Parsen erfolgreich\n");
		res = (char *) malloc(strlen(rs->meta)+strlen(rs->tupel)+strlen(rs->sw->reason)+strlen(rs->sw->sqlstate)+4+1+3+3+3+1);
		strcpy(res,"2 1\n");
		strcat(res,rs->sw->sqlstate);
		strcat(res,";");
		strcat(res,rs->sw->reason);
		strcat(res,"$$\n");
		strcat(res,rs->meta);
		strcat(res,"$$\n");
		strcat(res,rs->tupel);
		strcat(res,"$$\n");
		
	}
	else{
		printd("Parse Error %d!!\n",stat);
		printd("###%s \n###%s!!\n",rs->sw->reason,rs->sw->sqlstate);
		if(strstr(rs->sw->reason,"unexpected $end")!=0){
		  char * s = "SQL STATEMENT NOT YET COMPLETE:\n";
		  char * tmp =  rs->sw->reason;
		  rs->sw->reason = (char*)malloc(strlen(tmp)+strlen(s)+1);
		  strcpy(rs->sw->reason,s);
		  strcat(rs->sw->reason,tmp);
		  rs->sw->sqlstate = "03000";
		}
		res = (char *) malloc(strlen(rs->sw->reason)+strlen(rs->sw->sqlstate)+4+1+3+1);printd("Parse Error 2!!\n");
		strcpy(res,"2 0\n");
		strcat(res,rs->sw->sqlstate);
		strcat(res,";");
		strcat(res,rs->sw->reason);
		strcat(res,"$$\n");
		//printd("Parse Error 2!!\n");
	}
	//printf("return...\n");
	return res;
}





//CommHndl conn_fd;
/*
 * S E M A P H O R E
 */
static void handleDbpSession(int connFd) {
	
	CommHndl conn_fd;
   	struct sembuf procReleaseUnDo = {0,1,SEM_UNDO};

   	int r,b2,c, by=0, h;
 
   	char buffer[1024],bc[1];
   	char buffer2[500];
   	char *response=NULL; 	//Fehlermeldung reinschreiben und an Client zurückschicken.
   					//Vermutlich später nicht mehr notwendig!!
   	char *header=NULL;char *payload;
   	int nbytes;
      
   
   _SFCB_ENTER(TRACE_DBPDAEMON, "handledbpRequest");
   _SFCB_TRACE(1, ("--- Forking sql handler"));
   		
   // printf("dbpProcSem %p dbpProcId: %d hMax: %d doFork: %d\n",dbpProcSem,dbpProcId,hMax,doFork);
   // printf("dbpProcId: %d hMax: %d doFork: %d\n",dbpProcId,hMax,doFork);
    if (doFork) {
      	semAcquire(dbpWorkSem,0);
      	semAcquire(dbpProcSem,0);
      	for (dbpProcId=0; dbpProcId<hMax; dbpProcId++)
        	if (semGetValue(dbpProcSem,dbpProcId+1)==0) break;
        printf("dbpProcId: %d hMax: %d\n",dbpProcId,hMax);
      	procReleaseUnDo.sem_num=dbpProcId+1; 
         
      	r = fork();

      	if (r==0) {
         	currentProc=getpid();
         	processName="CIMSQL-Processor";
         	semRelease(dbpProcSem,0);
         	semAcquireUnDo(dbpProcSem,0);
         	semReleaseUnDo(dbpProcSem,dbpProcId+1);
         	semRelease(dbpWorkSem,0);

         	if (sfcbSSLMode) {
#if defined USE_SSL
            	conn_fd.socket=-2;
            	conn_fd.bio=BIO_new(BIO_s_socket());
            	BIO_set_fd(conn_fd.bio,connFd,BIO_CLOSE);
            	if (!(conn_fd.ssl = SSL_new(ctx)))
               		intSSLerror("Error creating SSL context");
            	SSL_set_bio(conn_fd.ssl, conn_fd.bio, conn_fd.bio);
            	if (SSL_accept(conn_fd.ssl) <= 0)
               		intSSLerror("Error accepting SSL connection");
#endif
         	}
         	else {
            	conn_fd.socket=connFd;
#if defined USE_SSL
            	conn_fd.bio=NULL;
            	conn_fd.ssl=NULL;
#endif
         	}
      	}
      	else if (r>0) {
         	running++;
         	_SFCB_EXIT();
      	}
   	}
   	else r = 0;

  	if (r < 0) {
      	char *emsg=strerror(errno);
      	mlogf(M_ERROR,M_SHOW,"--- fork handler: %s\n",emsg);
      	exit(1);
   	}

   	if (r == 0) {
      	if (doFork) {
         	_SFCB_TRACE(1,("--- Forked sql handler %d", currentProc))
         	resultSockets=sPairs[hBase+dbpProcId];
      	}

      	_SFCB_TRACE(1,("--- Started sql handler %d %d", currentProc,
                   resultSockets.receive));

      	if (getenv("SFCB_PAUSE_HTTP")) for (;;) {
        	fprintf(stderr,"-#- Pausing - pid: %d\n",currentProc);
         	sleep(5);
      	}   
       
      	conn_fd.socket=connFd;
#if defined USE_SSL
      	conn_fd.bio=NULL;
      	conn_fd.ssl=NULL;
#endif
    	for(;;){ 
     	//c onn_fd.socket=connFd;

     	// doHttpRequest(conn_fd);
			nbytes = read (connFd, buffer, HEADER);
			header = (char *) malloc(nbytes);//sowas wie \n Steuerzeichen
			strncpy(header,buffer,nbytes);
   			header[nbytes]=0;
			h = atoi(header);
				printd("Ein Client h: %d header: %s nbytes: %d\n",h,header,nbytes);
			//Sonderfall, noch nicht eingeloggt
			if(by&h){
				//login
				nbytes = read(connFd, buffer, 2);
				c = atoi((char*)&buffer);
				if(c==CONNECT){ 
					printd("Ein Client hat sich korrekt angemeldet\n");
					response = "1 1 1\n";//"Sie sind angemeldet. Warte auf Anfragen:\n";
					write(connFd, response , strlen(response));
					by=0;
					h=CONTINUE;//dummy, um switch sofort zu verlassen
				}
				else{
					printd("Ein Client hat sich NICHT korrekt angemeldet %d\n",c);
					response = "1 1 0\n";//"Sie sind nicht angemeldet. Auf wiedersehen.\n";
					write(connFd, response , strlen(response));
					break;
				} 
				while((nbytes = read(connFd, bc, 1))>0)//
	  				if(bc[0]=='\n')
	  					break;
			}
			if(by){
				printd("Ein Client hat sich NICHT korrekt angemeldet %d\n",h);
				response = "1 1 0\n";//"Sie sind nicht angemeldet. Auf wiedersehen.\n";
				write(connFd, response , strlen(response));
				break;	
			}
			switch(h){
				case CONTINUE: break;
				case PROTOCOL: {
					nbytes = read(connFd, buffer, 2);
					c = atoi((char*)&buffer);
					
					//Pipe leersaugen
					while((nbytes = read(connFd, bc, 1))>0)//
	      				if(bc[0]=='\n')
	      					break;
						
						
						if(c==CONNECT){ 
						   	response = "1 1 0\n"; //"Sie sind bereits angemeldet, Operation wird ignoriert:\n";
							write(connFd, response , strlen(response));
						}
						else if(c==LOGOUT){
							printd("Der Client hat die Verbindung beendet\n");
					   		response = "1 2 1\n";//"By\n";
				    		write(connFd, response ,strlen(response));
				    		by=1; 
						}
						else{
							b2 = sprintf(buffer2,"%d %d %d",1,c,0);//sprintf(buffer2, "Syntxfehler: Operation %d ist keine Protokolloperation\n",c);
							printd("%s",buffer2);
	   						write(connFd, buffer2 , b2);
						}
					break;	
				}
				case SQL:
					while((nbytes = read(connFd, buffer, 1024))>0){
						if(buffer[nbytes-1]=='\n')
							break;
						//falls Anweisung laenerg als 1024, muss buffer mit vorgaengerbuffer konkadiniert werden. vgl. adrian		
					}
					// abschließendes $ finden:
					
					//printd("SQL: %d\n",nbytes);
					nbytes--;
					while((nbytes>0) && (buffer[nbytes]!='$'))
						nbytes--;
					//printf("malloc %d bytes",nbytes);
					payload = (char *) malloc(nbytes+1+1);
					
					//ein \n voranstellen, yyerror()s wegen
					//*payload = '\n';
					strcpy(payload,"\n");
					strncat(payload,buffer,nbytes);
					
					response = processSQLQuery(payload,conn_fd); //--> Datenstruktur, in die das Statement reinkommt
					
					free(payload);payload=NULL;	
					//ResultMetaData
					
					
					write(connFd, response , strlen(response)); 
					free(response);response=NULL;
					
					break;
				case META:  
					nbytes = read(connFd, buffer, 2);
					c = atoi((char*)&buffer);
					if(c==METADB){
						response = (char *) malloc(strlen(metaDB)+7);
						response = strcpy(response,"3 1 1\n");
						response = strcat(response,metaDB);
						write(connFd, response , strlen(response));
						free(response);
						break;
					}
					
					if(c==TABLES||c==STABLES||c==KEYS||c==COLS){
					  //printf("UND los\n");
						
						while((nbytes = read(connFd, buffer, 1024))>0){
							if(buffer[nbytes-1]=='\n')
								break;
							//falls Anweisung laenerg als 1024, muss buffer mit vorgaengerbuffer konkadiniert werden. vgl. adrian		
						}
						// abschließendes $ finden:
					
						printd("SQL: %d\n",nbytes);
						nbytes--;
						while((nbytes>0) && (buffer[nbytes]!='$'))
							nbytes--;
						//printf("malloc %d bytes",nbytes);
						
						buffer[nbytes] = 0;
						if(nbytes==0)
							payload = NULL;
						else{
							payload = (char *) malloc(nbytes+1);
							strcpy(payload,buffer);
							//printd(">%s< %d\n",payload,c);
						}
						if(c==TABLES)
							response = processMetaTables(payload,"root/cimv2");
						else if(c==STABLES)
							response = processSuperTables(payload,"root/cimv2");
						else if(c==KEYS)
							response = processKeyTable(payload,"root/cimv2");
						else if(c==COLS){
							char *arg2 = strstr(payload,"::");
							
							char *arg1 = strtok(payload,"::");
							//printf("lllllll\n");
							//printf(">%s< >%s< >%s< >%s<\n",payload,arg2,arg1,arg2+2);
							response = processMetaColumns(arg1,arg2+2,"root/cimv2");
							//printf("zurück\n");
						}
						//printf(">>%s<<",response);
						free(payload);payload=NULL;
						write(connFd, response , strlen(response));
						//free(response);
						//dieses free tut nicht, weil irgendwo anders was nicht stimmt!!!
						break;	
					}
					break;
				case SPOOLIN: break;
				case SPOOLOUT: break;
				default: 
					printd("Unbekannter Befehl: \"%s\"\n",header);
					b2 = sprintf(buffer2,"Fehler: unbekannter Befehl:  %s \n", header);
					//strncpy(response,buffer2,b2);
				
	    			//response = "Fehler: Unbekannter Befehl\n";
	    			write(connFd, buffer2 , b2);
	    			break;
				
				
			}
	     	free(header);
	      	if(by)
	      		break;
      	}
      
      
     	if (!doFork) return;

      	_SFCB_TRACE(1, ("--- SQL handler exiting %d", currentProc));
      	printf("--- SQL handler exiting %d\n", currentProc);
      	dumpTiming(currentProc);
      	exit(0);
   }
        
    
}

/*kopiert!!*/
static void handleSigChld(int sig)
{
   const int oerrno = errno;
   pid_t pid;
   int status;

   for (;;) {
      pid = wait4(0, &status, WNOHANG, NULL);
      if ((int) pid == 0)
         break;
      if ((int) pid < 0) {
         if (errno == EINTR || errno == EAGAIN) {
    //        fprintf(stderr, "pid: %d continue \n", pid);
            continue;
         }
         if (errno != ECHILD)
            perror("child wait");
         break;
      }
      else {
         running--;
          fprintf(stderr, "%s: SIGCHLD signal %d - %s(%d)\n", name, pid,
                 __FILE__, __LINE__);
      }
   }
   errno = oerrno;
}
static void handleSigUsr1(int sig)
{
   stopAccepting=1;
          fprintf(stderr, "%s: handleSigUsr1 signal %d - %s(%d)\n", name,sig,
                 __FILE__, __LINE__);
}




/*
 * Von httpDaemon aus httpAdapter abgeschrieben.
 *
 */
int dbpDaemon(int argc, char *argv[], int sslMode, int sfcbPid) {//int argc, char *argv[], int sslMode) {
	struct sockaddr_in sin;
   int sz,sin_len,ru;
   char *cp;
   long procs, port;
   int listenFd, connFd;

   name = argv[0];
   debug = 1;
   doFork = 1;

   _SFCB_ENTER(TRACE_DBPDAEMON, "dbpDaemon");

   setupControl(configfile);
   sfcbSSLMode=sslMode;
   if (sslMode) processName="DBPS-Daemon";
   else processName="DBP-Daemon";

   if (sslMode) {
      if (getControlNum("dbpsPort", &port))
         port = 5981;
      hBase=stBase;
      hMax=stMax;
   }
   else {
      if (getControlNum("dbpPort", &port))
         port = 5980;
      hBase=htBase;
      hMax=htMax;
   }

   if (getControlNum("dbpProcs", &procs))
      procs = 10;
   initDbpProcCtl(procs);

//   if (getControlBool("doBasicAuth", &doBa))
//      doBa=0;
//
//   i = 1;
//   while (i < argc && argv[i][0] == '-') {
//      if (strcmp(argv[i], "-D") == 0)
//         debug = 1;
//      else if (strcmp(argv[i], "-nD") == 0)
//         debug = 0;
//      else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
//         ++i;
//         port = (unsigned short) atoi(argv[i]);
//      }
//      else if (strcmp(argv[i], "-tm") == 0) {
//         if (isdigit(*argv[i + 1])) {
//            ++i;
//         }
//      }
//      else if (strcmp(argv[i], "-F") == 0)
//         doFork = 1;
//      else if (strcmp(argv[i], "-nF") == 0)
//         doFork = 0;
//      else if (strcmp(argv[i], "-H") == 0);
//      ++i;
//   }
//
//   if (getControlBool("useChunking", &noChunking))
//      noChunking=0;
//   noChunking=noChunking==0;

   cp = strrchr(name, '/');
   if (cp != NULL)
      ++cp;
   else  cp = name;
   name = cp;

   if (sslMode) mlogf(M_INFO,M_SHOW,"--- %s DBPS Daemon V" sfcdbpDaemonVersion " started - %d - port %ld\n", 
         name, currentProc,port);
   else mlogf(M_INFO,M_SHOW,"--- %s DBP  Daemon V" sfcdbpDaemonVersion " started - %d - port %ld\n", 
         name, currentProc,port);


   if (doBa) mlogf(M_INFO,M_SHOW,"--- Using Basic Authentication\n");

   listenFd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
   sin_len = sizeof(sin);

   ru = 1;
   setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, (char *) &ru, sizeof(ru));

   memset(&sin,0,sin_len);

   sin.sin_family = AF_INET;
   sin.sin_addr.s_addr = INADDR_ANY;
   sin.sin_port = htons(port);

   if (bind(listenFd, (struct sockaddr *) &sin, sin_len) ||
       listen(listenFd, 0)) {
      mlogf(M_ERROR,M_SHOW,"--- Cannot listen on port %ld\n", port);
      kill(sfcbPid,3);
   }

  if (!debug) {
      int rc = fork();
      if (rc == -1) {
         char *emsg=strerror(errno);
         mlogf(M_ERROR,M_SHOW,"--- fork daemon: %s",emsg);
         exit(1);
      }
      else if (rc)
         exit(0);
   }
//   memInit();
    currentProc=getpid();
    setSignal(SIGCHLD, handleSigChld,0);
    setSignal(SIGUSR1, handleSigUsr1,0);
    setSignal(SIGINT, SIG_IGN,0);
    setSignal(SIGTERM, SIG_IGN,0);
    setSignal(SIGHUP, SIG_IGN,0);

 //   commInit();

#if defined USE_SSL
    if (sfcbSSLMode) {
       char *fnc,*fnk;
       ctx = SSL_CTX_new(SSLv23_method());
       getControlChars("sslCertificateFilePath", &fnc);
       if (SSL_CTX_use_certificate_chain_file(ctx, fnc) != 1)
           intSSLerror("Error loading certificate from file");
       getControlChars("sslKeyFilePath", &fnk);
       if (SSL_CTX_use_PrivateKey_file(ctx, fnk, SSL_FILETYPE_PEM) != 1)
           intSSLerror("Error loading private key from file");
    }
#endif

   for (;;) {
   char *emsg;
      listen(listenFd, 1);
      sz = sizeof(sin);
      if ((connFd = accept(listenFd, (__SOCKADDR_ARG) & sin, &sz))<0) {
         if (errno == EINTR || errno == EAGAIN) {
            if (stopAccepting) break;
            continue;
         }   
         emsg=strerror(errno);
         mlogf(M_ERROR,M_SHOW,"--- accept error %s\n",emsg);
         _SFCB_ABORT();
      }
      _SFCB_TRACE(1, ("--- Processing dbp request"));

      handleDbpSession(connFd); 
      close(connFd);
   }
   
   printf("--- %s draining %d\n",processName,running);
   for (;;) {
      if (running==0) {
         mlogf(M_INFO,M_SHOW,"--- %s terminating %d\n",processName,getpid());
         exit(0);
      }   
      sleep(1);
   }   
	



/*   struct sockaddr_in sin;
   int sz,i,sin_len,ru;
   char *cp;//??
   long procs, port;
   int listenFd, connFd;


   name = "sfcBroker";
   //debug = 1;
   //doFork = 1;

   _SFCB_ENTER(TRACE_DBPDAEMON, "dbpDaemon");

   //setupControl(NULL);
   processName="DBP Daemon"; //Variable aus sfcBroker, dieser neue Prozess hat noch keinen zugewiesen bekommen

	//if (getControlNum("httpPort", &port))
        port = 5980;
    //hBase=htBase;
    //hMax=htMax;
   
//initndpProcCtl(procs);
   

   cp = strrchr(name, '/');
   if (cp != NULL)
      ++cp;
   else  cp = name;
   name = cp;

   
   printf("--- %s  DBP  Daemon V" sfcdbpDaemonVersion " started - %d - port %ld\n", name, getpid(),port);



   listenFd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
   sin_len = sizeof(sin);

   ru = 1;
   setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, (char *) &ru, sizeof(ru));

   memset(&sin,0,sin_len);

   sin.sin_family = AF_INET;
   sin.sin_addr.s_addr = INADDR_ANY;
   sin.sin_port = htons(port);

   if (bind(listenFd, (struct sockaddr *) &sin, sin_len) ||
       listen(listenFd, 0)) {//Test, ob Port frei ist
      printf("cannot listen on port %ld", port);
      exit(3);
   }

  //if (!debug) {
      int rc = fork();
      if (rc == -1) {
         perror("fork daemon");
         exit(1);
      }
      else if (rc)
         exit(0);
   //}
//   memInit();
//printf("jetzt kann sich jemand verbinden\n");
 //   setSignal(SIGCHLD, handleSigChld,0);
	
    for (;;) {
     	
        listen(listenFd, 1);
        sz = sizeof(sin);
        if ((connFd = accept(listenFd, (__SOCKADDR_ARG) & sin, &sz))<0) {
            if (errno == EINTR || errno == EAGAIN)
            	continue;
        	perror("accept error");
        //  _SFCB_ABORT();
        }
         _SFCB_TRACE(1, ("--- Processing  dbp request"));

	   	printd("es hat sich glaub ich jemand verbunden\n");
	   
	    handleDbpSession(connFd); 
       	close(connFd);
    }
*/
}


