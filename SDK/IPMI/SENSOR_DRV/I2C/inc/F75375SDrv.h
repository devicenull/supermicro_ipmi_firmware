
     
#ifndef _F75375SDRV_H_
#define _F75375SDRV_H_



#define F75375S_CONFIGURATION_REG       0x00
#define F75375S_MODE_SELECT_REG         0x01
#define F75375S_FAN_FAULT_SELECT_REG    0x02
#define F75375S_VOLT_FAULT_SELECT_REG   0x03

#define F75375S_CONFIGURATION_AMONT     4

typedef struct 
{
	BYTE  b_RegAddr;
	BYTE  b_Value;
} St_F75375SConfigureTypeTag;

extern STATUS SetupF75375SDrv(DWORD , DWORD, void *, void *);
extern STATUS PryF75375SDrv(DWORD , DWORD, void *, void *);
extern STATUS WriteF75375SDrv(DWORD , DWORD, void *, void *);
extern STATUS ReadF75375SDrv(DWORD , DWORD, void *, void *);

#ifdef _F75375SDRV_C_

St_SensorAPITag ws_St_F75375SAPI =
{
    SetupF75375SDrv,
    PryF75375SDrv,
    WriteF75375SDrv,
    ReadF75375SDrv
};

#else

extern St_SensorAPITag ws_St_F75375SAPI;

#endif  
#endif  





