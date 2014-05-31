 
#ifndef _GPIODRV_H_
#define _GPIODRV_H_


#define GPIO_MAJOR_NO 243
#define GPIO_MINOR_NO 0

#define IOCTL_INIT_ISR				0xFFF0
#define IOCTL_INIT					0xFFF1
#define IOCTL_READ					0xFFF2
#define IOCTL_WRITE					0xFFF3
//#define IOCTL_READ_CPLD_FLAG	0xFFF4	//linda add CPLD INT flag

#define UNUSED					0xFF
#define GPIO_READ_BUF_SIZE		0x20
typedef struct
{
	unsigned long int dw_HWInfo_1;		
	unsigned long int dw_HWInfo_2;
	unsigned long int dw_HWInfo_3;
	unsigned char data_buf[32];
}HWInfoTag;

typedef struct
{
    unsigned char b_PinNo;
    
    unsigned char b_EventType;

    unsigned char b_EventPolarity;

	unsigned char b_EventDebounce;
    
	void (*TaskFunc)(void);
}St_GPIO_HandlerTag;

typedef struct
{
    unsigned char b_CrtIdx;
    unsigned int a_b_ISR_Seq[GPIO_READ_BUF_SIZE];
    
}St_GPIO_ReadIsrInfoTag;

extern STATUS PltGPIO_Init(void);
extern STATUS PltGPIO_Setup(DWORD , DWORD, void *, void *);
extern STATUS PltGPIO_Pry(DWORD , DWORD, void *, void *);
extern STATUS PltGPIO_Write(DWORD , DWORD, void *, void *);
extern STATUS PltGPIO_Read(DWORD , DWORD, void *, void *);

extern unsigned char OEMPltGPIO_Setup( unsigned long int dw_HWInfo_1, unsigned long int dw_HWInfo_2, void *p_HWInfoPtr, void *p_DataBuf);
extern unsigned char OEMPltGPIO_Write( unsigned long int dw_HWInfo_1, unsigned long int dw_HWInfo_2, void *p_HWInfoPtr, void *p_DataBuf);
extern unsigned char OEMPltGPIO_Read( unsigned long int dw_HWInfo_1, unsigned long int dw_HWInfo_2, void *p_HWInfoPtr, void *p_DataBuf);

#endif 



