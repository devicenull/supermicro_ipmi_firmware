
#ifndef _I2CMASTERWR_H_
#define _I2CMASTERWR_H_  


#ifdef _I2CMASTERWR_C_

#else

extern CMD_STATUS I2CMasterWRCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern BYTE ApiI2CMasterWRSubFnCheck(MsgBufFmt *p_St_MsgBufIn);

#endif   

#endif  





