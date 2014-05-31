
/*
 * $Id: fileRepository.c,v 1.16 2008/06/25 17:53:45 buccella Exp $
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
 * Directory/file based respository implementation.
 *
*/


//#define CMPI_VERSION 90

#include <stdio.h>
#include <alloca.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#include "trace.h"
#include "cmpimacs.h"
#include "cmpidt.h"
#include "cmpift.h"
#include "fileRepository.h"
#include "mlog.h"
#include "control.h"


#define BASE "repository"

char *repfn=NULL;

static char *getRepDir()
{
   int keyl;
   char *dir;
   
   if (repfn) return repfn;
   
   if (getControlChars("registrationDir",&dir)) {
     dir = "/var/lib/sfcb/registration";
   }
   keyl=strlen(BASE)+strlen(dir);
   repfn=(char*)malloc(keyl+64);
   
   strcpy(repfn,dir);
   strcat(repfn,"/");
   strcat(repfn,BASE);
   strcat(repfn,"/");

   return repfn;      
}

static int getIndexRecordCase(BlobIndex * bi, const char *key, size_t keyl, char **keyb, size_t *keybl, short ignorecase)
/* returns -1 failure, 0 OK, 1 not found (only with key)
 */
{
  static const char *delims = " \t";
  static const char *num = "0123456789";
  char *tokenptr; 
  char *kbptr; 
  int elen, ekl;
  int blen, bofs;
  int slen;
  
  /* check index range */
  if (bi->next > bi->dSize) {
    return -1;
  }
  tokenptr = bi->index + bi->next;
  /* get record length */
  /* trim white space first */
  slen = strspn(tokenptr,delims);
  tokenptr += slen;
  slen = strspn(tokenptr,num);
  /* get digits */
  if (slen == 0) {
    /* no more token found */
    return -1;
  }
  elen = atoi(tokenptr);
  if (elen <= 0) {
    /* record length not valid */
    return -1;
  }
  tokenptr += slen;
  /* get keybinding length */
  /* trim white space first */
  slen = strspn(tokenptr,delims);
  tokenptr += slen;
  slen = strspn(tokenptr,num);
  /* get digits */
  if (slen == 0) {
    /* no more token found */
    return -1;
  }
  ekl = atoi(tokenptr);
  if (ekl <= 0) {
    /* keybinding length not valid */
    return -1;
  }
  tokenptr += slen;
  /* skip keybindings and get blob length */
  /* trim white space first */
  slen = strspn(tokenptr,delims);
  kbptr = tokenptr + slen;
  tokenptr = kbptr + ekl;
  slen = strspn(tokenptr,delims);
  tokenptr += slen;
  slen = strspn(tokenptr,num);
  /* get digits */
  if (slen == 0) {
    /* no more token found */
    return -1;
  }
  blen = atoi(tokenptr);
  if (blen <= 0) {
    /* blob length not valid */
    return -1;
  }
  tokenptr += slen;
  /* get blob offset */
  /* trim white space first */
  slen = strspn(tokenptr,delims);
  tokenptr += slen;
  slen = strspn(tokenptr,num);
  /* get digits */
  if (slen == 0) {
    /* no more token found */
    return -1;
  }
  bofs = atoi(tokenptr);
  if (bofs < 0) {
    /* offset not valid */
    return -1;
  }
  /* set up blob info and advance to next record */
  bi->pos = bi->next;
  bi->len = elen;
  bi->blen = blen;
  bi->bofs = bofs;
  bi->next += elen;
  /* check if key was specified and matches */
  if (keyl > 0) {
    if (ignorecase) {
      if (keyl != ekl || strncasecmp(kbptr,key,keyl)) {
	/* mismatch */
	return 1;
      }
    }
    else {
      if (keyl != ekl || strncmp(kbptr,key,keyl)) {
	/* mismatch */
	return 1;
      }
    }
  }
  /* set found keybindings if possible */
  if (keyb && keybl) {
    *keyb = kbptr;
    *keybl = ekl;
  }
  return 0;  
}

static int getIndexRecord(BlobIndex * bi, char *key, size_t keyl, char **keyb, size_t *keybl)
{
  // use case-sensitive compare on the key
  return getIndexRecordCase(bi, key, keyl, keyb, keybl, 0);
}

