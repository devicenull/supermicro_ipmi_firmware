
#ifndef _LAN_H_
#define _LAN_H_



#ifdef _LAN_C_
void CHANGE_ULONG_ENDIAN(void *);
void CHANGE_USHORT_ENDIAN(void *);

#else


extern STATUS IPMISocketRecv(BYTE dw_LogChnl);

#endif

#endif  





