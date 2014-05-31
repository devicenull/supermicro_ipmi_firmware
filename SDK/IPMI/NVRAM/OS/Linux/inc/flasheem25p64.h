
#ifndef FLASHEEM25P64_H_  
#define FLASHEEM25P64_H_

#define I2C_EE24C256_MAJOR		240
#define I2C_EE24C128_MAJOR		240
#define I2C_EE24C64_MAJOR		240

extern char ee24c256[20];
extern char ee24c128[20];
extern char ee24c64[20];
extern char null_string[20];

#ifdef _FLASHEEM25P64_C_
STATUS FlasheeM25p64Init(
                     unsigned char b_channel,
                     unsigned char b_slaveaddr
                   );

STATUS FlasheeM25p64Read(
                     unsigned char b_NVRamBlock,
                     unsigned char b_slaveaddr,
                     unsigned short int w_offset,
                     unsigned short int w_length,
                     void *ReadBuf
                   );

STATUS FlasheeM25p64Write(
                      unsigned char b_NVRamBlock,
                      unsigned char b_slaveaddr,
                      unsigned short int w_offset,
                      unsigned short int w_length,
                      void *WriteBuf
                    );
                    
STATUS FlasheeM25p64ReadDrv(
							unsigned char b_NVRamBlock,
							unsigned short int w_offset,
							unsigned int w_length,
							BYTE *ReadBuf
							);
 
STATUS FlasheeM25p64WriteDrv(unsigned char b_NVRamBlock);                   
            
#else

extern STATUS FlasheeM25p64Init(
							unsigned char b_channel,
                     		unsigned char b_slaveaddr
                     		);
extern STATUS FlasheeM25p64Read(	
						unsigned char b_NVRamBlock,
						unsigned short int w_offset,
              			unsigned short int w_length,
              			void *ReadBuf
             		);
             		
extern STATUS FlasheeM25p64Write(
		    			unsigned char b_NVRamBlock,
                    	unsigned short int w_offset,
                    	unsigned short int w_length,
                    	void *WriteBuf
	      	  		 );  

extern STATUS FlasheeM25p64ReadDrv(
							unsigned char b_NVRamBlock,
							unsigned short int w_offset,
							unsigned int w_length,
							BYTE *ReadBuf
							);
 
extern STATUS FlasheeM25p64WriteDrv(unsigned char b_NVRamBlock); 
                    
#endif  

#endif  



