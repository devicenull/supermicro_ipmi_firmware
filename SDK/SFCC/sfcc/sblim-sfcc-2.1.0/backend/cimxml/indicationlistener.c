
/*
 * $\Id$
 *
 * © Copyright IBM Corp. 2007
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:        Sven Schuetz <sven@de.ibm.com>
 *
 * Description:   indication listener implementation
 *                Most of the network and http code is taken from sfcb
 *                and written by Adrian Schuur / Viktor Mihajlovski
 */


#include "nativeCimXml.h"
#include "utilft.h"
#include "cimXmlParser.h"

#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>

static int do_listen=1;

#define SOCKBUFSZ 32768
#define hdrBufsize 5000
#define hdrLimmit 5000

typedef struct _buffer {
   char *data, *content;
   int length, size, ptr, content_length,trailers;
   char *httpHdr, *authorization, *content_type, *host, *useragent;
   char *principal;
   char *protocol;
} Buffer;

typedef struct commHndl {
  int   socket;
  FILE *file;
  void *buf;
} CommHndl;


static void freeBuffer(Buffer * b)
{
   Buffer emptyBuf = { NULL, NULL, 0, 0, 0, 0, 0 ,0};
   if (b->data)
      free(b->data);
   if (b->content)
      free(b->content);
   *b=emptyBuf;   
}


int commWrite(CommHndl to, void *data, size_t count)
{
   int rc=0;
  
    if (to.file == NULL) { 
        rc = write(to.socket,data,count);
    } else {
        rc = fwrite(data,count,1,to.file);
        if (rc == 1) {
        /* return number of bytes written */
        rc = count;
        }
    }
    
    return rc;
}

int commRead(CommHndl from, void *data, size_t count)
{
    int rc=0;
    
    rc = read(from.socket,data,count);
    
    return rc;
}

void commClose(CommHndl hndl)
{
    if (hndl.file == NULL) {
        close(hndl.socket);
    } else {
        fclose(hndl.file);
        if (hndl.buf) {
            free(hndl.buf);
        }
    }
}

void commFlush(CommHndl hndl)
{
    if (hndl.file) {
        fflush(hndl.file);
    }
}

static void add2buffer(Buffer * b, char *str, size_t len)
{
   if (b->size == 0) {
      b->size = len + 500;
      b->length = 0;
      b->data = (char *) malloc(b->size);
   }
   else if (b->length + len >= b->size) {
      b->size = b->length + len + 500;
      b->data = (char *) realloc((void *) b->data, b->size);
   }
   memmove(&((b->data)[b->length]), str, len);
   b->length += len;
   (b->data)[b->length] = 0;
}

static void genError(CommHndl conn_fd, Buffer * b, int status, char *title,
                     char *more)
{
    char head[1000];
    char server[] = "Server: sfcc indListener\r\n";
    char clength[] = "Content-Length: 0\r\n";
    char cclose[] = "Connection: close\r\n";
    char end[] = "\r\n";

    snprintf(head, sizeof(head), "%s %d %s\r\n", b->protocol, status, title);
    commWrite(conn_fd, head, strlen(head));
    if (more) {
        commWrite(conn_fd, more, strlen(more));
    }
    commWrite(conn_fd, server, strlen(server));
    commWrite(conn_fd, clength, strlen(clength));
    commWrite(conn_fd, cclose, strlen(cclose));

    commWrite(conn_fd, end, strlen(end));
    commFlush(conn_fd);
}

static char *getNextHdr(Buffer * b)
{
    int i;
    char c;

    for (i = b->ptr; b->ptr < b->length; ++b->ptr) {
        c = b->data[b->ptr];
        if (c == '\n' || c == '\r') {
            b->data[b->ptr] = 0;
            ++b->ptr;
            if (c == '\r' && b->ptr < b->length && b->data[b->ptr] == '\n') {
                b->data[b->ptr] = 0;
                ++b->ptr;
            }
            return &(b->data[i]);
        }
    }
    return NULL;
}

static int readData(CommHndl conn_fd, char *into, int length)
{
    int c = 0, r;

    while (c < length) {
        r = commRead(conn_fd, into + c, length - c);
        if (r < 0 && (errno == EINTR || errno == EAGAIN)) {
            continue;
        }
        c += r;
    }
    return c;
}

static void getPayload(CommHndl conn_fd, Buffer * b)
{
    int c = b->length - b->ptr;
    b->content = (char *) malloc(b->content_length + 8);
    if (c) memcpy(b->content, (b->data) + b->ptr, c);

    readData(conn_fd, (b->content) + c, b->content_length - c);
    *((b->content) + b->content_length) = 0;
}

