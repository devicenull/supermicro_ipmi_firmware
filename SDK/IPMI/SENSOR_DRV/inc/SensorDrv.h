
#ifndef _SENSORDRV_H_
#define _SENSORDRV_H_


extern STATUS SetupSensorAPI(BYTE, BYTE, void *, void *);
extern STATUS PrySensorAPI(BYTE, BYTE, void *, void *);
extern STATUS WriteSensorAPI(BYTE, BYTE, void *, void *);
extern STATUS ReadSensorAPI(BYTE, BYTE, void *, void *);
extern STATUS SensorCallBackFunction(BYTE, void *, void *);

#ifdef OEM_CALLBACK_FUNCTION
extern STATUS OEM_CALLBACK_FUNCTION_SetupSensorAPI(BYTE, BYTE, void *, void *);
extern STATUS OEM_CALLBACK_FUNCTION_PrySensorAPI(BYTE, BYTE, void *, void *);
extern STATUS OEM_CALLBACK_FUNCTION_WriteSensorAPI(BYTE, BYTE, BYTE *, void *);
extern STATUS OEM_CALLBACK_FUNCTION_ReadSensorAPI(BYTE, BYTE, void *, void *);
#endif

#endif 





