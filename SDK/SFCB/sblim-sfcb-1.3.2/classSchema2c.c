
/*
 * classSchema.c
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
 *
 * Description:
 *
 * A small program converting a classSchema file to a .c representation 
 * that can be compiled.
 *
*/

#include "utilft.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <byteswap.h>

#include "constClass.h"
#include "providerRegister.h"
#include "trace.h"
#include "control.h"

#define NEW(x) ((x *) malloc(sizeof(x)))

#include "cmpidt.h"
#include "cmpift.h"
#include "cmpiftx.h"
#include "cmpimacs.h"
#include "cmpimacsx.h"
#include "objectImpl.h"
#include "mrwlock.h"

#include <time.h>

#define LOCALCLASSNAME "ClassProvider"

typedef void *(*SwapClass)(ClClass * cls, int *size);
static SwapClass swapEntryClass;
static int swapMode;

extern char * configfile;
extern ProviderRegister *pReg;

typedef struct _Class_Register_FT Class_Register_FT;
struct _ClassRegister {
   void *hdl;
   Class_Register_FT *ft;
   ClVersionRecord *vr;
   int assocs,topAssocs;
   char *fn;
};
typedef struct _ClassRegister ClassRegister;

typedef struct _ClassBase {
   UtilHashTable *ht;
   UtilHashTable *it;
   MRWLOCK mrwLock;
} ClassBase;

struct _Class_Register_FT {
   int version;
   void (*release) (ClassRegister * br);
   ClassRegister *(*clone) (ClassRegister * br);
   CMPIConstClass *(*getClass) (ClassRegister * br, const char *clsName);
   int (*putClass) (ClassRegister * br, CMPIConstClass * cls);
   int (*removeClass) (ClassRegister * br, const char *className);
   UtilList *(*getChildren) (ClassRegister * br, const char *className);
   void (*rLock)(ClassRegister * cr);
   void (*wLock)(ClassRegister * cr);
   void (*rUnLock)(ClassRegister * cr);
   void (*wUnLock)(ClassRegister * cr);
};

//extern Class_Register_FT *ClassRegisterFT;


typedef struct nameSpaces {
   int next,max,blen;
   char *base;
   char *names[1];
} NameSpaces;


static void buildClassSource(ClassRegister * cr, char *ns)
{
   ClassBase *cb = (ClassBase *) (cr + 1);
   UtilHashTable *ct = cb->ht, *it;
   HashTableIterator *i;
   char *cn;
   CMPIConstClass *cc;
   ClObjectHdr *hdr;
   unsigned char *buf;
   int j;
   int size;

   it = cb->it = UtilFactory->newHashTable(61,
             UtilHashTable_charKey | UtilHashTable_ignoreKeyCase);

   fprintf(stdout,"\n#include \"classSchemaMem.h\"\n\n"); 
   fprintf(stdout,"\n#include <stddef.h>\n\n"); 
   
   fprintf(stdout,"static unsigned char version[]={"); 
   
   buf=(unsigned char*)cr->vr;
   if (swapMode) {
     cr->vr->size=bswap_32(cr->vr->size);
   }
   if (buf) {
      fprintf(stdout,"\n    0x%.2x",buf[0]);
      for (j=1; j<sizeof(ClVersionRecord); j++) {
         fprintf(stdout,",0x%.2x",buf[j]);
	 if ((j+1)%15 == 0) {
	   fprintf(stdout,"\n   ");
	 }
      }
      fprintf(stdout,"\n};\n\n"); 
   }
   
   for (i = ct->ft->getFirst(ct, (void **) &cn, (void **) &cc); i;
        i = ct->ft->getNext(ct, i, (void **) &cn, (void **) &cc)) {

      hdr=(ClObjectHdr*)cc->hdl; 
      fprintf(stderr,"Class: %s - %d\n",  cc->ft->getCharClassName(cc),hdr->size); 
      
      fprintf(stdout,"static unsigned char %s[]={",cc->ft->getCharClassName(cc)); 
      
      if (swapMode) {
	hdr = swapEntryClass((ClClass*)hdr,&size);	
      } else {
	size = hdr->size;
      }
      
      buf=(unsigned char*)hdr;   
      fprintf(stdout,"\n    0x%.2x",buf[0]);
      for (j=1; j<size; j++) {
         fprintf(stdout,",0x%.2x",buf[j]);
	 if ((j+1)%15 == 0) {
	   fprintf(stdout,"\n   ");
	 }
      }
      fprintf(stdout,"\n};\n"); 
   }
   
   fprintf(stdout,"\nstatic ClassDir classes[]={\n"); 
   for (i = ct->ft->getFirst(ct, (void **) &cn, (void **) &cc); i;
        i = ct->ft->getNext(ct, i, (void **) &cn, (void **) &cc)) {
      fprintf(stdout,"\t{\"%s\",%s},\n",cc->ft->getCharClassName(cc),cc->ft->getCharClassName(cc)); 
   }        
   fprintf(stdout,"\t{NULL,NULL}};\n"); 
   fprintf(stdout,"\nClassSchema %s_classes={%s,classes};\n\n",ns,cr->vr ? "version" : "NULL");
       
}

