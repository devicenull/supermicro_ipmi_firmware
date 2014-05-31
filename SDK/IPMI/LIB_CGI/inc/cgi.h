#ifndef _CGI_H_
#define _CGI_H_
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/queue.h>
#include "status.h"  
#include "cgi_misc.h"

/* Supermicro, Tony Huang, 2012/08/03, add cgi post getValue function { */
#define GetCgiPostVar    cgiGetPostVariable
/* Supermicro, Tony Huang, 2012/08/03, add cgi post getValue function } */
#define GetVar    cgiGetVariable
#define SetVar    cgiSetVariable
#define SetSize   cgiSetSize     
#define SetArray  cgiSetArray			
#define cgi_printf  printf

typedef struct cgiVars{
    TAILQ_ENTRY(cgiVars)    entries;
    void 					*value;
	char 				    *name;
}St_CgiVarsList;
#ifndef _VAR_C_
extern int cgiInitialize(void);
extern int FreeCGIResoure(void);
extern const char *cgiGetVariable(const char *name, unsigned int buff_len);
extern const char *cgiGetPostVariable(const char *name, unsigned int buff_len);
#endif
extern void cgiCopyFILE(unsigned char priv,const char *directory,const char *tmpl);
extern void cgiCopyIMG(FILE   *out,const char *directory,const char *tmpl);
extern void cgiCopyHTM(unsigned char priv,const char *directory,const char *tmpl);
extern int set_html_tagvalue(const char *buffer,const char *tag,const char *value);
extern char *get_cgi_env(unsigned char *env,int);
extern int  get_cgi_script_filename(char *cginame,int);
extern int  get_cgi_content_length(int max_len);
extern unsigned char *search_content_type(char *str);
extern STATUS ATEN_Shm_AT();
extern STATUS ATEN_Shm_DT();

#endif