void freeBlobIndex(BlobIndex **bip, int all)
{
   BlobIndex *bi;
   if (bip==NULL) return;
   bi=*bip;

   if (bi==NULL) return;
   if (bi->freed) return;
   if (bi->dir) { free(bi->dir); bi->dir=NULL; }
   if (bi->fnx) { free(bi->fnx); bi->fnx=NULL; }
   if (bi->fnd) { free(bi->fnd); bi->fnd=NULL; }
   if (all) if (bi->index)  { free(bi->index); bi->fnd=NULL; }
   bi->freed=-1;
   if (bi->fd) fclose(bi->fd);
   if (bi->fx) fclose(bi->fx);
   free(bi);
   *bip=NULL;
}

static int indxLocateCase(BlobIndex *bi,const char *key,short ignorecase)
{
   int loc;
   int kl=strlen(key);
   bi->next = 0;
   while (bi->next < bi->dSize) {
     loc = getIndexRecordCase(bi,key,kl,NULL,0,ignorecase);
     if (loc < 0) {
       return 0;
     } else if (loc == 0) {
       /* found */
       return 1;
     }
   }
   return 0;
}

static int indxLocate(BlobIndex *bi,const char *key) {
  // use case-sensitive compare on the key
  return indxLocateCase(bi, key, 0);
}

void* getFirst(BlobIndex *bi, int *len, char** keyb, size_t *keybl)
{
   char *buf = NULL;
   bi->next=0;

   if (getIndexRecord(bi,NULL,0,keyb,keybl) == 0) {
     bi->fd=fopen(bi->fnd,"rb");
     fseek(bi->fd,bi->bofs,SEEK_SET);
     buf=(char*)malloc(bi->blen+8);
     fread(buf,bi->blen,1,bi->fd);
     buf[bi->blen]=0;
     if (len) *len=bi->blen;
   } else if (len) {
     *len = 0;
   }

   return (void*)buf;
}

void* getNext(BlobIndex *bi, int *len, char** keyb, size_t *keybl)
{
   char *buf = NULL;

   if (getIndexRecord(bi,NULL,0,keyb,keybl) == 0) {
     fseek(bi->fd,bi->bofs,SEEK_SET);
     buf=(char*)malloc(bi->blen+8);
     fread(buf,bi->blen,1,bi->fd);
     buf[bi->blen]=0;    
     if (len) *len=bi->blen;
   } else {
     fclose(bi->fd);
     bi->fd=NULL;
     if (len) *len=0;
   }

   return (void*)buf;
}

static void copy(FILE *o, FILE *i, int len, unsigned long ofs)
{
   char *buf=(char*)malloc(len);
   fseek(i,ofs,SEEK_SET);
   fread(buf,len,1,i);
   fwrite(buf,len,1,o);
   free(buf);
}

static int adjust(BlobIndex *bi, int pos, int adj)
{
   int dp=pos,l,o,sl;
   char *p,*r = NULL,str[32];
   while (dp<bi->dSize) {
      memset(str,' ',sizeof(str)-1);
      str[sizeof(str)-1]=0;
      l=atoi(bi->index+dp);
      for (p=bi->index+dp+l-2; *p!=' '; p--) if (*p=='\r') r=p;
      o=atoi(++p);
      o-=adj;
      sl=sprintf(str+(r-p),"%d",o);
      memcpy(p,str+sl,r-p);
      dp+=l;
   }
   return 0;
}

static int rebuild(BlobIndex *bi, const char *id, void *blob, int blen)
{
   int ofs,len,xt=0,dt=0;
   unsigned long pos;
   char *xn=alloca(strlen(bi->dir)+8);
   char *dn=alloca(strlen(bi->dir)+8);
   FILE *x,*d;

   strcpy(xn,bi->dir);
   strcat(xn,"idx");
   strcpy(dn,bi->dir);
   strcat(dn,"inst");
   x=fopen(xn,"wb");
   d=fopen(dn,"wb");

   if (bi->bofs) copy(d,bi->fd,bi->bofs,0);
   dt+=bi->bofs;
   len=bi->dlen-(bi->bofs+bi->blen);
   if (len) copy(d,bi->fd,bi->dlen-(bi->bofs+bi->blen),bi->bofs+bi->blen);
   dt+=len;
   pos=ftell(d);
   if (blen) fwrite(blob,blen,1,d);
   dt+=blen;
   fclose(d);

   adjust(bi,bi->pos,bi->blen);

   ofs=bi->pos+bi->len;
   if (bi->pos) fwrite(bi->index,bi->pos,1,x);
   xt+=bi->pos;
   len=bi->dSize-ofs;
   if (len) fwrite(bi->index+ofs,len,1,x);
   xt+=len;
   fclose(x);

   remove(bi->fnd);
   remove(bi->fnx);
   if (dt) rename(dn,bi->fnd);
   else remove(dn);
   if (xt) rename(xn,bi->fnx);
   else remove(xn);

   return 0;
}

