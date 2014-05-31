#ifndef _FILEAPI_H_
#define _FILEAPI_H_



#ifdef _FILEAPI_C_
STATUS FILEAPIInit(
		    unsigned char b_NVRamBlock,
		    unsigned char b_slaveaddr
	);

STATUS FILEAPIRead(
                     unsigned char b_NVRamBlock,
                     unsigned char b_slaveaddr,
                     unsigned short int w_offset,
                     unsigned short int w_length,
                     void *ReadBuf
                   );

STATUS FILEAPIWrite(
                      unsigned char b_NVRamBlock,
                      unsigned char b_slaveaddr,
                      unsigned short int w_offset,
                      unsigned short int w_length,
                      void *WriteBuf
                   );

STATUS FILEAPICommit(
		              unsigned char b_NVRamBlock,
                      unsigned short int w_offset,
                      unsigned short int w_length
	      	        );

static STATUS FileReadDrv(
                      int fd,
                      unsigned short int w_offset,
                      unsigned short int w_length,
                      unsigned char *ReadBuf
                    );
 
static STATUS FileWriteDrv(
                       int fd,
                       unsigned short int w_offset,
                       unsigned short int w_length,
                       unsigned char *WriteBuf
                     );                   
static int openfd(unsigned char b_NVRamBlock);
#else 

extern STATUS FILEAPIInit(
		    unsigned char b_NVRamBlock,
		    unsigned char b_slaveaddr
	);

extern STATUS FILEAPIRead(
                     unsigned char b_NVRamBlock,
                     unsigned char b_slaveaddr,
                     unsigned short int w_offset,
                     unsigned short int w_length,
                     void *ReadBuf
                   );

extern STATUS FILEAPIWrite(
                      unsigned char b_NVRamBlock,
                      unsigned char b_slaveaddr,
                      unsigned short int w_offset,
                      unsigned short int w_length,
                      void *WriteBuf
                    );

extern STATUS FILEAPICommit(
		              unsigned char b_NVRamBlock,
                      unsigned short int w_offset,
                      unsigned short int w_length
	      	        );
#endif

#endif

