
#ifndef _SENSOR_TYPE_H_
#define _SENSOR_TYPE_H_  

#define TEMPERATURE_SENSOR                                          0x01
#define ADC_SENSOR                                                  0x02
#define CURRENT_SENSOR                                              0x03
#define FAN_SENSOR                                                  0x04
#define GPIO_SENSOR                                                 0x05
#define GENERAL_CHASSIS_INTRUSION_SENSOR                            0x05
#define MEM_SENSOR_SPD0												0x06	//Linda
#define MEM_SENSOR_SPD1												0x07	//Linda
#define MEM_SENSOR_SPD2												0x09	//Linda
#define MEM_SENSOR_SPD3												0x0a	//Linda

#define GPU1_TEMP_SENSOR											0x0b   // [Farida] ->
#define GPU2_TEMP_SENSOR											0x0c   
#define GPU3_TEMP_SENSOR											0x0d   
#define GPU4_TEMP_SENSOR											0x0e   // <- [Farida]
#define GPU5_TEMP_SENSOR											0x0F   
#define GPU6_TEMP_SENSOR											0x10
#define GPU7_TEMP_SENSOR                      0x11
#define GPU8_TEMP_SENSOR                      0x12
#define XEON_PHI1_TEMP_SENSOR											0x21
#define XEON_PHI2_TEMP_SENSOR											0x22   
#define XEON_PHI3_TEMP_SENSOR											0x23   
#define XEON_PHI4_TEMP_SENSOR											0x24
#define XEON_PHI5_TEMP_SENSOR											0x25   
#define XEON_PHI6_TEMP_SENSOR											0x26
#define XEON_PHI7_TEMP_SENSOR											0x27
#define XEON_PHI8_TEMP_SENSOR											0x28
#define XEON_PHI9_TEMP_SENSOR											0x29
#define XEON_PHI10_TEMP_SENSOR											0x2a

#define POWER_SUPPY_SENSOR                                          0x08
#define DRIVE_SLOT													0x0D
#define EVENT_LOGGING_DISABLED                                      0x10
#define DRIVE_BAY_INTRUSION                                         0x80
#define IO_CARD_AREA_INTRUSION                                      0x81
#define PROCESSOR_AREA_INTRUSION                                    0x82
#define LAN_LEASH_LOST                                              0x83
#define UNAUTHORIZED_DOCK_UNDOCK                                    0x84
#define FAN_AREA_INTRUSION                                          0x85
#define POWER_CONSUMPTON											0xC1

#define GET_MUX_INFO_MASK                                           0xF0000000
#define GET_PHYSICAL_BUS_NO_MASK                                    0x0F000000
#define GET_DEVICE_ADDRESS_MASK                                     0x00FF0000
#define GET_DATA_ADDRESS_MASK                                       0x0000FF00
#define GET_SENSOR_TYPE_MASK                                        0x000000FF
#define GET_PWM_CHANNEL_NO_MASK                                     0x00FF0000
#define GET_SOFTWARE_OFFSET_MASK                                    0xFF000000
#define GET_OPEN_DEFINE_VALUE_MASK                                  0x00200000
#define GET_SENSOR_MODULE_NUMBER_MASK								0x00FF0000
#define GET_AUTO_FAN_ENABLE_MASK                                    0x00002000
#define GET_FORMULA_TYPE_MASK                                       0x00001000
#define GET_TEMPERATURE_MSB_MASK                                    0xBF 
#define GET_TEMPERATURE_LSB_MASK                                    0x80
#define GET_MSB_DATA_ADDRESS_MASK                                   0x0FF00000
#define GET_LSB_DATA_ADDRESS_MASK                                   0x000FF000
#define GET_DIRECTION_MASK                                          0x40000000
#define GET_INIT_VALUE_MASK                                         0x20000000
#define GET_ON_VALUE_MASK                                           0x10000000
#define GET_MODE_VALUE_MASK											0x00ff0000

