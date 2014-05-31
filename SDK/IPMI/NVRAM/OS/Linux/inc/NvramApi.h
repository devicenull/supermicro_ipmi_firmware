#ifndef _NVRAMAPI_H_
#define _NVRAMAPI_H_

#define NVRAM_INIT_PATTERN	0x00
#define VALID_CHECKSUM		1
#define ERR_RETRY_TIMES		5

#define ACCESS_ERROR   		0xFF
#define ACCESS_EPT_NV		0xFE

#ifdef _NVRAMAPI_C_
#else
extern int set_semvalue(int );
extern void sem_err_retry(jmp_buf );
extern int sem_p(int );
extern int sem_v(int );
extern void nvram_err_retry(jmp_buf );
extern int NVRamServiceInit(int *);
extern int GetNvSem(void);
extern int NVRamAccessCheck(
    		 unsigned char b_NVRamBlock,
                 unsigned short int w_offset,
                 unsigned short int w_length
                    );
#endif



#endif

