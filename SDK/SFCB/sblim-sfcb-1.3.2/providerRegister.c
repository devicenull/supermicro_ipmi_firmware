
/*
 * providerRegister.c
 *
 * © Copyright IBM Corp. 2005, 2007
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:        Adrian Schuur <schuur@de.ibm.com>
 * Based on concepts developed by Viktor Mihajlovski <mihajlov@de.ibm.com>
 *
 * Description:
 *
 * Provider registration support.
 *
*/


#include "utilft.h"
#include "mlog.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <support.h>

#include "providerRegister.h"

static ProviderInfo forceNotFound={"",FORCE_PROVIDER_NOTFOUND};
extern unsigned long exFlags;
extern char * configfile;
extern int setupControl(char *fn);
extern int getControlChars(char *id, char **val);

ProviderInfo *qualiProvInfoPtr = NULL;
ProviderInfo *classProvInfoPtr = NULL;
ProviderInfo *defaultProvInfoPtr = NULL;
ProviderInfo *interOpProvInfoPtr = NULL;
ProviderInfo *forceNoProvInfoPtr = &forceNotFound;



static void freeInfoPtr(ProviderInfo *info)
{
   int n=0;
   if(info->nextInRegister) {
      freeInfoPtr(info->nextInRegister);
   }
   free(info->className); 
   free(info->providerName); 
   free(info->location);
   free(info->group);
   n=0;
   if (info->ns) while (info->ns[n]) free(info->ns[n++]); 
   free(info->ns);
   free(info);
}

static void pRelease(ProviderRegister * br)
{
   ProviderBase *bb = (ProviderBase *) br->hdl;
   char *key = NULL;
   ProviderInfo *info = NULL;
   HashTableIterator *it;
   
   for (it = bb->ht->ft->getFirst(bb->ht, (void **) &key, (void **) &info);
        key && it && info;
        it = bb->ht->ft->getNext(bb->ht, it, (void **) &key, (void **) &info)) {
      freeInfoPtr(info);
   }
   
   free(bb->fn);
   bb->ht->ft->release(bb->ht);
   free(br);
}

static ProviderRegister *pClone(ProviderRegister * br)
{
   return NULL;
}

static void addProviderToHT(ProviderInfo *info, ProviderRegister *br)
{
    ProviderInfo *checkDummy;
    UtilHashTable *ht = ((ProviderBase *) br->hdl)->ht;
    
    /* first we add the provider to the providerRegister with the classname
     * as a key*/
	
    checkDummy = ht->ft->get(ht, info->className);
    if(checkDummy) {
    /* Another provider is in the register, but the newly found
     * wants to serve the same class - so we do not add it to the
     * register but append it to the one already found or to its master*/
        if(strcmp(checkDummy->providerName, info->providerName) == 0) {
            /* double registration - discard */
            freeInfoPtr(info);
            return;
        } else {
            checkDummy->nextInRegister = info;
        }
    } else {
        ht->ft->put(ht, info->className, info);
    }
}

