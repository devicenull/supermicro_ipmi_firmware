/**
 * $Id: mofs.l,v 1.6 2007/06/22 10:34:44 sschuetz Exp $
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
 * Description: Lexical Analysis for DMTF MOF Syntax
 *
 */

%{

# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include <hash.h>


# if defined SCANDEBUG    /* for debugging */
#   undef YY_DECL
    typedef YY_CHAR * TOKENTYPE;
    typedef YY_CHAR * SYMTOKENTYPE;
#   define YYLEX yylex
#   define YY_DECL TOKENTYPE YYLEX (void)
#   define token(a) (#a)
#   define tokensym(a) (#a)
#   define make_token(a) (#a)
#   define YYDISPLAY(t,x) printf( "%s is %" ## #t ## "\n", #x, x )
  /* debug main program */
    extern YY_DECL;
    char * upstrdup( const char *, int );
    extern int init_scanner(char*[], int, const char*, const char *, int);
    extern void stop_scanner(void);
    extern FILE * try_open_file(const char *);
    int main()      /* just to see what goes on */
    {

        TOKENTYPE s;

        init_scanner(&"xxx",1,NULL,NULL,1);

        while( ( s = YYLEX() ) != NULL )
            printf( "Recognised a %s with value >%s<\n", s, yytext );

        stop_scanner();
        return 0;
    }
# else
#   include "symtab.h"
#   include "mofdecl.h"
    typedef int TOKENTYPE;
    typedef symtab_entry * SYMTOKENTYPE;
#   define token(a) (a)
#   define tokensym(a) (a->sym_type == SYM_TOKEN ? a->sym_union.sym_token : Identifier)
#   define YYDISPLAY(t,x)
#   include "mofc.h"    /* get token id's */
class_chain * cls_chain_current;
class_chain * inst_chain_current;
qual_chain * qual_chain_current;
# endif

int line_number = 1;
char * file_name = "stdin"; /* default */
SYMTOKENTYPE temp_token;
hashentry * global_table;
hashentry * current_symtab;
hashentry * current_qualtab;
int verbosescan = 0;
char * upyytext;
char * includepath = NULL;
char * extrainclude = NULL;

typedef struct file_stack_struct {
  char                     * file_name;
  FILE                     * file_handle;
  int                        line_number;
  YY_BUFFER_STATE            yybuffer;
  struct file_stack_struct * next_file;
#ifndef SCANDEBUG
  class_chain              * file_cls;
#endif
} file_stack_entry;
file_stack_entry *current_file;
void push_file(FILE * file_handle, char * filename, int keep_symtab);
void pop_file();

%}

space                   [ \t]
letter                  [A-Z]
special                 _
digit                   [0-9]
hexdigit                [0-9A-F]
prefix                  #|$|{letter}|{special}
trailer                 {prefix}|{digit}
identifier              {prefix}{trailer}*
hexprefix               "0X"
int                     [+-]?{hexprefix}?{hexdigit}*B?
float                   [+-]?{digit}*\.{digit}+E?[+-]?{digit}*
string                  \"([^\\"]|\\.)*\"
charstring              \'([^\\']|\\.)*\'
punctuation             [;,.:=(){}]

%option nounput

%x mlc
%x inc

%%

{identifier}            {
#if ! defined SCANDEBUG
                          yylval.lval_id = strdup(yytext);
#endif
                          if ( ( temp_token 
                               = htlookup( global_table, 
                                           upstrdup(yytext, yyleng), 
                                           yyleng ) ) ) {
                              return tokensym(temp_token);
                          } else {
                              return token(Identifier);
                          }
                        }

{int}                   {
#if ! defined SCANDEBUG
                          yylval.lval_literal = strdup(yytext);
#endif
                          return token(IntLiteral);
                        }

{float}                 {
#if ! defined SCANDEBUG
                          yylval.lval_literal = strdup(yytext);
#endif
                          return token(FloatLiteral);
                        }

