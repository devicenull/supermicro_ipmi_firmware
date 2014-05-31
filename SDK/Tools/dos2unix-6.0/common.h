/*
 *   Copyright (C) 2009-2012 Erwin Waterlander
 *   All rights reserved.
 * 
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice in the documentation and/or other materials provided with
 *      the distribution.
 * 
 *   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY
 *   EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *   PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE
 *   FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 *   OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 *   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 *   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 *   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* define feature test macros for realpath() -- needed on    */
/* systems that have S_ISLNK, but chicken/egg means we must  */
/* define early, before including stdlib.h (or sys/stat.h)   */
#define _XOPEN_SOURCE 500

/* similarly, instead of realpath we like to use, if         */
/* available, the canonicalize_file_name() function, which   */
/* is a GNU extension. We only ACTUALLY use the function if  */
/* USE_CANONICALIZE_FILE_NAME is defined, but we don't define*/
/* that until later. So...define the feature test macro now. */
#define _GNU_SOURCE

#ifdef ENABLE_NLS

#include <libintl.h>
#define _(String) gettext (String)
#define gettext_noop(String) String
#define N_(String) gettext_noop (String)

#else

#define _(String) (String)
#define N_(String) String
#define textdomain(Domain)
#define bindtextdomain(Package, Directory)

#endif

#if defined(DJGPP) || defined(__TURBOC__) /* DJGPP */
#  include <dir.h>
#else
#  ifndef __MSYS__
#    include <libgen.h>
#  endif
#endif
#ifndef __TURBOC__
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utime.h>
#include <limits.h>
#ifdef __TURBOC__
#define __FLAT__
#endif
#include <sys/stat.h>
#ifdef ENABLE_NLS
#include <locale.h>
#endif
#include <errno.h>
#ifdef D2U_UNICODE
#include <wchar.h>
#endif

#if (defined(__WATCOMC__) && defined(__NT__))  /* Watcom */
#  define WIN32 1
#endif

#if defined(__WATCOMC__) && defined(__I86__) /* Watcom C, 16 bit Intel */
#define MSDOS 1
#endif

#if defined(__WATCOMC__) && defined(__DOS__) /* Watcom C, 32 bit DOS */
#define MSDOS 1
#endif

#if defined(WIN32) && !defined(__CYGWIN__) /* Windows */
#define MSDOS 1
#endif

#if  defined(__TURBOC__) || defined(DJGPP) || defined(__MINGW32__)
/* Some compilers have no mkstemp().
 * Use mktemp() instead.
 * BORLANDC, DJGPP, MINGW32 */
#define NO_MKSTEMP 1
#endif

#if  defined(__TURBOC__) || defined(DJGPP) || defined(__MINGW32__) || defined(__WATCOMC__)
/* Some compilers have no chown(). */
#define NO_CHOWN 1
#endif

/* Watcom C defines S_ISLNK */
#ifdef __WATCOMC__
#undef S_ISLNK
#endif

#if defined(MSDOS) || defined(__OS2__)
/* Systems without soft links use 'stat' instead of 'lstat'. */
#define STAT stat
#else
#define STAT lstat
#endif

#if defined(MSDOS) || defined(__OS2__)
/* On some systems rename() will always fail if target file already exists. */
#define NEED_REMOVE 1
#endif

#if defined(MSDOS) || defined(__CYGWIN__) || defined(__OS2__) /* DJGPP, MINGW32 and OS/2 */
/* required for setmode() and O_BINARY */
#include <fcntl.h>
#include <io.h>
#endif

#if defined(MSDOS) || defined(__CYGWIN__) || defined(__OS2__)
  #define R_CNTRL   "rb"
  #define W_CNTRL   "wb"
#else
  #define R_CNTRL   "r"
  #define W_CNTRL   "w"
#endif

#define BINARY_FILE 0x1
#define NO_REGFILE  0x2
#define WRONG_CODEPAGE  0x4
#define OUTPUTFILE_SYMLINK 0x8
#define INPUT_TARGET_NO_REGFILE 0x10
#define OUTPUT_TARGET_NO_REGFILE 0x20
#define LOCALE_NOT_UTF8 0x40
#define WCHAR_T_TOO_SMALL 0x80
#define UNICODE_CONVERSION_ERROR 0x100

#define CONVMODE_ASCII  0
#define CONVMODE_7BIT   1
#define CONVMODE_437    437
#define CONVMODE_850    850
#define CONVMODE_860    860
#define CONVMODE_863    863
#define CONVMODE_865    865
#define CONVMODE_1252   1252

#define FROMTO_DOS2UNIX 0
#define FROMTO_MAC2UNIX 1
#define FROMTO_UNIX2DOS 0
#define FROMTO_UNIX2MAC 1

#define SYMLINK_SKIP 0
#define SYMLINK_FOLLOW 1
#define SYMLINK_REPLACE 2

#define FILE_MBS     0  /* Multi-byte string or 8-bit char */
#define FILE_UTF16LE 1  /* UTF-16 Little Endian */
#define FILE_UTF16BE 2  /* UTF-16 Big Endian */
#define FILE_UTF8    3  /* UTF-8 */

typedef struct
{
  int NewFile;                          /* is in new file mode? */
  int Quiet;                            /* is in quiet mode? */
  int KeepDate;                         /* should keep date stamp? */
  int ConvMode;                         /* 0: ascii, 1: 7bit, 2: iso */  
  int FromToMode;                       /* 0: dos2unix/unix2dos, 1: mac2unix/unix2mac */  
  int NewLine;                          /* if TRUE, then additional newline */
  int Force;                            /* if TRUE, force conversion of all files. */
  int Follow;                           /* 0: skip symlink, 1: follow symbolic link, 2: replace symlink. */
  int status;
  int stdio_mode;                       /* if TRUE, stdio mode */
  int error;                            /* an error occurred */
  int bomtype;                          /* byte order mark */
  int add_bom;                          /* 1: write BOM */
} CFlag;


int symbolic_link(char *path);
int regfile(char *path, int allowSymlinks, CFlag *ipFlag, char *progname);
int regfile_target(char *path, CFlag *ipFlag, char *progname);
void PrintUsage(char *progname);
void PrintBSDLicense(void);
void PrintVersion(char *progname);
#ifdef ENABLE_NLS
void PrintLocaledir(char *localedir);
#endif
FILE* OpenInFile(char *ipFN);
FILE* OpenOutFile(int fd);
#if defined(__TURBOC__) || defined(__MSYS__)
char *dirname(char *path);
#endif
#ifdef NO_MKSTEMP
FILE* MakeTempFileFrom(const char *OutFN, char **fname_ret);
#else
int MakeTempFileFrom(const char *OutFN, char **fname_ret);
#endif
int ResolveSymbolicLink(char *lFN, char **rFN, CFlag *ipFlag, char *progname);
FILE *read_bom (FILE *f, int *bomtype);
#ifdef D2U_UNICODE
wint_t d2u_getwc(FILE *f, int bomtype);
wint_t d2u_ungetwc(wint_t wc, FILE *f, int bomtype);
wint_t d2u_putwc(wint_t wc, FILE *f, CFlag *ipFlag);
#endif

