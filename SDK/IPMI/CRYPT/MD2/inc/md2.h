

typedef struct {
  unsigned char state[16];                                         
  unsigned char checksum[16];                                   
  unsigned int count;                         
  unsigned char buffer[16];                                 
} MD2_CTX;

void MD2Init PROTO_LIST ((MD2_CTX *));
void MD2Update PROTO_LIST ((MD2_CTX *, unsigned char *, unsigned int));
void MD2Final PROTO_LIST ((unsigned char [16], MD2_CTX *));





