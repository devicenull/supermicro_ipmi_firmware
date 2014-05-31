
#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#define SDR_IDSTRING_LENGTH		16
typedef struct {
	unsigned short  SDR_RecordID;
	unsigned char   SDR_RecordType;
	unsigned char   SDR_OwnerID;
	unsigned char   SDR_OwnerLUN;	
	unsigned char   SDR_EntityID;
	unsigned char   SDR_EntityInstanceNumber;
	unsigned char   SDR_SensorInitilization;
	unsigned char   SDR_EventReadingTypeCode;
	unsigned char   SDR_IDString[SDR_IDSTRING_LENGTH];
}sdr_info_tag;
typedef enum 
{
	En_Normal = 1,
	En_Warning,
	En_Critical
} En_SystemHealthTag;

#define EACH_SENSORS(x)  for  (x = &at_St_SensorData[0]; x < &at_St_SensorData[at_St_InitAgentResult.b_sensor_data_num ];x ++) 


#ifdef _UTILSENSOR_C_
int SENSORMTX_LOCK(void);
int SENSORMTX_UNLOCK(void);

void SensorTimer(unsigned long dummy);

STATUS InitAgentInit();
STATUS Utilshmat();
unsigned char UtilGetSensorEventStatus(
                                       unsigned char sensor_num,
                                       unsigned char * sensor_stat,
                                       unsigned char * assertion_LSB,
                                       unsigned char * assertion_MSB,
                                       unsigned char * deassertion_LSB,
                                       unsigned char * deassertion_MSB
                                     );

unsigned char UtilGetSensorReading(
                                  unsigned char sensor_num,
                                  unsigned char *sensor_reading,
                                  unsigned char *reading_option,
                                  unsigned char *analog_discrete_LSB,
                                  unsigned char *discrete_MSB,
                                  unsigned char *sensor_type
                                );

unsigned char UtilGetHumanReading(
                                  unsigned char sensor_num,
                                  unsigned char *sensor_reading,
                                  unsigned char *reading_option,
                                  unsigned char *analog_discrete_LSB,
                                  unsigned char *discrete_MSB,
                                  unsigned char *sensor_type,
                                  float *human_reading
                                );


unsigned char UtilGetSensorEventEnable(
                                       unsigned char sensor_num,
                                       unsigned char * event_options,
                                       unsigned char * assertion_LSB,
                                       unsigned char * assertion_MSB,
                                       unsigned char * deassertion_LSB,
                                       unsigned char * deassertion_MSB
                                     );

unsigned char UtilGetSensorEventStatus(
                                       unsigned char sensor_num,
                                       unsigned char * sensor_stat,
                                       unsigned char * assertion_LSB,
                                       unsigned char * assertion_MSB,
                                       unsigned char * deassertion_LSB,
                                       unsigned char * deassertion_MSB
                                     );

unsigned char UtilGetSensorHysteresis(
                                     unsigned char sensor_num,
                                     unsigned char * positive_hysteresis,
                                     unsigned char * negative_hysteresis
                                   );

unsigned char UtilGetSensorReading(
                                  unsigned char sensor_num,
                                  unsigned char *sensor_reading,
                                  unsigned char *reading_option,
                                  unsigned char *analog_discrete_LSB,
                                  unsigned char *discrete_MSB,
                                  unsigned char *sensor_type
                                );

unsigned char UtilGetHumanReading(
                                  unsigned char sensor_num,
                                  unsigned char *sensor_reading,
                                  unsigned char *reading_option,
                                  unsigned char *analog_discrete_LSB,
                                  unsigned char *discrete_MSB,
                                  unsigned char *sensor_type,
                                  float *human_reading
                                );                                
                                
unsigned char UtilGetSensorThreshold(
                                    unsigned char sensor_num,
                                    unsigned char * threshold_mask,
                                    unsigned char * LNC,
                                    unsigned char * LC,
                                    unsigned char * LNR,
                                    unsigned char * UNC,
                                    unsigned char * UC,
                                    unsigned char * UNR
                                  );
                                  
