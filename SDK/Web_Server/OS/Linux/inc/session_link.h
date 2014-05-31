
#ifndef _SESSION_LINK_H_
#define _SESSION_LINK_H_

#include "sinfo.h"
#include <sys/stat.h>
#include "status.h"  

#define MAX_SESSION_ID_LEN 	20

#define BUFSIZE 135000
#define LOG_USER_LENGTH 128
#define REMOTE_ADDR1 getenv("REMOTE_ADDR")
#define HTTP_COOKIE1 getenv("HTTP_COOKIE")
#define check_session(x) check_session_api(x)


STATUS ATEN_Shm_AT();
STATUS ATEN_Shm_DT();

int start_session(int, char *);
int check_session_api( BaseInfo *);
void clean_session_file( void );
void print_session_error(char *);

int fn(const char *file, const struct stat *sb, int flag);
int reject_session(char *, int);

int GetSessCookie(char *p_b_buffer, char *p_b_sid);
/* Tony, 09/03/2012 Add host and user info { */
int GetSessionUserName(char *p_user_name);
/* Tony, 09/03/2012 Add host and user info } */
// int GetClientIP(char *p_ClientIP);
extern int GetClientIP(char *p_ClientIP,int max_ip_len);

#endif





