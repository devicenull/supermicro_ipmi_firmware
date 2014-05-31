
/*
 * $Id: sfcbdump.c,v 1.4 2008/01/30 22:44:27 buccella Exp $
 *
 * (C) Copyright IBM Corp. 2006
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
 * sfcBroker repository dumper
 *
*/

#include <fcntl.h>
#include <errno.h>
#include <stddef.h>
#include <getopt.h>
#include "objectImpl.h"

#define BINARY_NAME argv[0]
#define SCHEMA_NAME argv[optind]

typedef enum {REC_VER, 
	      REC_HDR, 
	      REC_QUIT, 
	      REC_FILL,
	      REC_CLS,
	      REC_ANY} DUMP_STATE;

static int checkargs(int argc, char * argv[]);
static int dumpString(const ClObjectHdr *hdr, const ClString *cs, const char * prefix);
static int dumpStringBuffer(const ClObjectHdr *hdr, const char *prefix);
static int dumpArrayBuffer(const ClObjectHdr *hdr, const char *prefix);

static int dumpProperties(const ClClass *cls, const char *prefix);
static int dumpQualifiers(const ClClass *cls, const char *prefix);
static int dumpMethods(const ClClass *cls, const char *prefix);

static int detail_stringbuf = 0;
static int detail_arraybuf = 0;
static int detail_properties = 0;
static int detail_qualifiers = 0;
static int detail_methods = 0;

int main(int argc, char * argv[])
{
  int rc = checkargs(argc,argv);
  if (rc == 0) {
    int fdSchema = open(SCHEMA_NAME,O_RDONLY);
    if (fdSchema < 0) {
      rc = errno;
      fprintf(stderr, "%s: %s",BINARY_NAME,strerror(rc));
    } else {
      DUMP_STATE state=REC_VER;
      DUMP_STATE fillState=REC_QUIT;
      int numRead=0, numFill=0;
      void * fillBuf=NULL;
      ClVersionRecord clv;
      ClObjectHdr coh, *clob;
      ClClass *cls;
      do {
	switch (state) {
	case REC_VER:
	  if (read(fdSchema,&clv,sizeof(ClVersionRecord)) != 
	      sizeof(ClVersionRecord)) {
	    rc = 1;
	    fprintf(stderr, "%s: version record error, short record\n",SCHEMA_NAME);
	    state = REC_QUIT;
	  } else if (clv.size != sizeof(ClVersionRecord)) {
	    rc = 1;
	    fprintf(stderr, "%s: version record size mismatch, is %d expected %zd\n",
		    SCHEMA_NAME, clv.size, sizeof(ClVersionRecord));
	    state = REC_QUIT;
	  } else  {
	    printf("%s: Size of version record: %d, version: %hx\n", 
		   SCHEMA_NAME,clv.size, clv.version);
	    state = REC_HDR;
	  }
	  break;
	case REC_HDR:
	  numRead = read(fdSchema,&coh,sizeof(ClObjectHdr));
	  if (numRead == 0) {
	    state = REC_QUIT; /* regular end */
	  } else if (numRead != sizeof(ClObjectHdr)) {
	    rc = 1;
	    fprintf(stderr, "%s: header record error, short record\n",SCHEMA_NAME);
	    state = REC_QUIT;
	  } else if (coh.size < sizeof(ClObjectHdr)) {
	    rc = 1;
	    fprintf(stderr, "%s: header record size mismatch, is %d expected at least %zd\n",
		    SCHEMA_NAME, coh.size, sizeof(ClObjectHdr));
	    state = REC_QUIT;
	  } else {
	    printf("%s: Header size: %d, type: %hx\n",
		   SCHEMA_NAME,coh.size,coh.type);
	    switch (coh.type) {
	    case HDR_Class:
	      numFill = coh.size - sizeof(ClObjectHdr);
	      fillState = REC_CLS;
	      state = REC_FILL;
	      break;
	    default:
	      fillState = REC_ANY; 
	      state = REC_FILL;
	      break;
	    }
	  }
	  break;
	case REC_CLS:
	  cls = (ClClass*) fillBuf;
	  printf("== Class Record\n");
	  rc = dumpString(&cls->hdr,&cls->name,"   Class name: ");
	  if (rc == 0) {
	    rc = dumpString(&cls->hdr,&cls->parent,"   Parent name: ");
	  }
	  if (rc == 0) {
	    rc = dumpStringBuffer(&cls->hdr,"   ");
	  }	  
	  if (rc == 0) {
	    rc = dumpArrayBuffer(&cls->hdr,"   ");
	  }	  
	  if (rc == 0) {
	    rc = dumpQualifiers(cls,"   ");
	  }	  
	  if (rc == 0) {
	    rc = dumpProperties(cls,"   ");
	  }	  
	  if (rc == 0) {
	    rc = dumpMethods(cls,"   ");
	  }	  
	  if (rc == 0) {
	    state = REC_HDR;
	  } else {
	    state = REC_QUIT;
	  }
	  break;
	case REC_ANY:
	  clob = (ClObjectHdr*) fillBuf;
	  printf("== Unspecified Record Type %d\n",clob->type);
	  if (rc == 0) {
	    rc = dumpStringBuffer(clob,"   ");
	  }	  
	  if (rc == 0) {
	    rc = dumpArrayBuffer(clob,"   ");
	  }	  
	  if (rc == 0) {
	    state = REC_HDR;
	  } else {
	    state = REC_QUIT;
	  }
	  break;
	case REC_FILL:
	  fillBuf = realloc(fillBuf,numFill+sizeof(ClObjectHdr));
	  memcpy(fillBuf,&coh,sizeof(ClObjectHdr));
	  if ((numRead=read(fdSchema, fillBuf+sizeof(ClObjectHdr), numFill)) != numFill) {
	    rc = 1;
	    fprintf(stderr, "%s: structure record short, is %zd expected %zd\n",
		    SCHEMA_NAME, numRead + sizeof(ClObjectHdr),
		    numFill + sizeof(ClObjectHdr));
	    state = REC_QUIT;
	  } else {
	    state = fillState;
	  }
	  break;
	case REC_QUIT:
	  close(fdSchema);
	  break;
	}
      } while(state != REC_QUIT);
      if (fillBuf) {
	free(fillBuf);
      }
    }
  }
  return rc;
}