unsigned char UtilRearm_All_Sensors(void);
unsigned char UtilRearmSensorEvents(
                                   unsigned char sensor_num,
                                   unsigned char rearm_options,
                                   unsigned char assertion_LSB,
                                   unsigned char assertion_MSB,
                                   unsigned char deassertion_LSB,
                                   unsigned char deassertion_MSB
                                 );
                                 
unsigned char UtilSetSensorEventEnable(
                                       unsigned char sensor_num,
                                       unsigned char event_options,
                                       unsigned char assertion_LSB,
                                       unsigned char assertion_MSB,
                                       unsigned char deassertion_LSB,
                                       unsigned char deassertion_MSB
                                     );
                                     
unsigned char UtilSetSensorHysteresis(
                                     unsigned char sensor_num,
                                     unsigned char positive_hysteresis,
                                     unsigned char negative_hysteresis
                                   );
                                   
unsigned char UtilSetSensorThreshold(
                                    unsigned char sensor_num,
                                    unsigned char threshold_mask,
                                    unsigned char LNC,
                                    unsigned char LC,
                                    unsigned char LNR,
                                    unsigned char UNC,
                                    unsigned char UC,
                                    unsigned char UNR,
                                    unsigned char u8Flag
                                  );                                                                                                                                                                                                                                                                                        

at_s_INIT_AGENT_RESULT Sensor_Init_Agent(BOOLEAN run_init_agent);
BYTE UtilCheckSELSensorEventMask(void);
int UtilGetSDRInfo(unsigned char b_sensor_num,sdr_info_tag *sdr_info);
int UtilGetSensorType(unsigned char b_sensor_num,unsigned char *sn_type_code,
									unsigned char *evt_reading_type);
int UtilGetNumberOfSensor();
int UtilGetSensorNumber(int b_sensor_data_num);
int UtilIsDiscreteSensor(unsigned char sensor_type);
void UtilIsSensorPresent(unsigned char sn_num,unsigned char *sn_enable,
							unsigned char *sn_present);