#define GET_MUX_INFO_SHIFT                                          28
#define GET_PHYSICAL_BUS_NO_SHIFT                                   24
#define GET_DEVICE_ADDRESS_SHIFT                                    16
#define GET_DATA_ADDRESS_SHIFT                                      8
#define GET_PWM_CHANNEL_NO_SHIFT                                    16
#define GET_SOFTWARE_OFFSET_SHIFT                                   24
#define GET_TEMPERATURE_MSB_SHIFT                                   1
#define GET_TEMPERATURE_LSB_SHIFT                                   7 
#define GET_SENSOR_MODULE_NUMBER_OFFSET_SHIFT						16
#define GET_AUTO_FAN_ENABLE_SHIFT                                   13
#define GET_FORMULA_TYPE_SHIFT                                      12
#define GET_MSB_DATA_MASK_SHIFT                                     20
#define GET_LSB_DATA_MASK_SHIFT                                     12
#define GET_GET_DIRECTION_SHIFT                                     30 
#define GET_GET_INIT_VALUE_SHIFT						            29
#define GET_GET_ON_VALUE_SHIFT                                      28
#define GET_MODE_VALUE_SHIFT										16

#define STATE_0_ASSERTED                                                0x01
#define STATE_1_ASSERTED                                                0x02
#define STATE_2_ASSERTED                                                0x04
#define STATE_3_ASSERTED                                                0x08
#define STATE_4_ASSERTED                                                0x10
#define STATE_5_ASSERTED                                                0x20
#define STATE_6_ASSERTED                                                0x40
#define STATE_7_ASSERTED                                                0x80
#define STATE_8_ASSERTED                                                0x01
#define STATE_9_ASSERTED                                                0x02
#define STATE_10_ASSERTED                                               0x04
#define STATE_11_ASSERTED                                               0x08
#define STATE_12_ASSERTED                                               0x10
#define STATE_13_ASSERTED                                               0x20
#define STATE_14_ASSERTED                                               0x40
#define BIT_7_RESERVED                                                  0x80

#define GET_PHYSICAL_BUS_NO(dw_Input) \
((unsigned char) ((dw_Input & GET_PHYSICAL_BUS_NO_MASK) >> GET_PHYSICAL_BUS_NO_SHIFT))
        
#define GET_DEVICE_ADDRESS(dw_Input) \
((unsigned char) ((dw_Input & GET_DEVICE_ADDRESS_MASK) >> GET_DEVICE_ADDRESS_SHIFT))

#define GET_DATA_ADDRESS(dw_Input) \
((unsigned char) ((dw_Input & GET_DATA_ADDRESS_MASK) >> GET_DATA_ADDRESS_SHIFT))

#define GET_SENSOR_TYPE(dw_Input)     ((unsigned char) (dw_Input & GET_SENSOR_TYPE_MASK))

#define GET_PWM_CHANNEL_NO(dw_Input) \
((unsigned char) ((dw_Input & GET_PWM_CHANNEL_NO_MASK) >> GET_PWM_CHANNEL_NO_SHIFT ) - 1)

#define GET_OPEN_DEFINE_VALUE(dw_HWInfo_1) \
((unsigned char) ((dw_HWInfo_1 & GET_OPEN_DEFINE_VALUE_MASK) >> 21))

#define GET_SOFTWARE_OFFSET(dw_Input) \
((unsigned char) ((dw_Input & GET_SOFTWARE_OFFSET_MASK) >> GET_SOFTWARE_OFFSET_SHIFT))

#define GET_TEMPERATURE_MSB(dw_Input) \
((unsigned char) ((dw_Input & GET_TEMPERATURE_MSB_MASK) << GET_TEMPERATURE_MSB_SHIFT))
        
#define GET_TEMPERATURE_LSB(dw_Input) \
((unsigned char) ((dw_Input & GET_TEMPERATURE_LSB_MASK) >> GET_TEMPERATURE_LSB_SHIFT))

#define GET_SENSOR_MODULE_NUMBER(dw_Input) \
((unsigned char) ((dw_Input & GET_SENSOR_MODULE_NUMBER_MASK) >> GET_SENSOR_MODULE_NUMBER_OFFSET_SHIFT))

#define GET_AUTO_FAN_ENABLE(dw_Input) \
((unsigned char) ((dw_Input & GET_AUTO_FAN_ENABLE_MASK) >> GET_AUTO_FAN_ENABLE_SHIFT))

#define GET_FORMULA_TYPE(dw_Input) \
((unsigned char) ((dw_Input & GET_FORMULA_TYPE_MASK) >> GET_FORMULA_TYPE_SHIFT))
		
#define GET_MUX_INFO(dw_Input) \
((unsigned char) ((dw_Input & GET_MUX_INFO_MASK) >> GET_MUX_INFO_SHIFT))
		
#define GET_MSB_DATA_MASK(dw_Input) \
((unsigned char) ((dw_Input & GET_MSB_DATA_ADDRESS_MASK) >> GET_MSB_DATA_MASK_SHIFT))

