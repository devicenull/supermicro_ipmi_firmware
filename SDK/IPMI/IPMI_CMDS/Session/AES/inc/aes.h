
#ifndef _AES_H_
#define _AES_H_

#ifndef uint8
#define uint8  unsigned char
#endif

#ifndef uint32
#define uint32 unsigned long int
#endif

typedef struct
{
    uint32 erk[64];     
    uint32 drk[64];     
    int nr;             
}
aes_context;

#ifdef _AES_C_
int  aes_set_key( aes_context *ctx, uint8 *key, int nbits );
void aes_encrypt( aes_context *ctx, uint8 input[16], uint8 output[16] );
void aes_decrypt( aes_context *ctx, uint8 input[16], uint8 output[16] );
void AES_Codec_Fun(
					unsigned char *p_dataBuff, 
					unsigned char *p_key, 
					unsigned char *p_IV, 
					unsigned char keylength, 
					unsigned char Encode
				  );
void Xor(unsigned char* buff, unsigned char * IV);

#else 
extern void AES_Codec_Fun(
							unsigned char *p_dataBuff, 
							unsigned char *p_key, 
							unsigned char *p_IV, 
							unsigned char keylength, 
							unsigned char Encode
						 );

#endif
#endif 





