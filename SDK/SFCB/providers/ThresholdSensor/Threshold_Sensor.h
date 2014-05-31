

#ifndef SYSLOGLOGUTIL_H
#define SYSLOGLOGUTIL_H


#include <time.h>


#ifdef __cplusplus 
extern "C" {
#endif


#ifdef DEBUG
#else
#endif


void GetDeviceID(unsigned char evt_rd_type_code,unsigned char sensor_type_code,
		unsigned char sensor_num, void *sdr_info, char* sn_device_id,
		unsigned int sn_specific_off);
void cHexToString(unsigned char c,unsigned char *cb);


#ifdef __cplusplus 
}
#endif

#endif