int getIndex(const char *ns, const char *cls, int elen, int mki, BlobIndex **bip)
{
   BlobIndex *bi;
   char *fn;
   char *p;
   char *dir;

   dir=getRepDir();
   
   fn=alloca(strlen(dir)+strlen(ns)+strlen(cls)+8);
   
   bi=NEW(BlobIndex);

   strcpy(fn,dir);
   p=fn+strlen(fn);
   strcat(fn,ns);
   strcat(fn,"/");
   while (*p) { *p=tolower(*p); p++; }
   bi->dir=strdup(fn);

   p=fn+strlen(fn);   
   strcat(fn,cls);
   
   while (*p) { *p=tolower(*p); p++; }
   
   bi->fnd=strdup(fn);
   strcat(fn,".idx");
   bi->fnx=strdup(fn);    

   bi->fx=fopen(bi->fnx,"rb+");
   if (bi->fx==NULL) {
      if (mki==0) {
         freeBlobIndex(&bi,1);
         *bip=NULL;
         return 0;
      }
      bi->fx=fopen(bi->fnx,"wb");
      bi->aSize=elen;
      bi->dSize=0;
      bi->index=malloc(bi->aSize);
   }

   else {
      fseek(bi->fx,0,SEEK_END);
      bi->dSize=ftell(bi->fx);
      bi->aSize=bi->dSize+elen;
      bi->index=malloc(bi->aSize);
      fseek(bi->fx,0,SEEK_SET);
      fread(bi->index,bi->dSize,1,bi->fx);
    }
    *bip=bi;
    return 1;
}

int addBlob(const char *ns, const char * cls, char *id, void *blob, int len)
{
   int keyl=strlen(ns)+strlen(cls)+strlen(id)+strlen(BASE);
   int es,ep,rc;
   char *idxe=alloca(keyl+64);
   BlobIndex *bi;

   rc=getIndex(ns,cls,keyl+64,1,&bi);
   if (rc==0) return 1;

   if (bi->dSize==0) {
      bi->fd=fopen(bi->fnd,"wb");
      if (bi->fd==NULL) { freeBlobIndex(&bi,1); return -1; }
      fwrite(blob,len,1,bi->fd);
      fclose(bi->fd);
      bi->fd=NULL;

      es=sprintf(idxe,"    %zd %s %d %d\r\n",strlen(id),id,len,0);
      ep=sprintf(idxe,"%d",es);
      idxe[ep]=' ';

      memcpy(bi->index,idxe,es);
      bi->dSize=es;
      fwrite(bi->index,bi->dSize,1,bi->fx);
      fclose(bi->fx);
      bi->fx=NULL;
   }

   else {

      if (indxLocate(bi,id)) {
         bi->fd=fopen(bi->fnd,"rb");
         fseek(bi->fd,0,SEEK_END);
         bi->dlen=ftell(bi->fd);
         es=sprintf(idxe,"    %zd %s %d %lu\r\n",strlen(id),id,len,bi->dlen);
         ep=sprintf(idxe,"%d",es);
         idxe[ep]=' ';

         memcpy(bi->index+bi->dSize,idxe,es);
         bi->dSize+=es;
         rebuild(bi,id,blob,len);
      }

      else {
         bi->fd=fopen(bi->fnd,"ab+");
         if (bi->fd==NULL) bi->fd=fopen(bi->fnd,"wb+");
         fseek(bi->fd,0,SEEK_END);
         bi->fpos=ftell(bi->fd);
         fwrite(blob,len,1,bi->fd);
         fclose(bi->fd);
         bi->fd=NULL;

         es=sprintf(idxe,"    %zd %s %d %lu\r\n",strlen(id),id,len,bi->fpos);
         ep=sprintf(idxe,"%d",es);
         idxe[ep]=' ';

         memcpy(bi->index+bi->dSize,idxe,es);
         bi->dSize+=es;
         fseek(bi->fx,0,SEEK_SET);
         fwrite(bi->index,bi->dSize,1,bi->fx);
         fclose(bi->fx);
         bi->fx=NULL;
     }
   }
   freeBlobIndex(&bi,1);
   return 0;
}

