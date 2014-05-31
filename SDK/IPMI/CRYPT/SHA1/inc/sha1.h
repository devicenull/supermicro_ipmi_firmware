
#ifndef _SHA1_H_
#define _SHA1_H_

typedef struct
{
    unsigned long int total[2];
    unsigned long int state[5];
    unsigned char buffer[64];
}
sha1_context;

void sha1_starts( sha1_context *ctx );
void sha1_update( sha1_context *ctx, unsigned char *input, unsigned long int length );
void sha1_finish( sha1_context *ctx, unsigned char digest[20] );

#endif 





