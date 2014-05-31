
/*
 * $Id: sfcbdumpP32onI32.c,v 1.7 2007/09/14 12:41:34 mihajlov Exp $
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
#include <byteswap.h>

#define CLP32
#define SETCLPFX CLP32_

#define CLP32_CMPIType CMPIType
#define CLP32_CMPIValueState CMPIValueState
#define CLP32_CMPIValue CMPIValue

typedef struct _CLP32_CMPIData {
   CLP32_CMPIType type;
   CLP32_CMPIValueState state;
   int fillP32;
   CLP32_CMPIValue value;
} CLP32_CMPIData;

#include "objectImpl.h"

typedef struct {
   unsigned short iUsed,iMax;
   int indexOffset;
   int *indexPtr;
   unsigned int bUsed, bMax;
   int fillP32;
   CLP32_CMPIData buf[1];
} CLP32_ClArrayBuf;

typedef struct {
   CLP32_ClString id;
   int fillP32;
   CLP32_CMPIData data;
} CLP32_ClQualifier;


typedef struct {
   CLP32_CMPIData data;
   CLP32_ClString id;
   CLP32_ClString refName;
   unsigned short flags;
   #ifndef SETCLPFX
    #define ClProperty_EmbeddedObjectAsString 1
    #define ClProperty_Deleted 2
   #endif 
   unsigned char quals;
   #ifndef SETCLPFX
    #define ClProperty_Q_Key 1
    #define ClProperty_Q_Propagated 2
    #define ClProperty_Q_EmbeddedObject 8
   #endif 
   unsigned char originId;
   CLP32_ClSection qualifiers;
   long            padP32;
} CLP32_ClProperty;

#define BINARY_NAME argv[0]
#define SCHEMA_NAME argv[optind]

typedef enum {REC_VER, 
	      REC_HDR, 
	      REC_QUIT, 
	      REC_FILL,
	      REC_CLS,
	      REC_ANY} DUMP_STATE;

static int checkargs(int argc, char * argv[]);
static int dumpString(const CLP32_ClObjectHdr *hdr, const CLP32_ClString *cs, const char * prefix);
static int dumpStringBuffer(const CLP32_ClObjectHdr *hdr, const char *prefix);
static int dumpArrayBuffer(const CLP32_ClObjectHdr *hdr, const char *prefix);

static int dumpProperties(const CLP32_ClClass *cls, const char *prefix);
static int dumpQualifiers(const CLP32_ClClass *cls, const char *prefix);
static int dumpMethods(const CLP32_ClClass *cls, const char *prefix);

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
      CLP32_ClVersionRecord clv;
      CLP32_ClObjectHdr coh, *clob;
      CLP32_ClClass *cls;
      do {
	switch (state) {
	case REC_VER:
	  if (read(fdSchema,&clv,sizeof(CLP32_ClVersionRecord)) != 
	      sizeof(CLP32_ClVersionRecord)) {
	    rc = 1;
	    fprintf(stderr, "%s: version record error, short record\n",SCHEMA_NAME);
	    state = REC_QUIT;
	  } else if (bswap_32(clv.size) != sizeof(CLP32_ClVersionRecord)) {
	    rc = 1;
	    fprintf(stderr, "%s: version record size mismatch, is %d expected %d\n",
		    SCHEMA_NAME, bswap_32(clv.size), sizeof(CLP32_ClVersionRecord));
	    state = REC_QUIT;
	  } else  {
	    printf("%s: Size of version record: %d, version: %hx\n", 
		   SCHEMA_NAME,bswap_32(clv.size), bswap_16(clv.version));
	    state = REC_HDR;
	  }
	  break;
	case REC_HDR:
	  numRead = read(fdSchema,&coh,sizeof(CLP32_ClObjectHdr));
	  if (numRead == 0) {
	    state = REC_QUIT; /* regular end */
	  } else if (numRead != sizeof(CLP32_ClObjectHdr)) {
	    rc = 1;
	    fprintf(stderr, "%s: header record error, short record\n",SCHEMA_NAME);
	    state = REC_QUIT;
	  } else if (bswap_32(coh.size) < sizeof(CLP32_ClObjectHdr)) {
	    rc = 1;
	    fprintf(stderr, "%s: header record size mismatch, is %d expected at least %d\n",
		    SCHEMA_NAME, bswap_32(coh.size), sizeof(CLP32_ClObjectHdr));
	    state = REC_QUIT;
	  } else {
	    printf("%s: Header size: %d, type: %hx\n",
		   SCHEMA_NAME,bswap_32(coh.size),bswap_16(coh.type));
	    switch (bswap_16(coh.type)) {
	    case HDR_Class:
	      numFill = bswap_32(coh.size) - sizeof(CLP32_ClObjectHdr);
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
	  cls = (CLP32_ClClass*) fillBuf;
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
	  clob = (CLP32_ClObjectHdr*) fillBuf;
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
	  fillBuf = realloc(fillBuf,numFill+sizeof(CLP32_ClObjectHdr));
	  memcpy(fillBuf,&coh,sizeof(CLP32_ClObjectHdr));
	  if ((numRead=read(fdSchema, fillBuf+sizeof(CLP32_ClObjectHdr), numFill)) != numFill) {
	    rc = 1;
	    fprintf(stderr, "%s: structure record short, is %d expected %d\n",
		    SCHEMA_NAME, numRead + sizeof(CLP32_ClObjectHdr),
		    numFill + sizeof(CLP32_ClObjectHdr));
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

static int dumpString(const CLP32_ClObjectHdr *hdr, const CLP32_ClString *cs, const char * prefix)
{
  /* check offset range */
  CLP32_ClStrBuf *sb = NULL;
  int      *index = NULL;
  int rc = 0;
  if (abs(bswap_32(hdr->strBufOffset)) <= bswap_32(hdr->size)) {
    sb = (void*)hdr + abs( bswap_32(hdr->strBufOffset));
    index = (void*)hdr + abs(bswap_32(sb -> indexOffset));
    if (bswap_32(cs->id) == 0) {
      printf("%s NULL\n",prefix);
    } else if (bswap_32(cs->id) > 0 &&bswap_32( cs->id) <= bswap_32(sb->iMax)) {
      if (bswap_32(index[bswap_32(cs->id)-1]) >= 0 && 
	  (bswap_32(index[bswap_32(cs->id)-1]) + abs(bswap_32(hdr->strBufOffset))) < bswap_32(hdr->size) ) {
	printf("%s \"%s\"\n",prefix,
	       (char*)sb + offsetof(CLP32_ClStrBuf,buf) + bswap_32(index[bswap_32(cs->id)-1]));
      } else {
	fprintf(stderr,"%s string index value %d out of range (0..%d)\n",
		prefix,bswap_32(index[bswap_32(cs->id)-1]),bswap_32(hdr->size) - abs(bswap_32(hdr->strBufOffset)));
        rc = 2;
      }
    } else {
      fprintf(stderr,"%s string index key %d out of range (0..%d) \n",
	      prefix,bswap_32(cs->id),bswap_16(sb->iMax));
        rc = 2;
    }
  } else {
    fprintf(stderr,"%s invalid string buffer offset %d, must be < %d\n",
	    prefix,abs(bswap_32(hdr->strBufOffset)),bswap_32(hdr->size));
        rc = 2;
  }
  return rc;
}

static int dumpStringBuffer(const CLP32_ClObjectHdr *hdr, const char *prefix)
{
  /* check offset range */
  CLP32_ClStrBuf *sb = NULL;
  int      *index = NULL;
  int rc = 0;
  int i;
  if (bswap_32(hdr->strBufOffset) == 0) {
    printf("%s * Empty string buffer\n",prefix);
  } else if (abs(bswap_32(hdr->strBufOffset)) <= bswap_32(hdr->size)) {
    sb = (void*)hdr + abs(bswap_32(hdr->strBufOffset));
    index = (void*)hdr + abs(bswap_32(sb -> indexOffset));
    if ((bswap_16(sb->iUsed) * sizeof(int) + abs(bswap_32(hdr->strBufOffset))) <= bswap_32(hdr->size)) {
      printf("%s * String buffer contains %d of maximum %d strings\n",
	     prefix, bswap_16(sb->iUsed), bswap_16(sb->iMax));
      if (detail_stringbuf) {
	for (i=0; i < bswap_16(sb->iUsed); i++) {
	  printf("%s sb[%3d]=\"%s\"\n",prefix,i,
		 sb->buf + bswap_32(index[i]));
	}
      }
    } else {
      fprintf(stderr,"%s to many stringbuffer index entries %d\n",
	      prefix, bswap_16(sb->iUsed));
      rc = 2;
    }
  } else {
    fprintf(stderr,"%s invalid string buffer offset %d, must be < %d\n",
	    prefix,abs(bswap_32(hdr->strBufOffset)),bswap_32(hdr->size));
    rc = 2;
  }
  return rc;
}

static int dumpArrayBuffer(const CLP32_ClObjectHdr *hdr, const char *prefix)
{
  /* check offset range */
  CLP32_ClArrayBuf *ab = NULL;
  int      *index = NULL;
  int rc = 0;
  int i;
  if (bswap_32(hdr->arrayBufOffset) == 0) {
    printf("%s * Empty array buffer\n",prefix);
  } else if (abs(bswap_32(hdr->arrayBufOffset)) <= bswap_32(hdr->size)) {
    ab = (void*)hdr + abs(bswap_32(hdr->arrayBufOffset));
    index = (void*)hdr + abs(bswap_32(ab -> indexOffset));
    if ((bswap_16(ab->iUsed) * sizeof(int) + abs(bswap_32(hdr->arrayBufOffset))) <= bswap_32(hdr->size)) {
      printf("%s * Array buffer contains %d of maximum %d values\n",
	     prefix, bswap_16(ab->iUsed), bswap_16(ab->iMax)); 
      if (detail_arraybuf) {
	for (i=0; i < bswap_16(ab->iUsed); i++) {
	  printf("%s ab[%3d]=(%hx,%hx,%016llx)\n",prefix,i,
		 bswap_16(ab->buf[bswap_32(index[i])].type), bswap_16(ab->buf[bswap_32(index[i])].state),
		 bswap_64(ab->buf[bswap_32(index[i])].value.uint64));
	}
      }
    } else {
      fprintf(stderr,"%s to many array buffer index entries %d\n",
	      prefix, bswap_16(ab->iUsed));
      rc = 2;
    }
  } else {
    fprintf(stderr,"%s invalid array buffer offset %d, must be < %d\n",
	    prefix,abs(bswap_32(hdr->arrayBufOffset)),bswap_32(hdr->size) + sizeof(CLP32_ClArrayBuf));
    rc = 2;
  }
  return rc;
}

static int dumpProperties(const CLP32_ClClass *cls, const char *prefix)
{
  /* check offset range */
  CLP32_ClProperty *cp = NULL;
  int rc = 0;
  int i;
  char * prefixbuf = NULL;
  if (bswap_32(cls->properties.sectionOffset) == 0) {
    printf("%s * Empty property section\n",prefix);
  } else if (abs(bswap_32(cls->properties.sectionOffset)) <= bswap_32(cls->hdr.size)) {
    cp = (void*)cls + abs(bswap_32(cls->properties.sectionOffset));
    if ((bswap_16(cls->properties.used) * sizeof(CLP32_ClProperty) + abs(bswap_32(cls->properties.sectionOffset))) 
	<= bswap_32(cls->hdr.size)) {
      printf("%s * Property section contains %d properties of maximum %d\n",
	     prefix, bswap_16(cls->properties.used), bswap_16(cls->properties.max));
      if (detail_properties) {
	prefixbuf = malloc(strlen(prefix) + strlen(" -- Property[999]:") + 2);
	for (i=0; i < bswap_16(cls->properties.used); i++) {
	  sprintf(prefixbuf,"%s -- Property[%3d]:",prefix,i);
	  dumpString(&cls->hdr,&cp[i].id,prefixbuf);
	}
      }
    } else {
      fprintf(stderr,"%s to many property entries %d\n",
	      prefix, bswap_16(cls->properties.used));
      rc = 2;
    }
  } else {
    fprintf(stderr,"%s invalid property section offset %d, must be < %d\n",
	    prefix,abs(bswap_32(cls->properties.sectionOffset)),bswap_32(cls->hdr.size));
    rc = 2;
  }
  return rc;
}

static int dumpQualifiers(const CLP32_ClClass *cls, const char *prefix)
{
  /* check offset range */
  CLP32_ClQualifier *cq = NULL;
  int rc = 0;
  int i;
  char * prefixbuf = NULL;
  if (bswap_32(cls->qualifiers.sectionOffset) == 0) {
    printf("%s * Empty qualifier section\n",prefix);
  } else if (abs(bswap_32(cls->qualifiers.sectionOffset)) <= bswap_32(cls->hdr.size)) {
    cq = (void*)cls + abs(bswap_32(cls->qualifiers.sectionOffset));
    if ((bswap_16(cls->qualifiers.used) * sizeof(CLP32_ClQualifier) + abs(bswap_32(cls->qualifiers.sectionOffset))) 
	<= bswap_32(cls->hdr.size)) {
      printf("%s * Qualifier section contains %d qualifiers of maximum %d\n",
	     prefix, bswap_16(cls->qualifiers.used), bswap_16(cls->qualifiers.max));
      if (detail_qualifiers) {
	prefixbuf = malloc(strlen(prefix) + strlen(" -- Qualifier[999]:") + 2);
	for (i=0; i < bswap_16(cls->qualifiers.used); i++) {
	  sprintf(prefixbuf,"%s -- Qualifier[%3d]:",prefix,i);
	  dumpString(&cls->hdr,&cq[i].id,prefixbuf);
	}
      }
    } else {
      fprintf(stderr,"%s to many qualifier entries %d\n",
	      prefix, bswap_16(cls->qualifiers.used));
      rc = 2;
    }
  } else {
    fprintf(stderr,"%s invalid qualifier section offset %d, must be < %d\n",
	    prefix,abs(bswap_32(cls->qualifiers.sectionOffset)),bswap_32(cls->hdr.size));
    rc = 2;
  }
  return rc;
}

static int dumpMethods(const CLP32_ClClass *cls, const char *prefix)
{
  /* check offset range */
  CLP32_ClMethod *cm = NULL;
  int rc = 0;
  int i;
  char * prefixbuf = NULL;
  if (bswap_32(cls->methods.sectionOffset) == 0) {
    printf("%s * Empty method section\n",prefix);
  } else if (abs(bswap_32(cls->methods.sectionOffset)) <= bswap_32(cls->hdr.size)) {
    cm = (void*)cls + abs(bswap_32(cls->methods.sectionOffset));
    if ((bswap_16(cls->methods.used) * sizeof(CLP32_ClMethod) + abs(bswap_32(cls->methods.sectionOffset))) 
	<= bswap_32(cls->hdr.size)) {
      printf("%s * Method section contains %d methods of maximum %d\n",
	     prefix, bswap_16(cls->methods.used), bswap_16(cls->methods.max));
      if (detail_methods) {
	prefixbuf = malloc(strlen(prefix) + strlen(" -- Method[999]:") + 2);
	for (i=0; i < bswap_16(cls->methods.used); i++) {
	  sprintf(prefixbuf,"%s -- Method[%3d]:",prefix,i);
	  dumpString(&cls->hdr,&cm[i].id,prefixbuf);
	}
      }
    } else {
      fprintf(stderr,"%s to many method entries %d\n",
	      prefix, bswap_16(cls->methods.used));
      rc = 2;
    }
  } else {
    fprintf(stderr,"%s invalid method section offset %d, must be < %d\n",
	    prefix,abs(bswap_32(cls->methods.sectionOffset)),bswap_32(cls->hdr.size));
    rc = 2;
  }
  return rc;
}