static int  getHdrs(CommHndl conn_fd, Buffer * b, char *cmd)
{
   int first=1,total=0,isReady;
   struct timeval httpTimeout;
   fd_set httpfds;
   int state=0;
   
   FD_ZERO(&httpfds);
   FD_SET(conn_fd.socket,&httpfds);
   httpTimeout.tv_sec=5;
   httpTimeout.tv_usec=0;
   isReady = select(conn_fd.socket+1,&httpfds,NULL,NULL,&httpTimeout);
   if (isReady == 0) return 3;
    
   for (;;) {
      char buf[hdrBufsize];
      int r = commRead(conn_fd, buf, sizeof(buf));
      
      if (r < 0 && (errno == EINTR || errno == EAGAIN)) continue;
      if (r <= 0) break;
      
      add2buffer(b, buf, r);
      total+=r;
      if (r && first) {
         if (strncasecmp(buf,cmd,strlen(cmd)) != 0) { 
       /* not what we expected - still continue to read to
          not confuse the client */
       state = 1;
     }
         first=0;
      }
      
      if (strstr(b->data, "\r\n\r\n") != NULL ||
          strstr(b->data, "\n\n") != NULL) {
         break;
      }
      
      if (total>=hdrLimmit) {
        fprintf(stderr, "-#- Possible DOS attempt detected\n");
        state = 2;
        break;
      }
   }
   return state;
}

static void processIndication(struct native_indicationlistener *i, char *xml)
{
    ResponseHdr rh;
    
    CIMCInstance *inst;
    
    rh = scanCimXmlResponse(xml, NULL);
    
    if (rh.errCode != 0) {
        free(rh.description);
        rh.rvArray->ft->release(rh.rvArray);
        return;
    }
   
    inst = (CIMCInstance*)rh.rvArray->ft->getElementAt(rh.rvArray, 0, NULL).value.inst;
    
    if(inst) {
        i->sendIndicationInstance(inst->ft->clone(inst, NULL));
    }
    
    rh.rvArray->ft->release(rh.rvArray);
}

static void handleConnection(int connFd, struct native_indicationlistener *i)
{
    Buffer inBuf = { NULL, NULL, 0, 0, 0, 0, 0 ,0};
    int badReq = 0;
    int discardInput=0;
    char *path, *hdr;
    char *cp;
    CommHndl conn_fd;
    int rc;
    
    inBuf.authorization = "";
    inBuf.protocol="HTTP/1.1";
    inBuf.content_type = NULL;
    inBuf.content_length = -1;
    inBuf.host = NULL;
    inBuf.useragent = "";
    
    conn_fd.socket=connFd;
    conn_fd.file=fdopen(connFd,"a");
    conn_fd.buf = NULL;
    if (conn_fd.file == NULL) {
        /* failed to create socket stream - continue with raw socket */
    } else {
        conn_fd.buf = malloc(SOCKBUFSZ);
    if (conn_fd.buf) {
        setbuffer(conn_fd.file,conn_fd.buf,SOCKBUFSZ);
    } else {
           /* failed to create socket buffer - continue unbuffered */
        }
    }

    rc=getHdrs(conn_fd, &inBuf,"POST ");
    
    if (rc==1) { 
        genError(conn_fd, &inBuf, 501, "Not Implemented", NULL);
        /* we continue to parse headers and empty the socket
        to be graceful with the client */
        discardInput=1;
    }
    else if (rc==2) {
        genError(conn_fd, &inBuf, 400, "Bad Request", NULL);
        discardInput=2;
        /* potential DOS attempt discovered */      
    }
    else if (rc==3) {
        genError(conn_fd, &inBuf, 400, "Bad Request", NULL);
        /* exiting after request timeout */
        commClose(conn_fd);
        exit(1);
    }
            
    if (inBuf.size == 0) {
        /* no buffer data - end of file - quit */   
        commClose(conn_fd);
        exit(1);
    }
    
    inBuf.httpHdr = getNextHdr(&inBuf);
    for (badReq = 1;;) {
        if (inBuf.httpHdr == NULL) {
            break;
        }
        path = strpbrk(inBuf.httpHdr, " \t\r\n");
        if (path == NULL) {
            break;
        }
        *path++ = 0;
        path += strspn(path, " \t\r\n");
        inBuf.protocol = strpbrk(path, " \t\r\n");
        *inBuf.protocol++ = 0;
        if (inBuf.protocol == NULL) {
            break;
        }
        badReq = 0;
    }
    
    while ((hdr = getNextHdr(&inBuf)) != NULL) {
        if (hdr[0] == 0) {
            break;
        }
        else if (strncasecmp(hdr, "Authorization:", 14) == 0) {
            cp = &hdr[14];
            cp += strspn(cp, " \t");
            inBuf.authorization = cp;
        }
        else if (strncasecmp(hdr, "Content-Length:", 15) == 0) {
            cp = &hdr[15];
            cp += strspn(cp, " \t");
            inBuf.content_length = atol(cp);
        }
        else if (strncasecmp(hdr, "Content-Type:", 13) == 0) {
            cp = &hdr[13];
            cp += strspn(cp, " \t");
            inBuf.content_type = cp;
        }
        else if (strncasecmp(hdr, "Host:", 5) == 0) {
            cp = &hdr[5];
            cp += strspn(cp, " \t");
            inBuf.host = cp;
            if (strchr(inBuf.host, '/') != NULL || inBuf.host[0] == '.') {
                if (!discardInput) {
                    genError(conn_fd, &inBuf, 400, "Bad Request", NULL);
                    discardInput=2;
                }
            }
        }
        else if (strncasecmp(hdr, "User-Agent:", 11) == 0) {
            cp = &hdr[11];
            cp += strspn(cp, " \t");
            inBuf.useragent = cp;
        }
        else if (strncasecmp(hdr, "TE:", 3) == 0) {
            char *cp = &hdr[3];
            cp += strspn(cp, " \t");
            if (strncasecmp(cp,"trailers",8)==0)
            inBuf.trailers=1;
        }
        else if (strncasecmp(hdr, "Expect:", 7) == 0) {
            if (!discardInput) {
                genError(conn_fd, &inBuf, 417, "Expectation Failed", NULL);
                discardInput=2;
            }
        }
    }
    
    if (inBuf.content_length < 0) {
        if (!discardInput) {
            genError(conn_fd, &inBuf, 411, "Length Required", NULL);
        }      
        commClose(conn_fd);
        exit(1);
    }

    getPayload(conn_fd, &inBuf);
    if (discardInput) {
        freeBuffer(&inBuf);
        return;
    }
    
    /* well, not really an error ... op successful */
    genError(conn_fd, &inBuf, 200, "OK", NULL);
    
    processIndication(i, inBuf.content);
    
    freeBuffer(&inBuf);
    free(conn_fd.buf);
}

