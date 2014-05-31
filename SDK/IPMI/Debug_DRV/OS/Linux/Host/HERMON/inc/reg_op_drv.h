wait_queue_head_t waitq;
int flag = 0;
#define REG(x)	((*(volatile unsigned char *)(x)))
#define REG8(x)	((*(volatile unsigned char *)(x)))
#define REG16(x)	((*(volatile unsigned short *)(x)))
#define REG32(x)	((*(volatile unsigned int *)(x)))
