
#define ADC_MAJOR				246
#define ADC_MODULE_NAME			"Analog to Digital Converter"
#define ADC_IRQ					31

#define WPCM450_VA_B800	   		WPCM450_ADDR(0x00030000)
#define WPCM450_VA_B000	   		WPCM450_ADDR(0x00010000)

#define CLK_BA					WPCM450_VA_B000+0x200
#define ADC_BA					WPCM450_VA_B800+0x9000
#define CLK_REG_TOTAL_SIZE		0x09
#define ADC_REG_TOTAL_SIZE		0x02

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;

#define WriteRegB(port,value)	*((volatile unsigned char *)(port))=value
#define ReadRegB(port)			*((volatile unsigned char *)(port))
#define WriteRegW(port,value)	*((volatile unsigned short *)(port))=value
#define	ReadRegW(port)			*((volatile unsigned short *)(port))
#define WriteRegD(port,value)	*((volatile unsigned long *)(port))=value
#define	ReadRegD(port)			*((volatile unsigned long *)(port))

#define	CLKDIV					(CLK_BA+0x08)		
#define ADC_CON					(ADC_BA+0x00)		
#define ADC_DATA				(ADC_BA+0x04)		


#define CLK_48MHZ				0x00000000
#define CLK_24MHZ				0x10000000
#define CLK_12MHZ				0x20000000
#define CLK_6MHZ				0x30000000

#define ADC_INT_EN				0x00200000
#define ADC_INT					0x00040000
#define ADC_EN					0x00020000
#define	ADC_RST					0x00010000
#define ADC_CONV				0x00002000

#define IOCTL_GET_CONVERT		0xFFF6

typedef struct
{
	unsigned char uc_Mux;	
	unsigned long uc_ConvertedData;
}HW_ADCInfoTag;

struct hermon_adc_dev 
{
	wait_queue_head_t waitq;
	HW_ADCInfoTag *HW_ADCInfo;
	int isr_flag;
};
