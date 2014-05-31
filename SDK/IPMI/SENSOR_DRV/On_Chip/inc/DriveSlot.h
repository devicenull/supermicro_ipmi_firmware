
      
#ifndef _DRIVE_SLOT_H_
#define _DRIVE_SLOT_H_

extern STATUS SetupDriveSlotDrv(DWORD , DWORD, void *, void *);
extern STATUS PryDriveSlotDrv(DWORD , DWORD, void *, void *);
extern STATUS WriteDriveSlotDrv(DWORD , DWORD, void *, void *);
extern STATUS ReadDriveSlotDrv(DWORD , DWORD, void *, void *);

#ifdef _DRIVE_SLOT_C_


St_SensorAPITag ws_St_DriveSlotAPI =
{
    SetupDriveSlotDrv,
    PryDriveSlotDrv,
    WriteDriveSlotDrv,
    ReadDriveSlotDrv
};

#else

extern St_SensorAPITag ws_St_DriveSlotAPI;

#endif  
#endif  