ProviderRegister *newProviderRegister(char *fn)
{
   FILE *in;
   char *dir;
   char fin[1024], *stmt = NULL;
   ProviderInfo *info = NULL;
   int err = 0, n = 0, i;
   CntlVals rv;
   int id=0;
   int interopFound=0;

   ProviderRegister *br = (ProviderRegister *) malloc(sizeof(ProviderRegister) +
                                                      sizeof(ProviderBase));
   ProviderBase *bb = (ProviderBase *) br + 1;
   
   setupControl(configfile);

   if (getControlChars("registrationDir",&dir)) {
     dir = "/var/lib/sfcb/registration";
   }

   strcpy(fin, dir);
   strcat(fin, "/providerRegister");
   in = fopen(fin, "r");
   if (in == NULL) 
      mlogf(M_ERROR,M_SHOW, "--- %s not found\n", fin);
      
   else {

      br->hdl = bb;
      br->ft = ProviderRegisterFT;
      bb->fn = strdup(fin);
      bb->ht = UtilFactory->newHashTable(61,
                  UtilHashTable_charKey | UtilHashTable_ignoreKeyCase);

      while (fgets(fin, 1024, in)) {
         n++;
         if (stmt) free(stmt);
         stmt = strdup(fin);
         switch (cntlParseStmt(fin, &rv)) {
         case 0:
            mlogf(M_ERROR,M_SHOW,"--- registration statement not recognized: \n\t%d: %s\n", n,stmt);
            err = 1;
            break;
         case 1:
            if (info) {
               if (classProvInfoPtr==NULL) {
                  if (strcmp(info->className,"$ClassProvider$")==0) classProvInfoPtr=info;
               }   
               else if (defaultProvInfoPtr==NULL) {
                  if (strcmp(info->className,"$DefaultProvider$")==0) defaultProvInfoPtr=info;
               }   
               else if (interOpProvInfoPtr==NULL) {
                  if (strcmp(info->className,"$InterOpProvider$")==0) {
                     if (exFlags & 2) interOpProvInfoPtr=info;
                     else interopFound=1;
                  }   
               }               
               else if (qualiProvInfoPtr==NULL) {
                  if (strcmp(info->className,"$QualifierProvider$")==0) qualiProvInfoPtr=info;
               }
               addProviderToHT(info, br);
            }
            info = (ProviderInfo *) calloc(1, sizeof(ProviderInfo));
            info->className = strdup(rv.id);
            info->id= ++id;
            break;
         case 2:
            if (strcmp(rv.id, "provider") == 0)
               info->providerName = strdup(cntlGetVal(&rv));
            else if (strcmp(rv.id, "location") == 0)
               info->location = strdup(cntlGetVal(&rv));
            else if (strcmp(rv.id, "parameters") == 0) {
               info->parms = strdup(cntlGetStr(&rv));
               for (i=strlen(info->parms); i>0 && info->parms[i]<=' '; i--) {
                   info->parms[i]=0;
               }
            }
            else if (strcmp(rv.id, "group") == 0)
               info->group = strdup(cntlGetVal(&rv));
            else if (strcmp(rv.id, "unload") == 0) {
               char *u;
               info->unload = 0;
               while ((u = cntlGetVal(&rv)) != NULL) {
                  if (strcmp(u, "never") == 0) {
                     info->unload =-1;
                  }   
                  else {
                     mlogf(M_ERROR,M_SHOW,"--- invalid unload specification: \n\t%d: %s\n", n, stmt);
                     err = 1;
                  }
               }   
            }   
            else if (strcmp(rv.id, "type") == 0) {
               char *t;
               info->type = 0;
               while ((t = cntlGetVal(&rv)) != NULL) {
                  if (strcmp(t, "instance") == 0)
                     info->type |= INSTANCE_PROVIDER;
                  else if (strcmp(t, "association") == 0)
                     info->type |= ASSOCIATION_PROVIDER;
                  else if (strcmp(t, "method") == 0)
                     info->type |= METHOD_PROVIDER;
                  else if (strcmp(t, "indication") == 0)
                     info->type |= INDICATION_PROVIDER;
                  else if (strcmp(t, "class") == 0)
                     info->type |= CLASS_PROVIDER;
                  else if (strcmp(t, "property") == 0)
                     info->type |= PROPERTY_PROVIDER; 
                  else if (strcmp(t, "qualifier") == 0)
                     info->type |= QUALIFIER_PROVIDER;                     
                  else {
                     mlogf(M_ERROR,M_SHOW,"--- invalid type specification: \n\t%d: %s\n", n, stmt);
                     err = 1;
                  }
               }
            }
            else if (strcmp(rv.id, "namespace") == 0) {
               int max=1,next=0;
               char *t;
               info->ns=(char**)malloc(sizeof(char*)*(max+1));
               while ((t = cntlGetVal(&rv)) != NULL) {
                  if (next==max) {
                     max++;
                     info->ns=(char**)realloc(info->ns,sizeof(char*)*(max+1));
                  }
                  info->ns[next]=strdup(t);
                  info->ns[++next]=NULL;
               }
            }
            else {
               mlogf(M_ERROR,M_SHOW,"--- invalid registration statement: \n\t%d: %s\n", n, stmt);
               err = 1;
            }
            break;
         case 3:
            break;
         }
      }

      if (info) {
      	 addProviderToHT(info, br);
      }
   }
   if (in) {
     fclose(in);
   }
   
   if (classProvInfoPtr==NULL) {
      mlogf(M_ERROR,M_SHOW,"--- Class provider definition not found - sfcbd will terminate\n");
      err=1;
   }
   
   if (defaultProvInfoPtr==NULL) 
      mlogf(M_INFO,M_SHOW,"--- Default provider definition not found - no instance repository available\n");

   if (qualiProvInfoPtr==NULL) 
      mlogf(M_INFO,M_SHOW,"--- Qualifier provider definition not found - no qualifier support available\n");
   
   if (interOpProvInfoPtr==NULL) {
      if (exFlags & 2 && interopFound==0)
         mlogf(M_INFO,M_SHOW,"--- InterOp provider definition not found - no InterOp support available\n");
      else if (interopFound)    
         mlogf(M_INFO,M_SHOW,"--- InterOp provider definition found but not started - no InterOp support available\n");
      interOpProvInfoPtr=&forceNotFound;
   }   
   
   if (err) {
      mlogf(M_ERROR,M_SHOW,"--- Broker terminated because of previous error(s)\n");
      exit(5);
   }
   if (stmt) free(stmt);
   return br;
}

