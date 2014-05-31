      
#ifndef _GPIO_SENSOR_H_
#define _GPIO_SENSOR_H_


extern STATUS SetupGPIOSnr(DWORD , DWORD, void *, void *);
extern STATUS PryGPIOSnr(DWORD , DWORD, void *, void *);
extern STATUS WriteGPIOSnr(DWORD , DWORD, void *, void *);
extern STATUS ReadGPIOSnr(DWORD , DWORD, void *, void *);

#ifdef _GPIO_SENSOR_C_

St_SensorAPITag at_St_GPIOSnrAPI =
{
	SetupGPIOSnr,
	PryGPIOSnr,
	WriteGPIOSnr,
	ReadGPIOSnr
};

extern STATUS PltGPIO_Setup(DWORD , DWORD, void *, void *);
extern STATUS PltGPIO_Pry(DWORD , DWORD, void *, void *);
extern STATUS PltGPIO_Write(DWORD , DWORD, void *, void *);
extern STATUS PltGPIO_Read(DWORD , DWORD, void *, void *);

#else

extern St_SensorAPITag at_St_GPIOSnrAPI;

#endif  
#endif  