#define GET_LSB_DATA_MASK(dw_Input) \
((unsigned char) ((dw_Input & GET_LSB_DATA_ADDRESS_MASK) >> GET_LSB_DATA_MASK_SHIFT))	        		
		
#define GET_OUTPUT_ADDRESS(dw_Input) ((unsigned short int) (dw_Input & GET_DATA_ADDRESS_MASK))

#define GET_DIRECTION(dw_Input) \
((unsigned char) ((dw_Input & GET_DIRECTION_MASK) >> GET_GET_DIRECTION_SHIFT))

#define GET_INIT_VALUE(dw_Input) \
((unsigned char) ((dw_Input & GET_INIT_VALUE_MASK) >> GET_GET_INIT_VALUE_SHIFT))

#define GET_ON_VALUE(dw_Input) \
((unsigned char) ((dw_Input & GET_ON_VALUE_MASK) >> GET_GET_ON_VALUE_SHIFT))

#define GET_MODE_VALUE(dw_HWInfo_2) \
((unsigned char) ((dw_HWInfo_2 & GET_MODE_VALUE_MASK) >> GET_MODE_VALUE_SHIFT))





typedef struct
{
    STATUS (* SetupSensor) (
                            unsigned long int dw_HWInfo_1,

                            unsigned long int dw_HWInfo_2,

                            void  *p_HWInfoPtr,

                            void  *p_DataBuf
                           );

    STATUS (* PrySensor) (
                          unsigned long int dw_HWInfo_1,
                          
                          unsigned long int dw_HWInfo_2,

                          void  *p_HWInfoPtr,

                          void  *p_DataBuf
                         ); 

    STATUS (* WriteSensor) (
                            unsigned long int dw_HWInfo_1,

                            unsigned long int dw_HWInfo_2,

                            void  *p_HWInfoPtr,

                            void  *p_DataBuf
                           ); 

    STATUS (* ReadSensor) (
                           unsigned long int dw_HWInfo_1,
                           
                           unsigned long int dw_HWInfo_2,

                           void  *p_HWInfoPtr,

                           void  *p_DataBuf
                          );
} St_SensorAPITag;


typedef struct
{
    STATUS (* GenericCallBack) (
                                 void  *p_Ptr1,
                                 
                                 void  *p_Ptr2
                               );

    STATUS (* OEMCallBack) (
                             void  *p_Ptr1,
                             
                             void  *p_Ptr2
                           );

    STATUS (* SpecificCallBack) (
                                  void  *p_Ptr1,
                                  
                                  void  *p_Ptr2
                                ); 
    
} St_SensorCallBackAPITag;

typedef struct
{
	unsigned short int			w_last_sdr_ptr;
	unsigned short int			w_last_ptr;
	unsigned char 				b_sensor_data_num;
	unsigned char				sdr_record_num;
	unsigned char				b_fru_data_num;
	unsigned char				status;
	unsigned char				errmsg;
}OEMws_s_INIT_AGENT_RESULT;


#define MAX_FAN_MODE 	5
#define MAX_TEMP_SAMPLE 4
typedef struct
{
	BYTE name[32];
	BYTE Tn[MAX_FAN_MODE][MAX_TEMP_SAMPLE];
	BYTE Pn[MAX_FAN_MODE][MAX_TEMP_SAMPLE];
	BYTE Hysteresis;
	BYTE b_reading;
	BYTE pre_stable_reading;
	BYTE b_ForceFlag;
    void *p_Region;
}OEM_FANCTRL_PARAMTag;

#define MAXFANREGION_NUM  2
#define MAXFANPROFILE_NUM 4

typedef struct PWMRegionVarStruct 
{
	BYTE b_Dutycycle;
	OEM_FANCTRL_PARAMTag FanProfile[MAXFANPROFILE_NUM];
}St_PWMRegionVarTag;

#define END_ENTRY "Dummy Temp"
#define FANPROFILE_END { \
				.name 		   = END_ENTRY,\
				}

#define IS_FANPROFILE_END(x) (strstr(END_ENTRY,x) != NULL)