static int putProvider(ProviderRegister * br, const char *clsName,
                       ProviderInfo * info)
{
   ProviderBase *bb = (ProviderBase *) br->hdl;
   return bb->ht->ft->put(bb->ht, clsName, info);
}

static ProviderInfo *getProvider(ProviderRegister * br,
                                 const char *clsName, unsigned long type)
{
   ProviderBase *bb = (ProviderBase *) br->hdl;
   ProviderInfo *info = bb->ht->ft->get(bb->ht, clsName);
   if (info && info->type & type) {
      return info;
   }
   return NULL;
}

static ProviderInfo *locateProvider(ProviderRegister * br, const char *provName)
{
   ProviderBase *bb = (ProviderBase *) br->hdl;
   HashTableIterator *it;
   char *key = NULL;
   ProviderInfo *info = NULL;

   for (it = bb->ht->ft->getFirst(bb->ht, (void **) &key, (void **) &info);
        key && it && info;
        it = bb->ht->ft->getNext(bb->ht, it, (void **) &key, (void **) &info)) {
      if (strcasecmp(info->providerName, provName) == 0)
         return info;
   }
   return NULL;
}

static void resetProvider(ProviderRegister * br, int pid)
{
   ProviderBase *bb = (ProviderBase *) br->hdl;
   HashTableIterator *it;
   char *key = NULL;
   ProviderInfo *info = NULL;
   
   for (it = bb->ht->ft->getFirst(bb->ht, (void **) &key, (void **) &info);
        key && it && info;
        it = bb->ht->ft->getNext(bb->ht, it, (void **) &key, (void **) &info)) {
      if (info->pid==pid) info->pid=0;
   }     
}

static void removeProvider(ProviderRegister * br, const char *clsName)
{
   ProviderBase *bb = (ProviderBase *) br->hdl;
   bb->ht->ft->remove(bb->ht, clsName);
}

static Provider_Register_FT ift = {
   1,
   pRelease,
   pClone,
   getProvider,
   putProvider,
   removeProvider,
   locateProvider,
   resetProvider
};

Provider_Register_FT *ProviderRegisterFT = &ift;

