 
#ifndef _WDTDRV_H_
#define _WDTDRV_H_


#define WDT_MAJOR	247
#define WDT_MINOR 	8

#define IOCTL_ENABLE	0xFFF1
#define IOCTL_RESET		0xFFF2
#define IOCTL_DISABLE	0xFFF3
#define IOCTL_SETTMR	0xFFF4
#define IOCTL_CLRTMR	0xFFF5

#define WTCR_WTE			  0x0080
#define WTCR_WTIE			  0x0040
#define WTCR_WTIS			  0x0030
#define WTCR_WTIF			  0x0008
#define WTCR_WTRF			  0x0004
#define WTCR_WTRE			  0x0002
#define WTCR_WTR			  0x0001

#define WTIS0	0x00
#define WTIS1	0x01
#define WTIS2	0x02
#define WTIS3	0x03


#endif 



