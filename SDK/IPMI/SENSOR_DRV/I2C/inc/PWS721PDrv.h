

#ifndef _PWS721PDRV_H_
#define _PWS721PDRV_H_


typedef struct 
{
	BYTE  b_RegAddr;
	BYTE  b_Value;
} St_PWS721PConfigureTypeTag;

extern STATUS SetupPWS721PDRV (DWORD ,DWORD ,void * ,void *);
extern STATUS PryPWS721PDRV (DWORD ,DWORD ,void * ,void *);
extern STATUS WritePWS721PDRV (DWORD ,DWORD ,void * ,void *);
extern STATUS ReadPWS721PDRV (DWORD ,DWORD ,void * ,void *);
extern STATUS ReadPWS721P_ME_DRV (DWORD ,DWORD ,void * ,void *);
extern STATUS ReadPWS721PDRV_Type2 (DWORD ,DWORD ,void * ,void *);

#define PS_REG_STATUS_FRU				0x0c
#define PS_REG_STATUS_PMBUS				0x78
#define PS_REG_AC_INPUT_VOLTAGE			0x88
#define PS_REG_AC_INPUT_CURRENT			0x89
#define PS_REG_DC12V_OUTPUT_VOLTAGE		0x8B
#define PS_REG_DC12V_OUTPUT_CURRENT		0x8C
#define PS_REG_TEMPERATURE_1			0x8D
#define PS_REG_TEMPERATURE_2			0x8E
#define PS_REG_FAN_1					0x90
#define PS_REG_FAN_2					0x91
#define PS_REG_DC12V_OUTPUT_POWER		0x96
#define PS_REG_AC_INPUT_POWER			0x97
#define PS_REG_PMBUS_REVISION			0x98
#define PS_REG_PWS_SERIAL_NUMBER		0xD0
#define PS_REG_PWS_MODULE_NUMBER		0xE0
#define PS_REG_PWS_REVISION				0xF0

/* Tony, 10/23/2012 add battery backup power { */
#define BBP_REG_TEMP							0x09
#define BBP_REG_POWER_STATUS					0x0C
#define BBP_REG_TEMP_HIGH_LIMIT					0x0D
#define BBP_REG_REAMIN_BATT_ENERGY				0xE0
#define BBP_REG_VOLTAGE_LOW_BYTE				0xE1
#define BBP_REG_VOLTAGE_HIGH_BYTE				0xE2
#define BBP_REG_CURRENT_LOW_BYTE				0xE3
#define BBP_REG_CURRENT_HIGH_BYTE				0xE4
#define BBP_REG_STATUS_LOW_BYTE					0xE5
#define BBP_REG_STATUS_HIGH_BYTE				0xE6
#define BBP_REG_CYCLECOUNT_LOW_BYTE				0xE7
#define BBP_REG_CYCLECOUNT_HIGH_BYTE			0xE8
#define BBP_REG_DISCHARGE_RESPONSE				0xE9
#define BBP_REG_DISCHARGE_COMMAND				0xEA
#define BBP_REG_PS_OUTPUT_WATT_RATING_LOW_BYTE	0xF2
#define BBP_REG_PS_OUTPUT_WATT_RATING_HIGH_BYTE	0xF3
#define BBP_REG_WRITE_PROTECTION_CONTROL		0xFF

/* Tony, 11/02/2012 Add bpp info { */
extern STATUS WriteBBPRgister (BYTE b_num,BYTE b_BBPDischargeCommand);
/* Tony, 11/02/2012 Add bpp info } */

/* Tony, 10/23/2012 add battery backup power } */
#ifdef _PWS721PDRV_C_

St_SensorAPITag ws_St_PWS721PAPI=
{
	SetupPWS721PDRV,
	PryPWS721PDRV,
	WritePWS721PDRV,
	ReadPWS721PDRV,
};

St_SensorAPITag ws_St_PWS721PFromME_API=
{
	SetupPWS721PDRV,
	PryPWS721PDRV,
	WritePWS721PDRV,
	ReadPWS721P_ME_DRV,
};

#else


extern St_SensorAPITag ws_St_PWS721PAPI;
extern St_SensorAPITag ws_St_PWS721PFromME_API;
#endif 

#endif 