#else
#ifdef __cplusplus
extern "C"{
#endif
extern int SENSORMTX_LOCK(void);
extern int SENSORMTX_UNLOCK(void);
extern void SensorTimer(unsigned long dummy);
extern STATUS InitAgentInit();
extern STATUS Utilshmat();

extern unsigned char UtilGetSensorEventStatus(
                                       unsigned char sensor_num,
                                       unsigned char * sensor_stat,
                                       unsigned char * assertion_LSB,
                                       unsigned char * assertion_MSB,
                                       unsigned char * deassertion_LSB,
                                       unsigned char * deassertion_MSB
                                     );

extern unsigned char UtilGetSensorReading(
                                  unsigned char sensor_num,
                                  unsigned char *sensor_reading,
                                  unsigned char *reading_option,
                                  unsigned char *analog_discrete_LSB,
                                  unsigned char *discrete_MSB,
                                  unsigned char *sensor_type
                                );

extern unsigned char UtilGetHumanReading(
                                  unsigned char sensor_num,
                                  unsigned char *sensor_reading,
                                  unsigned char *reading_option,
                                  unsigned char *analog_discrete_LSB,
                                  unsigned char *discrete_MSB,
                                  unsigned char *sensor_type,
                                  float *human_reading
                                );

extern unsigned char UtilGetSensorEventEnable(
                                       unsigned char sensor_num,
                                       unsigned char * event_options,
                                       unsigned char * assertion_LSB,
                                       unsigned char * assertion_MSB,
                                       unsigned char * deassertion_LSB,
                                       unsigned char * deassertion_MSB
                                     );

extern unsigned char UtilGetSensorEventStatus(
                                       unsigned char sensor_num,
                                       unsigned char * sensor_stat,
                                       unsigned char * assertion_LSB,
                                       unsigned char * assertion_MSB,
                                       unsigned char * deassertion_LSB,
                                       unsigned char * deassertion_MSB
                                     );

extern unsigned char UtilGetSensorHysteresis(
                                     unsigned char sensor_num,
                                     unsigned char * positive_hysteresis,
                                     unsigned char * negative_hysteresis
                                   );

extern unsigned char UtilGetSensorReading(
                                  unsigned char sensor_num,
                                  unsigned char *sensor_reading,
                                  unsigned char *reading_option,
                                  unsigned char *analog_discrete_LSB,
                                  unsigned char *discrete_MSB,
                                  unsigned char *sensor_type
                                );
                                
extern unsigned char UtilGetSensorThreshold(
                                    unsigned char sensor_num,
                                    unsigned char * threshold_mask,
                                    unsigned char * LNC,
                                    unsigned char * LC,
                                    unsigned char * LNR,
                                    unsigned char * UNC,
                                    unsigned char * UC,
                                    unsigned char * UNR
                                  );
                                  
extern unsigned char UtilRearm_All_Sensors(void);
extern unsigned char UtilRearmSensorEvents(
                                   unsigned char sensor_num,
                                   unsigned char rearm_options,
                                   unsigned char assertion_LSB,
                                   unsigned char assertion_MSB,
                                   unsigned char deassertion_LSB,
                                   unsigned char deassertion_MSB
                                 );
                                 
extern unsigned char UtilSetSensorEventEnable(
                                       unsigned char sensor_num,
                                       unsigned char event_options,
                                       unsigned char assertion_LSB,
                                       unsigned char assertion_MSB,
                                       unsigned char deassertion_LSB,
                                       unsigned char deassertion_MSB
                                     );
                                     
extern unsigned char UtilSetSensorHysteresis(
                                     unsigned char sensor_num,
                                     unsigned char positive_hysteresis,
                                     unsigned char negative_hysteresis
                                   );
                                   
extern unsigned char UtilSetSensorThreshold(
                                    unsigned char sensor_num,
                                    unsigned char threshold_mask,
                                    unsigned char LNC,
                                    unsigned char LC,
                                    unsigned char LNR,
                                    unsigned char UNC,
                                    unsigned char UC,
                                    unsigned char UNR,
                                    unsigned char u8Flag
                                  );                                                                                                                                                                                                                                                                                        

                                  
                                                                                                                                                                                                                                                                                                                  extern En_ANALOG_SENSOR_STATUSTag analog_sensor_comparison(
                                               St_SensorDataTag * ps_inputdata,
                                               BYTE b_reading,
                                               signed char *s_UNR, signed char *s_UC, signed char *s_UNC, 
											   signed char *s_LNR, signed char *s_LC, signed char *s_LNC
                                             );
extern void UnsignedThreshold2Signed(St_SensorDataTag *ps_inputdata, 
		signed char *s_UNR, signed char *s_UC, signed char *s_UNC, 
		signed char *s_LNR, signed char *s_LC, signed char *s_LNC
);

extern signed char Unsigned2Signed(BYTE b_us_value, BYTE b_cmp_type);
extern WORD GetSesnorReadingByNumber(BYTE );
extern En_SystemHealthTag GetSystemHealth();
extern at_s_INIT_AGENT_RESULT Sensor_Init_Agent(BOOLEAN run_init_agent);
extern BYTE UtilCheckSELSensorEventMask(void);
extern St_SensorDataTag		*at_p_St_SELSensorData;


extern int UtilGetSDRInfo(unsigned char b_sensor_num,sdr_info_tag *sdr_info);
extern int UtilGetSensorType(unsigned char b_sensor_num,unsigned char *sn_type_code,
							unsigned char *evt_reading_type);
extern void UtilIsSensorPresent(unsigned char sn_num,unsigned char *sn_enable,
							unsigned char *sn_present);
extern int UtilGetNumberOfSensor();
extern int UtilIsDiscreteSensor(unsigned char sensor_type);
extern int UtilGetSensorNumber(int b_sensor_data_num);
#ifdef __cplusplus
}
#endif


#endif
#endif