static ClassRegister *newClassRegister(char *fname, char *ns)
{
   ClassRegister *cr =
       (ClassRegister *) malloc(sizeof(ClassRegister) + sizeof(ClassBase));
   ClassBase *cb = (ClassBase *) (cr + 1);
   FILE *in;
   char fin[1024];
   long s, total=0;
   ClObjectHdr hdr;
   ClVersionRecord *vrp=(ClVersionRecord*)&hdr;
   int vRec=0,first=1;
   
   cr->hdl = cb;
//   cr->ft = ClassRegisterFT;
   cr->vr = NULL;
   cr->assocs = cr->topAssocs = 0;
   
   strcpy(fin, fname);
   strcat(fin, "/classSchemas");
   fprintf(stderr,"Opening %s\n",fname);
   in = fopen(fin, "r");
   
   if (in == NULL) {
      fprintf(stderr,"-*- classSchema directory %s not found\n",fname);
      return NULL;
   }

   cr->fn = strdup(fin);
   cb->ht = UtilFactory->newHashTable(61,
               UtilHashTable_charKey | UtilHashTable_ignoreKeyCase);
   MRWInit(&cb->mrwLock);

   while ((s = fread(&hdr, 1, sizeof(hdr), in)) == sizeof(hdr)) {
      CMPIConstClass *cc=NULL;
      char *buf=NULL;
      char *cn;

      if (first) {
         if (vrp->size==sizeof(ClVersionRecord) && vrp->type==HDR_Version) vRec=1;
         else if (vrp->size==sizeof(ClVersionRecord)<<24 && vrp->type==HDR_Version) {
            mlogf(M_ERROR,M_SHOW,"--- %s is in wrong endian format\n",fin);
	    fclose(in);
            return NULL;
         }
      }
      
      if (vRec==0 && hdr.type!=HDR_Class) {
         mlogf(M_ERROR,M_SHOW,"--- %s contains non-class record(s)\n",fin);
	 fclose(in);
         return NULL;
     }
      
      buf = (char *) malloc(hdr.size);
      if (buf==NULL) {
         mlogf(M_ERROR,M_SHOW,"--- %s contains record(s) that are too long\n",fin);
	 fclose(in);
         return NULL;
      }
      
      s=hdr.size;
      *((ClObjectHdr *) buf) = hdr;
      
      if (fread(buf + sizeof(hdr), 1, hdr.size - sizeof(hdr), in) == hdr.size - sizeof(hdr)) {
         if (vRec) {	    
            cr->vr=(ClVersionRecord*)buf;
	    vrp = NEW(ClVersionRecord);
	    memcpy(vrp,cr->vr,sizeof(ClVersionRecord));
            if (strcmp(cr->vr->id,"sfcb-rep")) {
               mlogf(M_ERROR,M_SHOW,"--- %s contains invalid version record - directory skipped\n",fin);
	       fclose(in);
               return NULL;
            }   
            vRec=0;
         } else {
	   vrp = NULL;
	 }

         if (first) {
            int v=-1;
            first=0;
            if (ClVerifyObjImplLevel(vrp)) continue;
            if (vrp) v=vrp->objImplLevel;
            mlogf(M_ERROR,M_SHOW,"--- %s contains unsupported object implementation format (%d) - directory skipped\n",
               fin,v);
	    fclose(in);
            return NULL;
         }

                
         cc = NEW(CMPIConstClass);
         cc->hdl = buf;
         cc->ft = CMPIConstClassFT;
         cc->ft->relocate(cc);
         cn=(char*)cc->ft->getCharClassName(cc);
         if (strncmp(cn,"DMY_",4)!=0) {    

            total+=s;
            cb->ht->ft->put(cb->ht, cn, cc);
            if (cc->ft->isAssociation(cc)) {
               cr->assocs++;
               if (cc->ft->getCharSuperClassName(cc) == NULL) cr->topAssocs++;
            }   
         } 
      }
      else {
         mlogf(M_ERROR,M_SHOW,"--- %s contains invalid record(s) - directory skipped\n",fin);
	 fclose(in);
         return NULL;
      }
   }
 
   fclose(in);

   if (cr->vr) {
      mlogf(M_INFO,M_SHOW,"--- ClassProvider for %s (%d.%d) using %ld bytes\n", 
          fname, cr->vr->version, cr->vr->level, total);
   }
   else mlogf(M_INFO,M_SHOW,"--- ClassProvider for %s (no-version) using %ld bytes\n", fname, total);

   buildClassSource(cr,ns);
   
   return cr;
}


