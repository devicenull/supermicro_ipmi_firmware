#ifndef _EEPROMSENSOR_H_
#define _EEPROMSENSOR_H_  

#ifdef _EEPROMSENSOR_C_

extern STATUS SetupEEPROMThreshold(
                                    DWORD dw_HWInfo_1,
                                    DWORD dw_HWInfo_2,
                                    void  *p_HWInfoPtr,
                                    void  *p_DataBuf
                                  );
                            
extern STATUS PryEEPROMThreshold(
                                  DWORD dw_HWInfo_1,
                                  DWORD dw_HWInfo_2,
                                  void  *p_HWInfoPtr,
                                  void  *p_DataBuf
                                );
                                
extern STATUS EEPROMThresholdWrite(
                                    DWORD dw_HWInfo_1,
                                    DWORD dw_HWInfo_2,
                                    void  *p_HWInfoPtr,
                                    void  *p_DataBuf
                                  );


extern STATUS EEPROMThresholdRead(
                                   DWORD dw_HWInfo_1,
                                   DWORD dw_HWInfo_2,
                                   void  *p_HWInfoPtr,
                                   void  *p_DataBuf
                                 );

extern STATUS SetupEEPROMDiscrete(
                                   DWORD dw_HWInfo_1,
                                   DWORD dw_HWInfo_2,
                                   void  *p_HWInfoPtr,
                                   void  *p_DataBuf
                                 );
                            
extern STATUS PryEEPROMDiscrete(
                                 DWORD dw_HWInfo_1,
                                 DWORD dw_HWInfo_2,
                                 void  *p_HWInfoPtr,
                                 void  *p_DataBuf
                               );
                                
extern STATUS EEPROMDiscreteWrite(
                                   DWORD dw_HWInfo_1,
                                   DWORD dw_HWInfo_2,
                                   void  *p_HWInfoPtr,
                                   void  *p_DataBuf
                                 );


extern STATUS EEPROMDiscreteRead(
                                  DWORD dw_HWInfo_1,
                                  DWORD dw_HWInfo_2,
                                  void  *p_HWInfoPtr,
                                  void  *p_DataBuf
                                );
                                
St_SensorAPITag ws_St_EEPROMThresholdAPI =
{
    SetupEEPROMThreshold,
    PryEEPROMThreshold,
    EEPROMThresholdWrite,
    EEPROMThresholdRead
};

St_SensorAPITag ws_St_EEPROMDiscreteAPI =
{
    SetupEEPROMDiscrete,
    PryEEPROMDiscrete,
    EEPROMDiscreteWrite,
    EEPROMDiscreteRead
};                                
#else

extern St_SensorAPITag ws_St_EEPROMThresholdAPI;
extern St_SensorAPITag ws_St_EEPROMDiscreteAPI;

#endif  


#endif  





