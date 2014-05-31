

#ifndef _W83795ADGDRV_H_
#define _W83795ADGDRV_H_


#define W83795ADG_CONFIG_REG          			0x01
#define CONFIG_DEFAUL_VALUE						0x15
#define CONFIG_RESET_INIT						0x80


#define W83795ADG_CONFIGURATION_REG          	0x40
#define W83795ADG_VBATMONITOR_REG          		0x5D
#define W83795ADG_CONFIGURATION_AMONT        	4


#define ENABLE									1
#define DISABLE									0

#define W83795ADG_BANK_SELECT_REG				0x00 
#define CHIP_ID_REG								0xFE
#define CHIP_ID									0x79

#define VOLT_CTRL1								0x02
#define VOLT_CTRL2								0x03
#define TEMP_CTRL1								0x04 
#define TEMP_CTRL2								0x05 

#define DTSE 									0x02 
#define VSEN1									0x10
#define VSEN2									0x11
#define VSEN3									0x12
#define VSEN4									0x13
#define VSEN5									0x14
#define VSEN6									0x15
#define VSEN7									0x16
#define VSEN8									0x17
#define VSEN9									0x18
#define VSEN10									0x19
#define VSEN11									0x1A
#define VTT										0x1B
#define VDD3									0x1C
#define VSB3									0x1D
#define VBAT									0x1E


#define TD1_TR1_VDSEN14_MF						0x21
#define TD2_TR2_VDSEN15_MF						0x22
#define TD3_TR3_VDSEN16_MF						0x23
#define TD4_TR4_VDSEN17_MF						0x24
#define DTS1									0x26
#define DTS2									0x27
#define DTS3									0x28
#define DTS4									0x29
#define DTS5									0x2A
#define DTS6									0x2B
#define DTS7									0x2C
#define DTS8									0x2D
#define TR5_VSEN12_MF							0x1F
#define TR6_VSEN13_MF							0x20



#define SMI_STS1								0x41
#define SMI_STS2								0x42
#define SMI_STS3								0x43
#define SMI_STS4								0x44
#define SMI_STS5								0x45
#define SMI_STS6								0x46
#define SMI_MASK6								0x4D
#define SMI_MASK6_DEFAULT_VALUE					0x7F

#define SMI_CTRL								0x40
#define SMI_CTRL_DEFAULT_VALUE					0x10

#define CHASSIS_INTRU_OFFSET					0x40

#define ENABLE_INTRRUPT_STATUS_MODE				~(1 << 7)
#define ENABLE_SMI								(1 << 1)
#define CLEAR_CASEOPEN_LATCH				    (1 << 7)
#define ENABLE_CASEOPEN_MASK				   (1 << 6)


#define ENABLE_REAL_TIME						0x80
#define DISABLE_MONITORING						0x00
#define ENABLE_TD								0x01
#define ENABLE_VOLTAGE							0x02
#define ENABLE_TR								0x03

#define ENABLE_VSEN								0x02
#define ENABLE_TR								0x03					

#define EN_VBAT									0x40

#define	CPU_OFFSET_REG							0xDF
#define CPU_OVERTEMP_REG						0xB2

typedef struct 
{
	BYTE  b_RegAddr;
	BYTE  b_Value;
} St_W83795ADGConfigureTypeTag;

extern STATUS Plt_W83795ADG_ClearCaseOpenLatch(void);
extern STATUS SetupW83795ADGDRV (DWORD ,DWORD ,void * ,void *);
extern STATUS PryW83795ADGDRV (DWORD ,DWORD ,void * ,void *);
extern STATUS WriteW83795ADGDRV (DWORD ,DWORD ,void * ,void *);
extern STATUS ReadW83795ADGDRV (DWORD ,DWORD ,void * ,void *);

#ifdef _W83795ADGDRV_C_

St_SensorAPITag ws_St_W83795ADGAPI=
{
	SetupW83795ADGDRV,
	PryW83795ADGDRV,
	WriteW83795ADGDRV,
	ReadW83795ADGDRV,
};

#else


extern St_SensorAPITag ws_St_W83795ADGAPI;

#endif 

#endif 


