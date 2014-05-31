
/*
 * $Id: cimcclient.c,v 1.2 2007/02/07 18:09:58 mihajlov Exp $
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
 * Author:        Viktor Mihajlovski <mihajlov@de.ibm.com>
 *
 * Description:
 *
 * CIM C Client Loader Implementation
 *
 */

#include "cimc.h"

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

/*
 * Canonical CIM C API entry points
 */

#define LIBLEN 512
#define ENTLEN 256
#define ERRLEN 1024


CIMCEnv* NewCIMCEnv(const char *id, unsigned int options, int *rc, char **msg)
{
   char         libName[LIBLEN+1];
   char         entry[ENTLEN+1];
   void        *library; 
   InitCimcEnv  init=NULL;
   CIMCEnv     *rv=NULL;
   
   libName[LIBLEN]=0;
   entry[ENTLEN]=0;
   if(strcmp(id,"SfcbLocal") != 0)
        snprintf(libName, LIBLEN, "/wsman/openwsman/lib/libcimcClient%s.so",id);
   else
        snprintf(libName, LIBLEN, "/SFCB/local/lib/libcimcClient%s.so",id);
//printf("libName = %s\n", libName);
   
   library = dlopen(libName, RTLD_LAZY);
   if (library==NULL) {
printf("library == NULL\n");
     *msg=calloc(1,ERRLEN+1);
     *rc=1;
     snprintf(*msg,ERRLEN,"Unable to load %s library %s",libName,dlerror());
   } else {
//printf("library != NULL\n");
     snprintf(entry, ENTLEN,"_Create_%s_Env", id);
     init = (InitCimcEnv)dlsym(library, entry);
     if (init==NULL) {
       *msg=calloc(1,ERRLEN+1);
       *rc=2;
       snprintf(*msg,ERRLEN,"Unable to locate entry %s in %s",entry,libName);
     } else {
       *rc=0;
       *msg=NULL;
       rv = init(id,options,rc,msg); 
       rv->hdl=library;
     }
   }
   return rv;
}

void ReleaseCIMCEnv(CIMCEnv *env)
{
   void *lib=NULL;
   if (env) {
     lib = env->hdl;
     env->ft->release(env);
     if (lib) {
       dlclose(lib);
     }
   }
}