static int checkargs(int argc, char * argv[])
{
  int c;
  int option_details=0;
  static struct option const long_options[] =
    {
      { "stringbuffer-details", no_argument, 0,'s' },
      { "arraybuffer-details",  no_argument, 0,'a' },
      { "property-details",  no_argument, 0,'p' },
      { "qualifier-details",  no_argument, 0,'q' },
      { "method-details",  no_argument, 0,'m' },
      { "help",  no_argument, 0,'h' },
      { 0, 0, 0, 0 }
    };
  
   while ((c = getopt_long(argc, argv, "hsapqm", long_options, 0)) != -1) {
     switch(c)  {
     case 's':
       detail_stringbuf = 1;
       break;
     case 'a':
       detail_arraybuf = 1;
       break;
     case 'p':
       detail_properties = 1;
       break;
     case 'q':
       detail_qualifiers = 1;
       break;
     case 'm':
       detail_methods = 1;
       break;
     case 'h':
       option_details = 1;
       break;
     default:
       return 1;
       break;
     }
   }
   
  if (argc - optind != 1) {
    fprintf(stderr,"Usage: %s [-hsapqm] schemafile\n",BINARY_NAME);
    if (option_details) {
      fprintf(stderr,"\n\tAllowed options:\n");
      fprintf(stderr,"\t-s, --stringbuffer-details\n");
      fprintf(stderr,"\t-a, --arraybuffer-details\n");
      fprintf(stderr,"\t-p, --property-details\n");
      fprintf(stderr,"\t-q, --qualifier-details\n");
      fprintf(stderr,"\t-m, --method-details\n");
      fprintf(stderr,"\t-h, --help\n");
    }
    return 2;
  } else {
    return 0;
  }
}

