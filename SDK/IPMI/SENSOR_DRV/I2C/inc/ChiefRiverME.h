#ifndef _CHIEF_RIVER_ME_H
#define _CHIEF_RIVER_ME_H


extern STATUS SetupChiefRiverME(DWORD , DWORD, void *, void *);
extern STATUS PryChiefRiverME(DWORD , DWORD, void *, void *);
extern STATUS WriteChiefRiverME(DWORD , DWORD, void *, void *);
extern STATUS ReadChiefRiverME(DWORD , DWORD, void *, void *);


/* struct ChiefRiverME API */
#ifdef _CHIEF_RIVER_ME_H
St_SensorAPITag at_St_ChiefRiverME_API =
{
    SetupChiefRiverME,
    PryChiefRiverME,
    WriteChiefRiverME,
    ReadChiefRiverME
};

#else

extern St_SensorAPITag at_St_ChiefRiverME_AP;

#endif 


#endif //End of _CHIEF_RIVER_ME_H
