

#ifndef _TI_TMP75_AIDGKT_H_
#define _TI_TMP75_AIDGKT_H_

#define TMP_REGISTER				0x00
#define CONFG_REGISTER				0x01
#define TLOW_REGISTER				0x02
#define THIGH_REGISTER				0x03

#define SHUTDOWN_MODE				0x01
#define THERMOSTAT_MODE				0x02
#define POLARITY					0x04
#define FAULT_QUEUE_1				0x08
#define FAULT_QUEUE_2				0x10
#define RESOLUTION_1				0x20
#define RESOLUTION_2				0x40
#define ONE_SHOT					0x80



extern STATUS SetupTI_TMP75_AIDGKT (DWORD ,DWORD ,void * ,void *);
extern STATUS PryTI_TMP75_AIDGKT (DWORD ,DWORD ,void * ,void *);
extern STATUS WriteTI_TMP75_AIDGKT (DWORD ,DWORD ,void * ,void *);
extern STATUS ReadTI_TMP75_AIDGKT (DWORD ,DWORD ,void * ,void *);

#ifdef _TI_TMP75_AIDGKT_C_

St_SensorAPITag at_St_TI_TMP75_AIDGKTAPI=
{
	SetupTI_TMP75_AIDGKT,
	PryTI_TMP75_AIDGKT,
	WriteTI_TMP75_AIDGKT,
	ReadTI_TMP75_AIDGKT,
};

#else


extern St_SensorAPITag at_St_TI_TMP75_AIDGKTAPI;

#endif 

#endif 


