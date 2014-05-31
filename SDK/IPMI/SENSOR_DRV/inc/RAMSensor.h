 
#ifndef _RAMSENSOR_H_
#define _RAMSENSOR_H_  

#ifdef _RAMSENSOR_C_

extern STATUS SetupRAMThreshold(
                                 DWORD dw_HWInfo_1,
                                 DWORD dw_HWInfo_2,
                                 void  *p_HWInfoPtr,
                                 void  *p_DataBuf
                               );
                            
extern STATUS PryRAMThreshold(
                               DWORD dw_HWInfo_1,
                               DWORD dw_HWInfo_2,
                               void  *p_HWInfoPtr,
                               void  *p_DataBuf
                             );
                                
extern STATUS RAMThresholdWrite(
                                 DWORD dw_HWInfo_1,
                                 DWORD dw_HWInfo_2,
                                 void  *p_HWInfoPtr,
                                 void  *p_DataBuf
                               );


extern STATUS RAMThresholdRead(
                                DWORD dw_HWInfo_1,
                                DWORD dw_HWInfo_2,
                                void  *p_HWInfoPtr,
                                void  *p_DataBuf
                              );

extern STATUS SetupRAMDiscrete(
                                DWORD dw_HWInfo_1,
                                DWORD dw_HWInfo_2,
                                void  *p_HWInfoPtr,
                                void  *p_DataBuf
                              );
                            
extern STATUS PryRAMDiscrete(
                              DWORD dw_HWInfo_1,
                              DWORD dw_HWInfo_2,
                              void  *p_HWInfoPtr,
                              void  *p_DataBuf
                            );
                                
extern STATUS RAMDiscreteWrite(
                                DWORD dw_HWInfo_1,
                                DWORD dw_HWInfo_2,
                                void  *p_HWInfoPtr,
                                void  *p_DataBuf
                              );


extern STATUS RAMDiscreteRead(
                               DWORD dw_HWInfo_1,
                               DWORD dw_HWInfo_2,
                               void  *p_HWInfoPtr,
                               void  *p_DataBuf
                             );

St_SensorAPITag ws_St_RAMThresholdAPI =
{
    SetupRAMThreshold,
    PryRAMThreshold,
    RAMThresholdWrite,
    RAMThresholdRead
};

St_SensorAPITag ws_St_RAMDiscreteAPI =
{
    SetupRAMDiscrete,
    PryRAMDiscrete,
    RAMDiscreteWrite,
    RAMDiscreteRead
};
#else

extern St_SensorAPITag ws_St_RAMThresholdAPI;
extern St_SensorAPITag ws_St_RAMDiscreteAPI;
#endif  


#endif  





