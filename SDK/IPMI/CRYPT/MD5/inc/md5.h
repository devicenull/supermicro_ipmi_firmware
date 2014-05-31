 

#ifdef __alpha
typedef unsigned int UINT4;
#else
typedef unsigned long int UINT4;
#endif

typedef struct {
  UINT4 buf[4];                                    
  UINT4 i[2];                   
  unsigned char in[64];                              
} MD5_CTX;

void MD5Init(MD5_CTX *mdContext);
void MD5Update(MD5_CTX *mdContext, unsigned char *bug, unsigned int len);
void MD5Final(unsigned char digest[16], MD5_CTX *mdContext);
void Transform(UINT4 *buf, UINT4 *in);