int deleteBlob(const char *ns, const char * cls, const char *id)
{
   int keyl=strlen(ns)+strlen(cls)+strlen(id)+strlen(BASE);
   BlobIndex *bi;
   int rc;

   rc=getIndex(ns,cls,keyl+64,0,&bi);

   if (rc) {
      if (indxLocate(bi,id)) {
         bi->fd=fopen(bi->fnd,"rb");
         fseek(bi->fd,0,SEEK_END);
         bi->dlen=ftell(bi->fd);
         rebuild(bi,id,NULL,0);
         freeBlobIndex(&bi,1);
         return 0;
      }
   }
   freeBlobIndex(&bi,1);
   return 1;
}

int existingBlob(const char *ns, const char * cls, const char *id)
{
   int keyl=strlen(ns)+strlen(cls)+strlen(id)+strlen(BASE);
   BlobIndex *bi;
   int rc=0,r=0;

   rc=getIndex(ns,cls,keyl+64,0,&bi);

   if (rc)
      if (indxLocate(bi,id)) r=1;

   freeBlobIndex(&bi,1);
   return r;
}

void *getBlob(const char *ns, const char *cls, const char *id, int *len)
{
   int keyl=strlen(ns)+strlen(cls)+strlen(id)+strlen(BASE);
   BlobIndex *bi;
   char *buf;
   int rc=0;

   // checking for qualifiers should be case-insensitive
   short ignorecase = (strcmp("qualifiers", cls)) ? 0 : 1;

   rc=getIndex(ns,cls,keyl+64,0,&bi);

   if (rc) {
      if (indxLocateCase(bi,id,ignorecase)) {
         bi->fd=fopen(bi->fnd,"rb");
         if (bi->fd==NULL) {
            char *emsg=strerror(errno);
            mlogf(M_ERROR,M_SHOW,"*** Repository error for %s\n",bi->fnd);
            mlogf(M_ERROR,M_SHOW,"Repository error: %s\n",emsg);
            exit(5);
         }
         fseek(bi->fd,bi->bofs,SEEK_SET);
         buf=(char*)malloc(bi->blen+8);
         fread(buf,bi->blen,1,bi->fd);
         if (len) *len=bi->blen;
         buf[bi->blen]=0;
         freeBlobIndex(&bi,1);
         return (void*)buf;
      }
   }
   freeBlobIndex(&bi,1);
   return NULL;
}

int existingNameSpace(const char *ns)
{
   int keyl;
   char *fn,*p;
   char *dir;
   DIR *d;

   dir=getRepDir();
   
   keyl=strlen(ns)+strlen(dir);
   fn=alloca(keyl+64);

   strcpy(fn,dir);
   p=fn+strlen(fn);
   strcat(fn,ns);

   while (*p) { *p=tolower(*p); p++; }

#ifdef __MAIN__
   printf("--- testing %s \n",fn);
#endif
    
   if ((d=opendir(fn))==NULL) {
      perror("opendir");
      return 0;
   }   
   closedir(d);
   return 1;
}


#ifdef __MAIN__

char *o1="first-object";
char *o2="second-object";
char *o3="third-object";
char *o4="fourth-object";
char *ns="root";

int main()
{
   if (existingNameSpace(ns)==0) {
      printf("--- namspace %s does not exist\n",ns);
      exit(1);
   }
   addBlob(ns,"class1",o1,o1,strlen(o1));
   addBlob(ns,"class1",o2,o2,strlen(o2));
   addBlob(ns,"class1",o3,o3,strlen(o3));
   addBlob(ns,"class1",o4,o4,strlen(o4));
   addBlob(ns,"class1",o2,o2,strlen(o2));
   deleteBlob(ns,"class1",o2);
   deleteBlob(ns,"class1",o1);
   deleteBlob(ns,"class1",o4);
   printf("--- %s\n",(char*)getBlob(ns,"class1",o3,NULL));
   deleteBlob(ns,"class1",o3);
}

#endif