typedef enum 
{
	OEM_AT_OR_BELOW_LOWER_NON_CRITICAL=0xc1,
	OEM_AT_OR_BELOW_LOWER_CRITICAL=0xc2,
	OEM_AT_OR_BELOW_LOWER_NON_RECOVERABLE=0xc4,
	OEM_AT_OR_ABOVE_UPPER_NON_CRITICAL=0xc8,
	OEM_AT_OR_ABOVE_UPPER_CRITICAL=0xd0,
	OEM_AT_OR_ABOVE_UPPER_NON_RECOVERABLE=0xe0,
	OEM_AT_SENSOR_NORMAL=0xc0,
	OEM_ERROR_SENSOR_STATUS=0
} OEM_ANALOG_SENSOR_STATUS;

typedef enum 
{
	CORRECTABLE_MEMORY_ERROR_LOGGING_DISABLED_EVENT	= 0x00,
	EVENT_TYPE_LOGGING_DISABLED_EVENT = 0x01,						
	LOG_AREA_RESET_CLEARED_EVENT = 0x02,							
	ALL_EVENT_LOGGING_DISABLED_EVENT = 0x03,						
	SEL_FULL_EVENT = 0x04,										
	SEL_ALMOST_FULL_EVENT = 0x05									
} En_EVENT_LOGGING_DISABLED_OFFSETTag;


typedef struct
{
    unsigned char  analog_characteristic_flags;
    unsigned char  nominal_reading;
    unsigned char  normal_maximum;
    unsigned char  normal_minimum;
    unsigned char  upper_non_recoverable_threshold;
    unsigned char  upper_critical_threshold;
    unsigned char  upper_non_critical_threshold;
    unsigned char  lower_non_recoverable_threshold;
    unsigned char  lower_critical_threshold;
    unsigned char  lower_non_critical_threshold;
    unsigned char  sensor_reading;	
    unsigned char  old_sensor_reading;
	unsigned long int sensor_state_count;
	unsigned char pre_stable_reading;
	OEM_ANALOG_SENSOR_STATUS  sensor_status;
	OEM_ANALOG_SENSOR_STATUS  old_sensor_status;
}OEM_ANALOG_SENSOR_DATA;


typedef struct
{
	unsigned short int  disc_manual_assertion_event_status;
	unsigned short int  disc_manual_deassertion_event_status;
	unsigned char  disc_last_event_issued;
	unsigned char  disc_sensor_reading_LSB;
    unsigned char  disc_sensor_reading_MSB;
    unsigned char  old_disc_sensor_reading_LSB;
    unsigned char  old_disc_sensor_reading_MSB;
}OEM_DISC_SENSOR_DATA;


typedef struct {
    unsigned char  sensor_type;
	unsigned char  WS_reserve_1;
	unsigned char  WS_reserve_2;
	unsigned char  WS_reserve_3;
	unsigned char  sensor_number;
	unsigned char  sensor_power;
    unsigned char  WS_reserve_4;
    unsigned char  WS_reserve_5;
    unsigned char  WS_reserve_6;
    unsigned char  sensor_capabilities;
    unsigned char  event_slash_reading_type_code;
    unsigned short int  assertion_event_mask_lower_threshold_reading_mask;
    unsigned short int  deassertion_event_mask_upper_threshold_reading_mask;
    unsigned short int  WS_reserve_9;
	unsigned short int  assertion_event_status;
	unsigned short int  deassertion_event_status;
	unsigned char  WS_reserve_10;
    unsigned char  WS_reserve_11;
    unsigned char  WS_reserve_12;
    unsigned char  WS_reserve_13;
    unsigned char  WS_reserve_14;
    unsigned char  scan_period;
    unsigned char  WS_reserve_15;
    unsigned char  WS_reserve_16;
    unsigned char  WS_reserve_17;
    unsigned char  WS_reserve_18;
    unsigned char  WS_reserve_19;
    unsigned char  WS_reserve_20;
    unsigned char  WS_reserve_21;
	union
	{
		OEM_ANALOG_SENSOR_DATA analog_sensor;
		OEM_DISC_SENSOR_DATA disc_sensor;
	}sensor;

#ifdef OWN_HTTP_SERVER	
	unsigned char  WS_reserve_29;
	unsigned char  WS_reserve_30;
	unsigned char  WS_reserve_31;
	unsigned char  WS_reserve_32;
	unsigned char  WS_reserve_33;
	unsigned char  WS_reserve_34;
	unsigned char  WS_reserve_35;
	unsigned char  WS_reserve_36;
	unsigned char  WS_reserve_37[16];
	unsigned char  WS_reserve_38;
	unsigned char  WS_reserve_39;
#endif	
    unsigned short int offset;
} OEM_SENSOR_DATA;

#endif