{string}                {
#if ! defined SCANDEBUG
                          yylval.lval_literal = strdup(yytext);
#endif
                          return token(StringLiteral);
                        }

{charstring}            {
#if ! defined SCANDEBUG
                          yylval.lval_literal = strdup(yytext);
#endif
                          return token(CharLiteral);
                        }

{punctuation}|"["|"]"   {
                          return token(yytext[0]);
                        }
#{space}*PRAGMA{space}+{prefix}{trailer}*{space}*\({space}*{string}{space}*\) {
#ifndef SCANDEBUG
                          char * includeidx;
                          char * incfilename;
                          FILE * file_handle;
                          char * bslashidx; /* ah yes */
                          includeidx = strstr(yytext,"include");
                          if ( includeidx ) {
                             incfilename = strdup(strchr(includeidx,'"')+1);
                             *strrchr(incfilename,'"') = 0;
                             for (bslashidx=strchr(incfilename,'\\');
                                  bslashidx; 
                                  bslashidx=strchr(bslashidx,'\\')) {
                               *bslashidx='/';
                             }
                             file_handle = try_open_file( incfilename );
                             if ( file_handle ) {
                               push_file( file_handle, incfilename, 0 );
                             } else {
                               fprintf(stderr,"include file %s not found\n", 
                                       incfilename);
                             }
                          }
#endif
                        }
{space}*                /* consume white space */
"//".*                  /* consume one line comments */
"/*"                    BEGIN mlc; /* switch to multi line comment statte */
<mlc>"*"/[^/]           /* consume stray asterisks */
<mlc>[^\*\n]            /* consume anything but asterisks and newlines */
<mlc>"*/"               BEGIN INITIAL; /* end of mult line comment */
<INITIAL,mlc>\n         line_number++;
"\r"                    /* consume ^Ms - courtesy win editors */
<<EOF>>                 {
#ifndef SCANDEBUG
                           if (current_file -> next_file ) {
                              yy_delete_buffer( YY_CURRENT_BUFFER );
                              if (current_file -> file_handle) {
                                 fclose(current_file -> file_handle);
                              }
                              current_file = current_file -> next_file;
                              file_name = current_file -> file_name;
                              line_number = current_file -> line_number;
                              cls_chain_current = current_file -> file_cls;
                              yy_switch_to_buffer(current_file->yybuffer);
                           } else {
                              yyterminate();
                           }
#else
                              yyterminate();
#endif
                         }

%%

/*

*/

