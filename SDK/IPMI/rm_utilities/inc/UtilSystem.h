#ifndef _UTILSYSTEM_H_
#define _UTILSYSTEM_H_


#ifdef _UTILSYSTEM_C_

#else 
extern int  run_shellcmd(char *cmd);
extern void xstrcpy(char *dest,const char *src,int len);
extern void xstrcat(char *dest, const char *src,int max_len);
#endif
#endif
