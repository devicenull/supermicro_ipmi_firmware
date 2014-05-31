#ifndef _UTILPOSTCODE_H_
#define _UTILPOSTCODE_H_


#define IOCTL_GET_POSTCODES												  0x1556
#define POST_CODE_SIZE													  0xff
unsigned char UtilGetLatestPOSTCode(void);
unsigned char UtilGetPOSTCode(unsigned char * ucpPOSTCode);
#endif
