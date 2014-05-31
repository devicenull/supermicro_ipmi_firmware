

#ifndef _NCT6776FDRV_H_
#define _NCT6776FDRV_H_

#define NCT6776F_REG4E_BANKSELECT				0x4E
//#define NCT6776F_REG09_ATBANK7					0x09
//#define NCT6776F_REG_CPU_OVERTEMP				0x55

// Jack Change the VCore
#define FAN_LIMIT	0x786
#define AVCCHigh	0xe3
#define AVCCLow		0xb9
#define VcoreHigh	0xBA
#define VcoreLow	0x43
#define VCC3High	0xe3
#define VCC3Low		0xb9
#define VIN0High	0xf9
#define VIN0Low		0xcb
#define VIN1High	0xce
#define VIN1Low		0xa9
#define VIN2High	0xac
#define VIN2Low		0x8d
#define VIN3High	0xa8
#define VIN3Low		0x73
#define VSBHigh		0xe3
#define VSBLow		0xb9
#define VBATHigh	0xe3
#define VBATLow		0xb9
#define I2CAddr		0x2c 

//#define CPUFANOUT_ENABLED	1
#define CPUFANOUT_STOP_MODE	1
#define CPUFANOUT_TEMP_SOURCE	0xc
#define CPUFANOUT_CRITICAL_TEMP	0x5f
#define CPUFANOUT_CRITICAL_TOLERANCE	0x3
#define CPUFANOUT_T1	0x32
#define CPUFANOUT_T2	0x5a
#define CPUFANOUT_T3	0x5a
#define CPUFANOUT_T4	0x5a
#define CPUFANOUT_T4_TOLERANCE	0x5
#define CPUFANOUT_NONSTOP_PWM	0x33
#define CPUFANOUT_STEPUP_TIME	0x1
#define CPUFANOUT_STEPDOWN_TIME	0x1
#define CPUFANOUT_DC_PWM1	0x33
#define CPUFANOUT_DC_PWM2	0xff
#define CPUFANOUT_DC_PWM3	0xff
#define CPUFANOUT_DC_PWM4	0xff

//#define SYSFANOUT_ENABLED	1
#define SYSFANOUT_STOP_MODE	1
#define SYSFANOUT_TEMP_SOURCE	0x2
#define SYSFANOUT_CRITICAL_TEMP	0x55
#define SYSFANOUT_CRITICAL_TOLERANCE	0x5
#define SYSFANOUT_T1	0x23
#define SYSFANOUT_T2	0x28
#define SYSFANOUT_T3	0x37
#define SYSFANOUT_T4	0x37
#define SYSFANOUT_T4_TOLERANCE	0x5
#define SYSFANOUT_NONSTOP_PWM	0x28
#define SYSFANOUT_STEPUP_TIME	0x1
#define SYSFANOUT_STEPDOWN_TIME	0x1
#define SYSFANOUT_DC_PWM1	0x28
#define SYSFANOUT_DC_PWM2	0x51
#define SYSFANOUT_DC_PWM3	0xff
#define SYSFANOUT_DC_PWM4	0xff

extern STATUS Plt_NCT6776F_ClearCaseOpenLatch(void);
extern STATUS SetupNCT6776FDrv (DWORD ,DWORD ,void * ,void *);
extern STATUS PryNCT6776FDrv (DWORD ,DWORD ,void * ,void *);
extern STATUS WriteNCT6776FDrv (DWORD ,DWORD ,void * ,void *);
extern STATUS ReadNCT6776FDrv (DWORD ,DWORD ,void * ,void *);
extern STATUS ReadNCT6776F_ME_Drv (DWORD ,DWORD ,void * ,void *);

#ifdef _NCT6776FDRV_C_

St_SensorAPITag ws_St_NCT6776FAPI=
{
	SetupNCT6776FDrv,
	PryNCT6776FDrv,
	WriteNCT6776FDrv,
	ReadNCT6776FDrv,
};
//get CPU/Memory from ME
St_SensorAPITag ws_St_NCT6776F_ME_API=
{
	SetupNCT6776FDrv,
	PryNCT6776FDrv,
	WriteNCT6776FDrv,
	ReadNCT6776F_ME_Drv,
};

#else


extern St_SensorAPITag ws_St_NCT6776FAPI;

#endif 

#endif 