static int parse_options(int argc, char * argv[])
{
  int opt;
  while ((opt=getopt(argc,argv,"X:")) != -1) {
    switch (opt) {
    case 'X': 
      if (strcmp(optarg,"P32") == 0) {
#ifdef SFCB_IX86
	swapMode = 1;
#else 
	fprintf(stderr,"P32 only allowed on i386 platforms.\n");
	return -1;
#endif
      } else {
	fprintf(stderr,"unsupported cross-build platform %s.\n",optarg);
	return -1;
      }
      break;
    default:
      return -1;
    }
  }
  return optind;
}

int main(int argc, char *argv[])
{
   char *ns,*fname;
   time_t tme;
   int i;
   int argidx;

   argidx = parse_options(argc, argv);
   
   if (argidx < 0 || argidx > argc - 1) {
      fprintf(stderr,
	      "usage: %s [-X P32] <classSchema-directory> [<classSchema-prefix>]\n",
	      argv[0]);
      return -1;
   }
   
   if (swapMode) {
     void *swapLib=dlopen("libsfcObjectImplSwapI32toP32.so", RTLD_LAZY);
     if (swapLib==NULL) {
       fprintf (stderr, "--- swap library not found - %s\n", dlerror());
       exit(16);
     }
     swapEntryClass = dlsym(swapLib, "swapI32toP32Class");
     if (swapEntryClass==NULL) {
       fprintf (stderr, "--- swap entry point not found - %s\n", dlerror());
       exit(16);
     }
   }

   fname=argv[argidx];
   
   if (argc==argidx+2) ns=argv[argidx+1];
   else {
      for (i=strlen(fname); i && fname[i]!='/'; i--);
      ns=fname+i+1; 
   }
   
   tme=time(NULL);
   fprintf(stdout,"/* File generated by classSchema2c %s */\n",ctime(&tme)); 
   if (swapMode) {
     fprintf(stdout,"/* SWAPMODE! */\n"); 
   }
   fprintf(stdout,"/* input: %s */\n",fname); 
   fprintf(stdout,"/*  ns:    %s */\n\n",ns); 
  
   if (newClassRegister(fname,ns)==NULL) {
         return -1;
   }
   
   return 0;
}