int init_scanner(char * parsefiles[], int numfiles, const char * includedir,
                const char * extrafile, int verbose)
{
  FILE * file_handle;
  int i;
  verbosescan=verbose;
  current_symtab = htcreate("PRIMARY");
  current_qualtab = htcreate("QUALIFIERS");
  global_table = htcreate("GLOBAL");
  htinsert( global_table, "AS", 2, make_token(AS)) ;
  htinsert( global_table, "OF", 2, make_token(OF)) ;
  htinsert( global_table, "CLASS", 5, make_token(CLASS)) ;
  htinsert( global_table, "REF", 3, make_token(REF)) ;
  htinsert( global_table, "QUALIFIER", 9, make_token(QUALIFIER)) ;
  htinsert( global_table, "INSTANCE", 8, make_token(INSTANCE)) ;
  htinsert( global_table, "SINT8", 5, make_token(BaseTypeSINT8)) ;
  htinsert( global_table, "UINT8", 5, make_token(BaseTypeUINT8)) ;
  htinsert( global_table, "SINT16", 6, make_token(BaseTypeSINT16)) ;
  htinsert( global_table, "UINT16", 6, make_token(BaseTypeUINT16)) ;
  htinsert( global_table, "SINT32", 6, make_token(BaseTypeSINT32)) ;
  htinsert( global_table, "UINT32", 6, make_token(BaseTypeUINT32)) ;
  htinsert( global_table, "SINT64", 6, make_token(BaseTypeSINT64)) ;
  htinsert( global_table, "UINT64", 6, make_token(BaseTypeUINT64)) ;
  htinsert( global_table, "REAL32", 6, make_token(BaseTypeREAL32)) ;
  htinsert( global_table, "REAL64", 6, make_token(BaseTypeREAL64)) ;
  htinsert( global_table, "STRING", 6, make_token(BaseTypeSTRING)) ;
  htinsert( global_table, "CHAR16", 6, make_token(BaseTypeCHAR16)) ;
  htinsert( global_table, "BOOLEAN", 7, make_token(BaseTypeBOOLEAN)) ;
  htinsert( global_table, "DATETIME", 8, make_token(BaseTypeDATETIME)) ;
  htinsert( global_table, "TRUE", 4, make_token(BoolLiteral)) ;
  htinsert( global_table, "FALSE", 5, make_token(BoolLiteral)) ;
  htinsert( global_table, "NULL", 4, make_token(NullLiteral)) ;
  htinsert( global_table, "SCOPE", 5, make_token(SCOPE));      
  htinsert( global_table, "FLAVOR", 6, make_token(FLAVOR));        

#ifndef SCANDEBUG
  cls_chain_current = calloc(sizeof(class_chain),1);
  inst_chain_current = calloc(sizeof(class_chain),1);
  qual_chain_current = calloc(sizeof(qual_chain),1);  
  current_file = NULL;
  for (i=numfiles-1; i >=0; i--) {
    file_handle = fopen(parsefiles[i],"r");
    if (file_handle == NULL) {
      fprintf(stderr,"could not open %s\n",parsefiles[i]);   
      return -1;
    }
    push_file(file_handle,parsefiles[i],1);
  }
  includepath = (char*) includedir;
  extrainclude = (char*) extrafile;
  if (extrainclude) {
    file_handle = try_open_file( extrainclude );
    if (file_handle) {
      push_file(file_handle, extrainclude,0);
    } else {
      fprintf(stderr,"extra file %s not found\n", 
                      extrainclude);
      return -1;
    }
  }
#endif
  return 0;
}

void stop_scanner()
{
  htdelete( global_table );
  htdelete( current_symtab );
}

void push_file( FILE *file_handle, char * filename, int keep_symtab )
{
  file_stack_entry * fse = 
         calloc( sizeof(file_stack_entry), 1);
  fse -> file_handle = file_handle;
  fse -> file_name = strdup( filename );
  fse -> yybuffer = 
      yy_create_buffer( file_handle, YY_BUF_SIZE );
  if (current_file) {
    /* save state info */
    current_file -> file_cls = cls_chain_current;
    current_file -> line_number = line_number;
    current_file -> yybuffer = YY_CURRENT_BUFFER;
  }
  if ( !keep_symtab ) {
    cls_chain_current = calloc(sizeof(class_chain),1);
  }
  fse -> next_file = current_file;
  current_file = fse;
  file_name = fse -> file_name;
  line_number = 1;
  yy_switch_to_buffer( current_file -> yybuffer );
}

FILE * try_open_file( const char * filename )
{
  char filenamebuf[300];
  char * i1, * i2;
  FILE * file = fopen(filename,"r");
  
  if (file) {
    if (verbosescan)
      fprintf(stderr,"  + including file %s\n", filename);
    return file; /* handle local and absolute file names */
  }

  i1 = includepath ? strdup( includepath ) : NULL;
  while ( i1 ) {
    i2 = strchr(i1,':');
    if (i2) {
      *i2 = 0;
       i2 += 1;
    }
    sprintf(filenamebuf,"%s/%s",i1,filename);
    file = fopen(filenamebuf,"r");
    if ( file ) {
      if (verbosescan)
        fprintf(stderr,"  + including file %s\n", filenamebuf);
      return file;
    }
    i1 = i2;
  }
  return NULL;
}

char * upstrdup( const char * s, int slen )
{
  char * tmpstr = calloc( slen + 1, 1 );

  while( slen -- )
    tmpstr[ slen ] = (char) toupper( s[ slen ] );

  return tmpstr;
}