static int dumpString(const ClObjectHdr *hdr, const ClString *cs, const char * prefix)
{
  /* check offset range */
  ClStrBuf *sb = NULL;
  int      *index = NULL;
  int rc = 0;
  if (abs(hdr->strBufOffset) <= hdr->size) {
    sb = (void*)hdr + abs(hdr->strBufOffset);
    index = (void*)hdr + abs(sb -> indexOffset);
    if (cs->id == 0) {
      printf("%s NULL\n",prefix);
    } else if (cs->id > 0 && cs->id <= sb->iMax) {
      if (index[cs->id-1] >= 0 && 
	  (index[cs->id-1] + abs(hdr->strBufOffset)) < hdr->size ) {
	printf("%s \"%s\"\n",prefix,
	       (char*)sb + offsetof(ClStrBuf,buf) + index[cs->id-1]);
      } else {
	fprintf(stderr,"%s string index value %d out of range (0..%d)\n",
		prefix,index[cs->id-1],hdr->size - abs(hdr->strBufOffset));
        rc = 2;
      }
    } else {
      fprintf(stderr,"%s string index key %ld out of range (0..%d) \n",
	      prefix,cs->id,sb->iMax);
        rc = 2;
    }
  } else {
    fprintf(stderr,"%s invalid string buffer offset %d, must be < %d\n",
	    prefix,abs(hdr->strBufOffset),hdr->size);
        rc = 2;
  }
  return rc;
}

static int dumpStringBuffer(const ClObjectHdr *hdr, const char *prefix)
{
  /* check offset range */
  ClStrBuf *sb = NULL;
  int      *index = NULL;
  int rc = 0;
  int i;
  if (hdr->strBufOffset == 0) {
    printf("%s * Empty string buffer\n",prefix);
  } else if (abs(hdr->strBufOffset) <= hdr->size) {
    sb = (void*)hdr + abs(hdr->strBufOffset);
    index = (void*)hdr + abs(sb -> indexOffset);
    if ((sb->iUsed * sizeof(int) + abs(hdr->strBufOffset)) <= hdr->size) {
      printf("%s * String buffer contains %d of maximum %d strings\n",
	     prefix, sb->iUsed, sb->iMax);
      if (detail_stringbuf) {
	for (i=0; i < sb->iUsed; i++) {
	  printf("%s sb[%3d]=\"%s\"\n",prefix,i,
		 sb->buf + index[i]);
	}
      }
    } else {
      fprintf(stderr,"%s to many stringbuffer index entries %d\n",
	      prefix, sb->iUsed);
      rc = 2;
    }
  } else {
    fprintf(stderr,"%s invalid string buffer offset %d, must be < %d\n",
	    prefix,abs(hdr->strBufOffset),hdr->size);
    rc = 2;
  }
  return rc;
}

static int dumpArrayBuffer(const ClObjectHdr *hdr, const char *prefix)
{
  /* check offset range */
  ClArrayBuf *ab = NULL;
  int      *index = NULL;
  int rc = 0;
  int i;
  if (hdr->arrayBufOffset == 0) {
    printf("%s * Empty array buffer\n",prefix);
  } else if (abs(hdr->arrayBufOffset) <= hdr->size) {
    ab = (void*)hdr + abs(hdr->arrayBufOffset);
    index = (void*)hdr + abs(ab -> indexOffset);
    if ((ab->iUsed * sizeof(int) + abs(hdr->arrayBufOffset)) <= hdr->size) {
      printf("%s * Array buffer contains %d of maximum %d values\n",
	     prefix, ab->iUsed, ab->iMax);
      if (detail_arraybuf) {
	for (i=0; i < ab->iUsed; i++) {
	  printf("%s ab[%3d]=(%hx,%hx,%016llx)\n",prefix,i,
		 ab->buf[index[i]].type, ab->buf[index[i]].state,
		 ab->buf[index[i]].value.uint64);
	}
      }
    } else {
      fprintf(stderr,"%s to many array buffer index entries %d\n",
	      prefix, ab->iUsed);
      rc = 2;
    }
  } else {
    fprintf(stderr,"%s invalid array buffer offset %d, must be < %zd\n",
	    prefix,abs(hdr->arrayBufOffset),hdr->size + sizeof(ClArrayBuf));
    rc = 2;
  }
  return rc;
}

