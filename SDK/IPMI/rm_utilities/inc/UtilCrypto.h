#ifndef __UTILCRYPTO_H_


#define CRYPTO_BLOCK_SIZE (256+20-1)
#define CRYPTO_PW_MAX_LEN 96

#ifdef __UTILCRYPTO_C_

#else
extern int aes256_encrypt(unsigned char *plaintext, int *len,unsigned char *output);
extern int aes256_decrypt(unsigned char *ciphertext, int *len,unsigned char *output);
extern int aes128_encrypt(unsigned char *plaintext, int *len,unsigned char *output);
#endif



#endif
