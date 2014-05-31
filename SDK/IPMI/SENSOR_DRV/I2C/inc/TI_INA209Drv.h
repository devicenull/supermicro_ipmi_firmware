#ifndef _TI_INA209_H_
#define _TI_INA209_H_

#define CONFIG_REG					0x00
#define	STATUS_REG					0x01
#define VOLT_S_REG					0x03
#define VOLT_B_REG					0x04
#define POWER_REG					0x05
#define	CURRENT_REG					0x06
#define	CALIBRATION_REG				0x16

#define CONF_VALUE_MSB				0x07
#define	CONF_VALUE_LSB				0xff

#define CAL_VALUE_MSB				0xb2
#define CAL_VALUE_LSB				0x30

#define CURRENT_LSB					0.000916
#define	SDR_M_CONSTANT				0xa0

#define CURRENT_ADJUST				(5.5/8.0)

extern STATUS SetupTI_INA209 (DWORD ,DWORD ,void * ,void *);
extern STATUS PryTI_INA209 (DWORD ,DWORD ,void * ,void *);
extern STATUS WriteTI_INA209 (DWORD ,DWORD ,void * ,void *);
extern STATUS ReadTI_INA209 (DWORD ,DWORD ,void * ,void *);

#ifdef _TI_INA209_C_

St_SensorAPITag at_St_TI_INA209API=
{
	SetupTI_INA209,
	PryTI_INA209,
	WriteTI_INA209,
	ReadTI_INA209,
};

#else


extern St_SensorAPITag at_St_TI_INA209API;

#endif 

#endif 