static int dumpProperties(const ClClass *cls, const char *prefix)
{
  /* check offset range */
  ClProperty *cp = NULL;
  int rc = 0;
  int i;
  char * prefixbuf = NULL;
  if (cls->properties.sectionOffset == 0) {
    printf("%s * Empty property section\n",prefix);
  } else if (abs(cls->properties.sectionOffset) <= cls->hdr.size) {
    cp = (void*)cls + abs(cls->properties.sectionOffset);
    if ((cls->properties.used * sizeof(ClProperty) + abs(cls->properties.sectionOffset)) 
	<= cls->hdr.size) {
      printf("%s * Property section contains %d properties of maximum %d\n",
	     prefix, cls->properties.used, cls->properties.max);
      if (detail_properties) {
	prefixbuf = malloc(strlen(prefix) + strlen(" -- Property[999]:") + 2);
	for (i=0; i < cls->properties.used; i++) {
	  sprintf(prefixbuf,"%s -- Property[%3d]:",prefix,i);
	  dumpString(&cls->hdr,&cp[i].id,prefixbuf);
	}
      }
    } else {
      fprintf(stderr,"%s to many property entries %d\n",
	      prefix, cls->properties.used);
      rc = 2;
    }
  } else {
    fprintf(stderr,"%s invalid property section offset %d, must be < %d\n",
	    prefix,abs(cls->properties.sectionOffset),cls->hdr.size);
    rc = 2;
  }
  return rc;
}

static int dumpQualifiers(const ClClass *cls, const char *prefix)
{
  /* check offset range */
  ClQualifier *cq = NULL;
  int rc = 0;
  int i;
  char * prefixbuf = NULL;
  if (cls->qualifiers.sectionOffset == 0) {
    printf("%s * Empty qualifier section\n",prefix);
  } else if (abs(cls->qualifiers.sectionOffset) <= cls->hdr.size) {
    cq = (void*)cls + abs(cls->qualifiers.sectionOffset);
    if ((cls->qualifiers.used * sizeof(ClQualifier) + abs(cls->qualifiers.sectionOffset)) 
	<= cls->hdr.size) {
      printf("%s * Qualifier section contains %d qualifiers of maximum %d\n",
	     prefix, cls->qualifiers.used, cls->qualifiers.max);
      if (detail_qualifiers) {
	prefixbuf = malloc(strlen(prefix) + strlen(" -- Qualifier[999]:") + 2);
	for (i=0; i < cls->qualifiers.used; i++) {
	  sprintf(prefixbuf,"%s -- Qualifier[%3d]:",prefix,i);
	  dumpString(&cls->hdr,&cq[i].id,prefixbuf);
	}
	free(prefixbuf);
      }
    } else {
      fprintf(stderr,"%s to many qualifier entries %d\n",
	      prefix, cls->qualifiers.used);
      rc = 2;
    }
  } else {
    fprintf(stderr,"%s invalid qualifier section offset %d, must be < %d\n",
	    prefix,abs(cls->qualifiers.sectionOffset),cls->hdr.size);
    rc = 2;
  }
  return rc;
}

static int dumpMethods(const ClClass *cls, const char *prefix)
{
  /* check offset range */
  ClMethod *cm = NULL;
  int rc = 0;
  int i;
  char * prefixbuf = NULL;
  if (cls->methods.sectionOffset == 0) {
    printf("%s * Empty method section\n",prefix);
  } else if (abs(cls->methods.sectionOffset) <= cls->hdr.size) {
    cm = (void*)cls + abs(cls->methods.sectionOffset);
    if ((cls->methods.used * sizeof(ClMethod) + abs(cls->methods.sectionOffset)) 
	<= cls->hdr.size) {
      printf("%s * Method section contains %d methods of maximum %d\n",
	     prefix, cls->methods.used, cls->methods.max);
      if (detail_methods) {
	prefixbuf = malloc(strlen(prefix) + strlen(" -- Method[999]:") + 2);
	for (i=0; i < cls->methods.used; i++) {
	  sprintf(prefixbuf,"%s -- Method[%3d]:",prefix,i);
	  dumpString(&cls->hdr,&cm[i].id,prefixbuf);
	}
      }
    } else {
      fprintf(stderr,"%s to many method entries %d\n",
	      prefix, cls->methods.used);
      rc = 2;
    }
  } else {
    fprintf(stderr,"%s invalid method section offset %d, must be < %d\n",
	    prefix,abs(cls->methods.sectionOffset),cls->hdr.size);
    rc = 2;
  }
  return rc;
}

