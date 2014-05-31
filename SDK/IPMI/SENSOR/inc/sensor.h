
#ifndef _SENSOR_H_
#define _SENSOR_H_
#define OWN_WBEM_SERVER 
#define MAX_SENSOR_NUM	256


#ifdef OWN_GLOBE_BMC
#define SENSOR_PROC_STACK_SIZE    1024
#else
#define SENSOR_PROC_STACK_SIZE    768
#endif


#ifdef OEM_CALLBACK_FUNCTION
#endif

typedef enum 
{
	AT_OR_BELOW_LOWER_NON_CRITICAL=0xc1,
	AT_OR_BELOW_LOWER_CRITICAL=0xc2,
	AT_OR_BELOW_LOWER_NON_RECOVERABLE=0xc4,
	AT_OR_ABOVE_UPPER_NON_CRITICAL=0xc8,
	AT_OR_ABOVE_UPPER_CRITICAL=0xd0,
	AT_OR_ABOVE_UPPER_NON_RECOVERABLE=0xe0,
	AT_SENSOR_NORMAL=0xc0,
	ERROR_SENSOR_STATUS=0
} En_ANALOG_SENSOR_STATUSTag;

typedef struct
{
    BYTE  analog_characteristic_flags;
    BYTE  nominal_reading;
    BYTE  normal_maximum;
    BYTE  normal_minimum;
    BYTE  upper_non_recoverable_threshold;
    BYTE  upper_critical_threshold;
    BYTE  upper_non_critical_threshold;
    BYTE  lower_non_recoverable_threshold;
    BYTE  lower_critical_threshold;
    BYTE  lower_non_critical_threshold;
    BYTE  sensor_reading;	
    BYTE  old_sensor_reading;
    WORD  assertion_status;
	WORD  deassertion_status;
	unsigned long int sensor_state_count;
	BYTE pre_stable_reading;
	En_ANALOG_SENSOR_STATUSTag  sensor_status;
	En_ANALOG_SENSOR_STATUSTag  old_sensor_status;
} St_ANALOG_SENSOR_DATATag;

typedef struct
{
	WORD  disc_manual_assertion_event_status;
	WORD  disc_manual_deassertion_event_status;
	BYTE  disc_last_event_issued;
	BYTE  disc_sensor_reading_LSB;
    BYTE  disc_sensor_reading_MSB;
    BYTE  old_disc_sensor_reading_LSB;
    BYTE  old_disc_sensor_reading_MSB;
    BYTE  volatile_old_disc_sensor_reading_LSB;
    BYTE  volatile_old_disc_sensor_reading_MSB; 
    unsigned long int sensor_state_count;
} St_DISC_SENSOR_DATATag;


typedef struct {
    BYTE  sensor_type;
	BYTE  sensor_index;
	BYTE  sensor_owner_id;
	BYTE  sensor_owner_lun;
	BYTE  sensor_number;
	BYTE  sensor_power;
    BYTE  entity_id;
    BYTE  entity_instance;
    BYTE  sensor_initialization;
    BYTE  sensor_capabilities;
    BYTE  event_slash_reading_type_code;
    WORD  assertion_event_mask_lower_threshold_reading_mask;	
    WORD  deassertion_event_mask_upper_threshold_reading_mask;
    WORD  disc_reading_mask_settable_thres_mask_readable_thres_mask;
	WORD  assertion_event_status;
	WORD  deassertion_event_status;
    BYTE  sensor_unit1;
    BYTE  sensor_unit2;
    BYTE  sensor_unit3;
    BYTE  positive_going_threshold_hysteresis_value;
    BYTE  negative_going_threshold_hysteresis_value;
    BYTE  scan_period;
    BYTE  scan_tick;
	BYTE  set_event_enable_assertion_mask_LSB;
	BYTE  set_event_enable_assertion_mask_MSB;
	BYTE  set_event_enable_deassertion_mask_LSB;
	BYTE  set_event_enable_deassertion_mask_MSB;
	BYTE  set_event_enable_status;
	En_ANALOG_SENSOR_STATUSTag  deassertion_status;

	union
	{
		St_ANALOG_SENSOR_DATATag analog_sensor;
		St_DISC_SENSOR_DATATag disc_sensor;
	}sensor;
#ifdef OWN_WBEM_SERVER
	unsigned short  SDR_RecordID;
	BYTE  SDR_RecordType;
	BYTE  SDR_OwnerID;
	BYTE  SDR_OwnerLUN;	
	BYTE  SDR_EntityID;
	BYTE  SDR_EntityInstanceNumber;
	BYTE  SDR_SensorInitilization;
	BYTE  SDR_EventReadingTypeCode;
#endif
	BYTE  SDR_IDString[16];
#ifdef OWN_HTTP_SERVER
	BYTE  RecordType;
	BYTE  Base_Unit;
	BYTE  Linearization;
	BYTE  M;
	BYTE  MTolerance;
	BYTE  B;
	BYTE  BAccuracy;
	BYTE  Rexp;
	BYTE  IDString[16];
	BYTE  UNC_Value;
	BYTE  LNC_Value;
#endif
#ifdef OWN_AST_WEB_SERVER
	WORD  RecordID;
	BYTE  RecordType;
	BYTE  Base_Unit;
	BYTE  Linearization;
	BYTE  M;
	BYTE  MTolerance;
	BYTE  B;
	BYTE  BAccuracy;
	BYTE  Rexp;
	BYTE  IDString[16];
	BYTE  UNC_Value;
	BYTE  LNC_Value;
	BYTE  RecordLength;
#endif	
    WORD w_offset;
    
} St_SensorDataTag;




