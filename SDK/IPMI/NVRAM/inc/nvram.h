
#ifndef NVRAM_H_
#define NVRAM_H_

#define FRUTYPE	    0
#define SELTYPE     1
#define SDRTYPE     2
#define PSTYPE      3
#define OEMPSTYPE   4
#define OEMFRUTYPE  5
#define FWFIREWALLTYPE 6

#define NULL_FRU_INDEX 0xFF

#define NVRAM_BASE	0x00	

typedef struct NVRamInfoType
{
	unsigned long int d_length;
} NVRamInfo;
	
typedef struct NVRamAPIType
{
	STATUS (* NVRamInfoGet)(
                             unsigned char b_NVRamType, NVRamInfo *
                           );
                        
    STATUS (* NVRamInit)(
                        unsigned char b_NVRamBlock,
                        unsigned char b_slaveaddr
                      );
    STATUS (* NVRamRead)(
                        #ifdef OS_LINUX
    					unsigned char b_NVRamBlock,
						#elif OS_THREAD_X
    					unsigned char b_channel,				 
    					#endif 
                        unsigned char b_slaveaddr,
                        unsigned short int w_offset,
                        unsigned short int w_length,
                        void *ReadBuf
                      );
    STATUS (* NVRamWrite)(
    					 #ifdef OS_LINUX
    					 unsigned char b_NVRamBlock,
    					 #elif OS_THREAD_X
    					 unsigned char b_channel,				 
    					 #endif            
                         unsigned char b_slaveaddr,
                         unsigned short int w_offset,
                         unsigned short int w_length,
                         void *WriteBuf
                       ); 
    STATUS (* NVRamCommit)(unsigned char, unsigned short int, unsigned short int);
} NVRamAPI;

typedef struct NVRamTableType
{
	unsigned char b_channel;
	unsigned char b_slaveaddr;
	unsigned short int w_offset;
	unsigned short int w_size;
	char b_nvramtype;
	NVRamAPI *APIEntryPoint;
} NVRamTable;


typedef struct
{
    unsigned char MB_FRU_Index;
} FRU_TBL_STRUC_TAG;

extern NVRamTable at_St_NVRamTable[];



#ifndef _NVRAM_C	
	#ifdef OS_THREAD_X
		unsigned char SetupMBFRUTbl(unsigned char b_FRU_ID, unsigned char *p_FRUTblIdx);		
	#endif
	
    #ifdef OS_LINUX 
    	extern int NVRAM_Shm_AT(void);
    	extern int NVRAM_Shm_DT(void);
		extern unsigned char SetupMBFRUTbl(unsigned char b_FRU_ID, unsigned char *p_FRUTblIdx);
	#endif
	
	extern STATUS NVRamInit(unsigned char b_NVRamType, unsigned char b_Index);
	extern STATUS NVRamRead(
	                         unsigned char b_NVRamType,
	                         unsigned short int w_offset,
	                         unsigned short int w_length,
	                         void *p_ReadBuf
	                       );
	                     
	extern STATUS NVRamWrite(
	                          unsigned char b_NVRamType,
	                          unsigned short int w_offset,
	                          unsigned short int w_length,
	                          void *p_WriteBuf
	                        );
	extern STATUS NVRamInfoGet(unsigned char b_NVRamType, NVRamInfo *);	                      
	extern STATUS NVRamCommit(unsigned char, unsigned short int, unsigned short int);
	
#endif 

#endif 


