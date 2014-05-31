/**
 * $Id: mofdecl.h,v 1.2 2005/11/20 17:37:05 bestorga-oss Exp $
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
 * Description: Declarations needed by Parser and Scanner
 *
 */

#ifndef MOFDECL_H
#define MOFDECL_H

#include <hash.h>
#include <stdio.h>

extern hashentry * current_symtab;
extern hashentry * current_qualtab;
extern FILE * yyin;
extern int yylex(void);
int yyparse(void);
char * upstrdup( const char *, int );
int init_scanner(char * parsefiles[], int numfiles, const char * includedir, 
		 const char * extrafile, int verbose);
void stop_scanner(void);
FILE * try_open_file(const char * filename);
extern int line_number;
extern char * file_name;

#endif
