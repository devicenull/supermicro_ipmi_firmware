

#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define INC "#pragma include "

char *path;
int err=0;

FILE *getFILE(char *str)
{
   FILE *f=NULL;
   char *p,*n,ch,fn[1024];
   
   if ((p=strchr(str,'"'))) {
      p++;
      if ((n=strchr(p,'"'))) {
         ch=*n;
         *n=0;
         strcpy(fn,path);
         strcat(fn,p);
         f=fopen(fn,"r");
         *n=ch;
      }      
   }
   
   return f;
}

int incOK(char *str, char **s, char **e, char **ifn, FILE **f)
{
   char ch,fn[1024];
   *f=NULL;
   *ifn=NULL;
   
   while (*str && *str<=' ') str++;
   if (*str && strncmp(str,"#pragma ",8)==0) {
      *s=str;
      str+=8;
      while (*str && *str<=' ') str++;
      if (*str && strncmp(str,"include",7)==0) {
         str+=7;
         while (*str && *str<=' ') str++;
         if (*str=='(') {
            str+=1;
            while (*str && *str<=' ') str++;
            if (*str=='"') {
               str++;
               if ((*e=strchr(str,'"'))) {
                  ch=**e;
                  **e=0;
                  strcpy(fn,path);
                  strcat(fn,str);
                  *ifn=strdup(fn);
                  **e=ch;
                  (*e)++;
                  while (**e && **e<=' ') (*e)++;
                  if (**e==')') 
                     *f=fopen(fn,"r");
                  return 1;   
               }
            }
         }
      }
   }
   return 0;
}

void processFile(char *fn, FILE *in, FILE *out)
{
   char *s,*e,rec[10000],*ifn=NULL;
   FILE *incFile;
   int comment=0;
   int nl=0;
   
   while (fgets(rec, sizeof(rec), in)) {
      nl++;
      if (comment==0 && incOK(rec,&s,&e,&ifn,&incFile)) {
         if (incFile) {
            fprintf(out,"// resolved:  >>>> %s",s);
            processFile(ifn,incFile,out);
            fprintf(out,"// back from: >>>> %s",s);
            fclose(incFile);
         }
         else {
            fprintf(stderr,"%s:%d File not found: %s\n",fn,nl,s);
         }
      }
      else {
         if (comment) {
            s=rec;
            if ((e=strstr(s,"*/"))) {
               strcpy(s,e+2);
               comment=0;
            }
            else continue;
         }
         while ((s=strstr(rec,"/*"))) {
            if ((e=strstr(s+2,"*/"))) {
               strcpy(s,e+2);
            }
            else {
               *s=0;
               comment=1;
               break;
            }
         }
         fprintf(out,"%s",rec);
      }
   }
   free(fn);
}

int main(int argc, char *argv[])
{
   char *fn=NULL,*p=NULL,ch;
   FILE *in;
   
   if (argc!=2) {
      fprintf(stderr,"usage: %s filename\n",argv[0]);
      return 2;
   }
   
   fn=strdup(argv[1]);
   
   if ((p=strrchr(fn,'/'))) {
      ch=*(p+1);
      *(p+1)=0;
      path=strdup(fn);
      *(p+1)=ch;
   }
   else path="";
   
   in=fopen(fn,"r");
   
   if (in) processFile(strdup(fn),in,stdout);
   else {
      fprintf(stderr,"file %s not found\n",fn);
      return 1;
   }
   
   return err;
}