typedef enum 
{
	LOWER_NON_CRITICAL_GOING_LOW=0,
	LOWER_NON_CRITICAL_GOING_HIGH=1,
	LOWER_CRITICAL_GOING_LOW=2,
	LOWER_CRITICAL_GOING_HIGH=3,
	LOWER_NON_RECOVERABLE_GOING_LOW=4,
	LOWER_NON_RECOVERABLE_GOING_HIGH=5,
	UPPER_NON_CRITICAL_GOING_LOW=6,
	UPPER_NON_CRITICAL_GOING_HIGH=7,
	UPPER_CRITICAL_GOING_LOW=8,
	UPPER_CRITICAL_GOING_HIGH=9,
	UPPER_NON_RECOVERABLE_GOING_LOW=0xa,
	UPPER_NON_RECOVERABLE_GOING_HIGH=0xb
} En_ANALOG_EVENT_CODETag;


#ifdef _SENSOR_C_


void update_anal_sensor_status(
                                  St_SensorDataTag * ps_inputdata,
                                  BYTE b_pollreading,
                                  signed char *s_UNR,signed char *s_UC,signed char *s_UNC, 
								  signed char *s_LNR,signed char *s_LC,signed char *s_LNC
                              );
						      
void update_disc_sensor_status(
						          St_SensorDataTag * s_inputdata,
						          BYTE * b_sensor_reading
						      );

void update_disc_generic_status(
                                  St_SensorDataTag * s_inputdata,
						          BYTE * b_sensor_reading
						      );

En_ANALOG_SENSOR_STATUSTag analog_sensor_comparison(
                                               St_SensorDataTag * ps_inputdata,
                                               BYTE b_reading,
                                               signed char *s_UNR,signed char *s_UC,signed char *s_UNC, 
											   signed char *s_LNR,signed char *s_LC,signed char *s_LNC
                                             );
                                             
void update_anal_sensor_event_status(St_SensorDataTag * ps_inputdata,
						signed char *s_UNR,signed char *s_UC,signed char *s_UNC, 
						signed char *s_LNR,signed char *s_LC,signed char *s_LNC);

void update_disc_sensor_event_status(BYTE SenorType, BYTE Offset, void *p_EventdataPtr);

void discassertion (
                       St_SensorDataTag * ps_inputdata,
                                 BYTE event_data1,
                                 BYTE event_data2,
                                       WORD match,
                                   BYTE direction
                   );
void discdeassert (
                      St_SensorDataTag * ps_inputdata,
                      BYTE event_data1,
                      WORD match,
                      BYTE direction
                  );                   
void SensorProcInit(void);
void SensorTask(unsigned long int dummy);
void sensor_init(void);
BYTE get_total_sensor_number();


void check_sensor_event(St_SensorDataTag *s_inputdata);
void assertevent(
                     St_SensorDataTag *s_inputdata,
                     En_ANALOG_EVENT_CODETag en_event_assert
                 );
void deassertevent(
                       St_SensorDataTag *s_inputdata,
                       En_ANALOG_EVENT_CODETag en_event_deassert
                  );

BYTE Set_Sensor_Hysteresis(
						             BYTE sensor_num,
						             BYTE positive_hysteresis,
							         BYTE negative_hysteresis
						           );
BYTE Get_Sensor_Hysteresis(
						             BYTE sensor_num,
						             BYTE * positive_hysteresis,
							         BYTE * negative_hysteresis
						           );
BYTE Set_Sensor_Threshold(
						            BYTE sensor_num,
						            BYTE threshold_mask,
						            BYTE LNC,
						            BYTE LC,
						            BYTE LNR,
						            BYTE UNC,
						            BYTE UC,
						            BYTE UNR
						          );
