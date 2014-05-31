#ifndef EE24C256_H_
#define EE24C256_H_

#define I2C_EE24C256_MAJOR		240
#define I2C_EE24C128_MAJOR		240
#define I2C_EE24C64_MAJOR		240

extern char ee24c256[20];
extern char ee24c128[20];
extern char ee24c64[20];
extern char null_string[20];

#ifdef _EE24C256_C_
STATUS EE24C256Init(
                     unsigned char b_channel,
                     unsigned char b_slaveaddr
                   );

STATUS EE24C256Read(
                     unsigned char b_NVRamBlock,
                     unsigned char b_slaveaddr,
                     unsigned short int w_offset,
                     unsigned short int w_length,
                     void *ReadBuf
                   );

STATUS EE24C256Write(
                      unsigned char b_NVRamBlock,
                      unsigned char b_slaveaddr,
                      unsigned short int w_offset,
                      unsigned short int w_length,
                      void *WriteBuf
                    );
                    
STATUS EE24C256ReadDrv(
                      BYTE b_channel,
                      BYTE b_slaveaddr,
                      unsigned short int w_offset,
                      unsigned char w_length,
                      BYTE *ReadBuf
                    );
 
STATUS EE24C256WriteDrv(
                       BYTE b_channel,
                       BYTE b_slaveaddr,
                       unsigned short int w_offset,
                       unsigned char w_length,
                       BYTE *WriteBuf
                     );                   
            
#else

extern STATUS EE24C256Init(
							unsigned char b_channel,
                     		unsigned char b_slaveaddr
                     		);
extern STATUS EE24C256Read(	
						unsigned char b_NVRamBlock,
						unsigned short int w_offset,
              			unsigned short int w_length,
              			void *ReadBuf
             		);
             		
extern STATUS EE24C256Write(
		    			unsigned char b_NVRamBlock,
                    	unsigned short int w_offset,
                    	unsigned short int w_length,
                    	void *WriteBuf
	      	  		 );                      
#endif  

#endif  



