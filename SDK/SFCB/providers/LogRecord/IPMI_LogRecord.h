

#ifndef SYSLOGLOGUTIL_H
#define SYSLOGLOGUTIL_H


#include <time.h>


#ifdef __cplusplus 
extern "C" {
#endif


#ifdef DEBUG
#else
#endif



#define SYSLOG_CONFPATH "/etc/syslog.conf"

char* ConvertTimeStamp(unsigned long TimeStamp);

#ifdef __cplusplus 
}
#endif

#endif

