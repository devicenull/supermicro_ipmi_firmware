#ifndef _UTILPOWERSUPPLY_H_
#define _UTILPOWERSUPPLY_H_

#define EACH_EXISTEDPOWERSUPPLY(x,y)  for (x = 0;x < 16 ;x++) \
										if ( y & (1 << i))
#define POWER_BEHINED_ME              0x80000000
#define ISEXISTED_MEPOWER(x)		  (POWER_BEHINED_ME & x)


#ifdef  _UTILPOWERSUPPLY_C_
DWORD   dw_Add_PowerBitmap = 0;
extern  WORD ApiDoingAddSDRCmd (MsgBufFmt *, MsgBufFmt *);
extern  WORD ApiDoingDeleteSDRCmd (MsgBufFmt *, MsgBufFmt *);
extern  St_SensorCallBackAPITag at_St_PowerFailCallBackAPI;
extern  STATUS (*PltPWR_ReadCallback)(DWORD ,DWORD ,void  *,void  *);
extern  BYTE GetPSIndexBySensorNum(BYTE b_SensorNum);
extern  int GetPSIndexByAddr(BYTE slave_address, BYTE *PSIndex);
typedef STATUS (*POWERCALLBACK)(WORD,WORD,BYTE,BYTE);

/* Tony, 10/23/2012 add battery backup power { */
extern  BYTE GetBBPIndexBySensorNum(BYTE b_SensorNum);
extern DWORD GetBBPRegister(BYTE b_BBPIndex,BYTE Offset,BYTE RefreshAll);
/* Tony, 10/23/2012 add battery backup power } */
/* Tony, 01/03/2013 Add power source info { */
extern void GetPSInfo(BYTE PSIndex);
/* Tony, 01/03/2013 Add power source info } */
#else

extern DWORD   dw_Add_PowerBitmap ;
#endif
#endif
