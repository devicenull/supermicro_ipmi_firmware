
/*
 * control.c
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
 * Author:        Adrian Schuur <schuur@de.ibm.com>
 *
 * Description:
 *
 * sfcb.cfg config parser.
 *
*/


#include "utilft.h"
#include "support.h"
#include "mlog.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#ifndef SFCB_CONFDIR
#define SFCB_CONFDIR "/etc/sfcb"
#endif

#ifndef SFCB_STATEDIR
#define SFCB_STATEDIR "/var/lib/sfcb"
#endif

#ifndef SFCB_LIBDIR
#define SFCB_LIBDIR "/usr/lib"
#endif

typedef struct control {
   char *id;
   int type;
   char *strValue;
   int dupped;
} Control;

static UtilHashTable *ct = NULL;

char * configfile = NULL;

Control init[] = {
   {"httpPort",         1, "5988"},
   {"enableHttp",       2, "true"},
   {"enableUds",        2, "true"},
   {"httpProcs",        1, "8"},
   {"httpsPort",        1, "5989"},
   {"enableHttps",      2, "false"},
   {"httpsProcs",       1, "8"},
   {"httpLocalOnly",    2, "false"},
#ifdef HAVE_JDBC
   {"dbpPort",         1, "5980"},
   {"enableDbp",         2, "true"},
   {"dbpProcs",        1, "8"},
   {"dbpsPort",         1, "5981"},
   {"enableDbps",         2, "true"},
   {"dbpsProcs",        1, "8"},
#endif
#ifdef HAVE_SLP
   {"slpRefreshInterval",         1, "600"},
   {"slpHostnameLib",   0, "sfcSlpHostname"},
#endif
   {"provProcs",        1, "32"},
   {"basicAuthLib",     0, "sfcBasicAuthentication"},
   {"doBasicAuth",      2, "false"},
   {"doUdsAuth",        2, "false"},
   
   {"useChunking",      2, "false"},
   {"chunkSize",        1, "50000"},
   
   {"keepaliveTimeout",        1, "15"},
   {"keepaliveMaxRequest",     1, "10"},

   {"providerSampleInterval",  1, "30"},
   {"providerTimeoutInterval", 1, "60"},
   {"providerAutoGroup", 2, "true"},

   {"sslKeyFilePath",   0, SFCB_CONFDIR "/file.pem"},
   {"sslCertificateFilePath", 0, SFCB_CONFDIR "/server.pem"},

   {"registrationDir", 0, SFCB_STATEDIR "/registration"},
   {"providerDirs", 3, SFCB_LIBDIR " " SFCB_LIBDIR "/cmpi" }, /* 3: unstripped */

   {"enableInterOp", 2, "true"},
   {"sslClientTrustStore", 0, SFCB_CONFDIR "/client.pem"},
   {"sslClientCertificate", 0, "ignore" },
   {"certificateAuthLib",   0, "sfcCertificateAuthentication"},
   {"localSocketPath",   0, "/tmp/sfcbLocalSocket"},
   {"httpSocketPath",   0, "/tmp/sfcbHttpSocket"},
   
   {"traceFile",   0, "stderr"},
   {"traceLevel",   1, "0"},
   {"traceMask",   1, "0"},

};

void sunsetControl()
{
   int i,m;
   for (i = 0, m = sizeof(init) / sizeof(Control); i < m; i++) {
      if(init[i].dupped) free(init[i].strValue);
   }
   ct->ft->release(ct);
   ct=NULL;
}

int setupControl(char *fn)
{
   FILE *in;
   char fin[1024], *stmt = NULL;
   int i, m, n=0, err=0;
   CntlVals rv;

   if (ct)
      return 0;

   ct = UtilFactory->newHashTable(61, UtilHashTable_charKey |
                UtilHashTable_ignoreKeyCase);

   for (i = 0, m = sizeof(init) / sizeof(Control); i < m; i++) {
      ct->ft->put(ct, init[i].id, &init[i]);
   }

   if (fn) {
     strcpy(fin,fn);
   } 
   else {
      strcpy(fin, SFCB_CONFDIR);  
      strcat(fin, "/sfcb.cfg");
   }
   
   //if (fin[0]=='/') mlogf(M_INFO,M_SHOW,"--- Using %s\n",fin);
   //else mlogf(M_INFO,M_SHOW,"--- Using ./%s\n",fin);
   in = fopen(fin, "r");
   if (in == NULL) {
      mlogf(M_ERROR,M_SHOW, "--- %s not found\n", fin);
      return -2;
   }

   while (fgets(fin, 1024, in)) {
      n++;
      if (stmt) free(stmt);
      stmt = strdup(fin);
      switch (cntlParseStmt(fin, &rv)) {
      case 0:
      case 1:
         mlogf(M_ERROR,M_SHOW,"--- control statement not recognized: \n\t%d: %s\n", n, stmt);
         err = 1;
         break;
      case 2:
         for (i=0; i<sizeof(init)/sizeof(Control); i++) {
            if (strcmp(rv.id, init[i].id) == 0) {
	      if (init[i].type == 3) {
		/* unstripped character string */
		init[i].strValue=strdup(rv.val);
		if (strchr(init[i].strValue,'\n'))
		  *(strchr(init[i].strValue,'\n')) = 0;
                init[i].dupped=1; 
	      } 
              else {
		init[i].strValue=strdup(cntlGetVal(&rv));
                init[i].dupped=1; 
	      }
	      goto ok;
            }
         }
         mlogf(M_ERROR,M_SHOW,"--- invalid control statement: \n\t%d: %s\n", n, stmt);
         err = 1;
      ok:
         break;
      case 3:
         break;
      }
   }

   if (stmt) free(stmt);


   if (in) {
     fclose(in);
   }

   if (err) {
      mlogf(M_INFO,M_SHOW,"--- Broker terminated because of previous error(s)\n");
      abort();
   }

   return 0;
}

int getControlChars(char *id, char **val)
{
   Control *ctl;
   int rc = -1;
   if ((ctl = ct->ft->get(ct, id))) {
      if (ctl->type == 0 || ctl->type == 3) {
         *val = ctl->strValue;
         return 0;
      }
      rc = -2;
   }
   *val = NULL;
   return rc;
}

int getControlNum(char *id, long *val)
{
   Control *ctl;
   int rc = -1;
   if ((ctl = ct->ft->get(ct, id))) {
      if (ctl->type == 1) {
	*val = strtol(ctl->strValue,NULL,0);
         return 0;
      }
      rc = -2;
   }
   *val = 0;
   return rc;
}

int getControlBool(char *id, int *val)
{
   Control *ctl;
   int rc = -1;
   if ((ctl = ct->ft->get(ct, id))) {
      if (ctl->type == 2) {
         *val = strcasecmp(ctl->strValue,"true")==0;
         return 0;
      }
      rc = -2;
   }
   *val = 0;
   return rc;
}
