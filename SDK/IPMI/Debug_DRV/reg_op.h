
/* IO CTRL for accessing device driver */
#define IOCTL_READ					0xFFF2
#define IOCTL_WRITE					0xFFF3
#define IOCTL_READ8				0xFFF4
#define IOCTL_READ16				0xFFF5
#define IOCTL_READ32				0xFFF6
#define IOCTL_WRITE8					0xFFF7
#define IOCTL_WRITE16					0xFFF8
#define IOCTL_WRITE32					0xFFF9

typedef struct
{
	unsigned long offset;		
	unsigned char data;
	unsigned char data8;
	unsigned short data16;
	unsigned int data32;
}RegInfo;
