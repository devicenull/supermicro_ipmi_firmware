/**
 * $Id: mofparse.c,v 1.7 2006/10/27 13:14:21 sschuetz Exp $
 *
 * (C) Copyright IBM Corp. 2004
 * 
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:       Viktor Mihajlovski <mihajlov@de.ibm.com>
 * Contributors: 
 *
 * Description: MOF Compiler Driver
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <hash.h>
#include <symtab.h>
#include <mofdecl.h>
#include <sys/stat.h>
#include "mofc.h"
#include "backend.h"
#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#error "config.h missing -- did you run ./configure ?"
#endif


static int opt_verbose = 0; 
static int opt_version = 0;
static int opt_help = 0;
static char inclpath[3000] = {0};
static char outfile[300] = "mofc.out";
static char outdir[3000] = {0};
static char prefix[3000] = {0};
static char backendopt[300] = {0};
static char namespace[600] = {0};
static size_t backendopt_used = 0;
static char * inclfile = NULL;
static char * valid_options = "b:o:I:i:d:n:hvV";

extern class_chain  * cls_chain_current;
extern class_chain  * inst_chain_current;
extern qual_chain   * qual_chain_current;
extern backend_type * backend_ptr;

static int parse_options(int argc, char * argv[])
{
  int opt;
  while ((opt=getopt(argc,argv,valid_options)) != -1) {
    switch (opt) {
    case 'b': 
      backendopt_used += strlen(optarg);
      if (backendopt_used < sizeof(backendopt) - 1 ) {
	strcat(backendopt," ");
	strcat(backendopt,optarg);
      }
      break;
    case 'o': 
      strncpy(outfile,optarg,sizeof(outfile));
      break;
    case 'n': 
      strncpy(namespace,optarg,sizeof(outfile));
      break;      
    case 'd': 
      strncpy(outdir,optarg,sizeof(outdir));
      mkdir(outdir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
      break;      
    case 'v': 
      opt_verbose = 1;
      break;
    case 'V': 
      opt_version = 1;
      break;
    case 'h':
      opt_help = 1;
      break;
    case 'I':
      if (strlen(inclpath) > 0) {
	strcat(inclpath,":");
      }
      strcat(inclpath,optarg);
      break;
    case 'i':
      inclfile = strdup(optarg);
      break;
    default:
      return -1;
    }
  }
  return optind;
}

static void usage(const char * name)
{
  fprintf(stderr,"usage: %s [-hvV] [-I includepath ...] [-i extrafile] [-o outfile] \
[-b backendopts] [-d outputdirectory] [-n namespace] filename ... \n",name);
}

static void version()
{
  printf("MOF Compiler Version %s with %s\n",VERSION,BACKEND_INFO);
}

static void help(const char * name)
{
  usage(name);
  version(name);
  printf(" Allowed options are\n");
  printf("  -h             display this message\n");
  printf("  -v             print some extra information\n");
  printf("  -V             print version information\n");
  printf("  -I includepath directory to search for include files\n");
  printf("  -i extrafile   file to load before parsing\n");
  printf("  -o outfile     class schema output file to generate (default: mofc.out)\n");
  printf("  -b backendopts backend options, see backend documentation\n");
  printf("  -d output directory (recommended when parsing qualifiers and instances, as this creates more than one file)\n");
  printf("  -n target namespace, only needed for instances and qualifiers\n");
}

static int error_occured = 0;

int main(int argc, char * argv[])
{
  int argidx,i;
  if ((argidx=parse_options(argc,argv)) == -1) {
    usage(argv[0]);
    return 1;
  }
  if (opt_version) {
    version();
    return 0;
  }
  if (opt_help) {
    help(argv[0]);
    return 0;
  }
  if (argidx > argc-1) {
    usage(argv[0]);
    return 1;
  }
  if (opt_help) {
    help(argv[0]);
    return 0;
  }
  if (opt_verbose) {
    version();
    printf("Parsing ");
    for (i=argidx; i<argc; i++) {
      printf("%s ",argv[i]);
    }
    printf("\n  MOF include path=%s\n",inclpath);
    if (inclfile) {
      printf("  extra MOF file=%s\n",inclfile);
    }
  }
  if(*outdir) {
  	strcat(prefix, outdir);
  	strcat(prefix, "/");
  	
  	//append "/", otherwise the filerep won't work if it's not there
  	strcat(outdir, "/");
  }
  if(*namespace) {
  	strcat(prefix, namespace);
  	strcat(prefix, "/");
  }
  if(*prefix) {
  	int offset = -1;
  	char localpath[600];
  	char * pos;
	while((pos = strchr(prefix + offset + 1, '/'))) {
		offset = pos - prefix;
		memcpy(localpath, prefix, offset);
		localpath[offset] = 0;
		mkdir(localpath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	}
	mkdir(prefix, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	
	strcat(prefix, outfile);
  }
  
  if (init_scanner(argv+argidx,argc-argidx,inclpath,inclfile,opt_verbose) != 0 ) {
    return 2;
  }
  if ( ( yyparse() || error_occured )) {
    if ( opt_verbose ) { 
      fprintf( stderr, "error has occured in %s\n", argv[argidx] );
    }
    stop_scanner();
    return 3;
  }
  if (backend_ptr(cls_chain_current, inst_chain_current, qual_chain_current, prefix, outdir, namespace,  
		  opt_verbose ? BACKEND_VERBOSE : BACKEND_DEFAULT,
		  backendopt) ) {
    fprintf( stderr, "backend error has occured writing %s\n", outfile );
  }
  stop_scanner();
  return 0;
}

void yyerror( char * errstr )
{
  error_occured = 1;
  fprintf( stderr
         , "error in %s line %d near %s: %s\n"
         , file_name, line_number, 
	   yylval.lval_id , 
	   errstr);
}

int yywrap(void)
{
  return 1;
}