static void* establish_listener(int sslMode, int port, struct native_indicationlistener *i)
{
    struct sockaddr_in sin;
    socklen_t sz,sin_len;
    int listenFd, connFd;
    int ru;
    struct timeval timeout;
    fd_set socksset;
    int dataRead;
    char *xml;

    listenFd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    sin_len = sizeof(sin);

    memset(&sin, 0, sin_len);
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(port);
    
    ru = 1;
    setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, (char *) &ru, sizeof(ru));
    
    if (bind(listenFd, (struct sockaddr *) &sin, sin_len) ||
        listen(listenFd, 0)) {
      exit(0);
    }
    
    sz = sizeof(sin);
    listen(listenFd, 1);
    
    while(do_listen) {
    
        timeout.tv_sec = 3;
        timeout.tv_usec = 0;
        FD_ZERO(&socksset);
        FD_SET(listenFd,&socksset);
        
        dataRead = select(listenFd+1, &socksset, NULL, NULL, &timeout);
        
        if(dataRead < 0) {
            fprintf(stderr, "Error during select(), return value was: %d\n", dataRead);
        }
        else if(dataRead == 0) {
            /* timed out, trying again */
        } else {
            if ((connFd = accept(listenFd, (__SOCKADDR_ARG) & sin, &sz))<0) {
                fprintf(stderr, "Error during accept(), return value was: %d\n", connFd);
                exit(0);
            }
            handleConnection(connFd, i);
            close(connFd);
        }
    }
}

static void* start_listen_thread(void *parms)
{
    struct native_indicationlistener* i;
    i = (struct native_indicationlistener*) parms;
    establish_listener(i->sslMode, i->port, i);   
}

static CIMCStatus _ilft_release(CIMCIndicationListener* il)
{
    struct native_indicationlistener* i = (struct native_indicationlistener*)
                                          il;
                                          
    if(i) {
        free(i);
    }
}

static CIMCIndicationListener* _ilft_clone(CIMCIndicationListener* il,
                                           CIMCStatus* rc)
{
    /* not yet implemented. function is just here to comply with the default
     * function tables. it is questionable if a clone is needed for an
     * indication listener */
}

static CIMCStatus _ilft_start(CIMCIndicationListener* il)
{
    pthread_t id;
    struct native_indicationlistener* i;
    
    i = (struct native_indicationlistener*) il;
    
    do_listen = 1;
    
    pthread_create(&id, NULL, &start_listen_thread, i);
}

static CIMCStatus _ilft_stop(CIMCIndicationListener* il)
{
    do_listen = 0;
}

CIMCIndicationListener *newCIMCIndicationListener(int sslMode,
                                                  int *portNumber,
                                                  void (*fp) (CIMCInstance *indInstance),
                                                  CIMCStatus *rc)
{
    static CIMCIndicationListenerFT ilft = {
        NATIVECIMXML_FT_VERSION,
        _ilft_release,
        _ilft_clone,
        _ilft_start,
        _ilft_stop
    };
    
    static CIMCIndicationListener il = {
        "CIMCIndicationListener",
        &ilft
    };
    
    struct native_indicationlistener  * indicationlistener =
                (struct native_indicationlistener*) calloc(1,
                   sizeof(struct native_indicationlistener));
                           
    indicationlistener->il = il;
    indicationlistener->port = *portNumber;
    indicationlistener->sslMode = sslMode;
    indicationlistener->sendIndicationInstance = fp;
    
    return (CIMCIndicationListener*) indicationlistener;
}
