

#ifndef _W83601RDRV_H_
#define _W83601RDRV_H_

#define W83601R_CONFIGURATION_AMONT             2

#define W83601R_CONFIGURATION_GP1               0x03
#define W83601R_CONFIGURATION_GP2               0x0B
#define W83601R_INPUT_MODE_GP1                  0xFF
#define W83601R_INPUT_MODE_GP2                  0x7F
                                                
#define W83601R_INVERSION_GP1                   0x02
#define W83601R_INVERSION_GP2                   0x0A
#define W83601R_OUTPUT_REG1                     0x01
#define W83601R_OUTPUT_REG2                     0x09
 

typedef struct 
{
	BYTE  b_RegAddr;
	BYTE  b_Value;
} St_W83601RConfigureTypeTag;

extern STATUS SetupW83601RDrv(DWORD, DWORD, void *, void *);
extern STATUS PryW83601RDrv(DWORD, DWORD, void *, void *);
extern STATUS WriteW83601RDrv(DWORD, DWORD, void *, void *);
extern STATUS ReadW83601RDrv(DWORD, DWORD, void *, void *);

#ifdef _W83601RDRV_C_

St_SensorAPITag ws_St_W83601RAPI =
{
	SetupW83601RDrv,
	PryW83601RDrv,
	WriteW83601RDrv,
	ReadW83601RDrv,
};

#else


extern St_SensorAPITag ws_St_W83601RAPI;

#endif 

#endif 