BYTE Get_Sensor_Threshold(
						            BYTE sensor_num,
						            BYTE * threshold_mask,
						            BYTE * LNC,
						            BYTE * LC,
						            BYTE * LNR,
						            BYTE * UNC,
						            BYTE * UC,
						            BYTE * UNR
						          );
BYTE Set_Sensor_Event_Enable( 
							           BYTE sensor_num,
							           BYTE event_options,
							           BYTE assertion_LSB,
							           BYTE assertion_MSB,
							           BYTE deassertion_LSB,
							           BYTE deassertion_MSB
						             );
BYTE Get_Sensor_Event_Enable(
		                               BYTE sensor_num,
							           BYTE * event_options,
						               BYTE * assertion_LSB,
							           BYTE * assertion_MSB,
							           BYTE * deassertion_LSB,
							           BYTE * deassertion_MSB
						             );

BYTE Get_Sensor_Event_Status(
                                       BYTE sensor_num,
                                       BYTE * sensor_stat,
                                       BYTE * assertion_LSB,
                                       BYTE * assertion_MSB,
                                       BYTE * deassertion_LSB,
                                       BYTE * deassertion_MSB
                                     );
BYTE Get_Sensor_Reading(
                                  BYTE sensor_num,
                                  BYTE *sensor_reading,
                                  BYTE *reading_option,
                                  BYTE *analog_discrete_LSB,
                                  BYTE *discrete_MSB,
                                  BYTE *sensor_type
                                );
BYTE OEM_Set_Sensor_Reading(
                                      BYTE sensor_num,
									  BYTE sensor_reading
									);

signed char Unsigned2Signed(BYTE b_us_value, BYTE b_cmp_type);									

void UnsignedThreshold2Signed(St_SensorDataTag *ps_inputdata, 
		signed char *s_UNR,signed char *s_UC,signed char *s_UNC, 
		signed char *s_LNR,signed char *s_LC,signed char *s_LNC
);
									
									
BOOLEAN CheckMBPowerStatus(BYTE b_mb_status);
#ifdef CONFIG_PLATFORM_ADLINK_CMM
BYTE OEM_Get_SEL_Enable(BYTE sensorType);
#endif
#else


extern BYTE Set_Sensor_Hysteresis(
						             BYTE sensor_num,
						             BYTE positive_hysteresis,
							         BYTE negative_hysteresis
						           );
extern BYTE Get_Sensor_Hysteresis(
						             BYTE sensor_num,
						             BYTE * positive_hysteresis,
							         BYTE * negative_hysteresis
						           );
extern BYTE Set_Sensor_Threshold(
						            BYTE sensor_num,
						            BYTE threshold_mask,
						            BYTE LNC,
						            BYTE LC,
						            BYTE LNR,
						            BYTE UNC,
						            BYTE UC,
						            BYTE UNR
						          );
extern BYTE Get_Sensor_Threshold(
						            BYTE sensor_num,
						            BYTE * threshold_mask,
						            BYTE * LNC,
						            BYTE * LC,
						            BYTE * LNR,
						            BYTE * UNC,
						            BYTE * UC,
						            BYTE * UNR
						          );
extern BYTE Set_Sensor_Event_Enable( 
							           BYTE sensor_num,
							           BYTE event_options,
							           BYTE assertion_LSB,
							           BYTE assertion_MSB,
							           BYTE deassertion_LSB,
							           BYTE deassertion_MSB
						             );
extern BYTE Get_Sensor_Event_Enable(
		                               BYTE sensor_num,
							           BYTE * event_options,
						               BYTE * assertion_LSB,
							           BYTE * assertion_MSB,
							           BYTE * deassertion_LSB,
							           BYTE * deassertion_MSB
						             );

extern BYTE Get_Sensor_Event_Status(
                                       BYTE sensor_num,
                                       BYTE * sensor_stat,
                                       BYTE * assertion_LSB,
                                       BYTE * assertion_MSB,
                                       BYTE * deassertion_LSB,
                                       BYTE * deassertion_MSB
                                     );
extern BYTE Get_Sensor_Reading(
                                  BYTE sensor_num,
                                  BYTE *sensor_reading,
                                  BYTE *reading_option,
                                  BYTE *analog_discrete_LSB,
                                  BYTE *discrete_MSB,
                                  BYTE *sensor_type
                                );
extern BYTE OEM_Set_Sensor_Reading(
                                      BYTE sensor_num,
									  BYTE sensor_reading
									);

extern void SensorProcInit(void);								

#endif
#endif



