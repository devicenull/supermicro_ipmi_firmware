
#ifndef _SENSORINITAGENT_H_
#define _SENSORINITAGENT_H_

#define MAX_MSG_CTRL_NUM 		    10
#define	TEMP_BUF_SIZE				0x44
#define	SDRTYPE					    2
#define	IG_ENABLE					1
#define	IG_DISABLE					0
#define SENSOR_DATA_SIZE			at_b_Max_Sensor_Data_Num * (sizeof(St_SensorDataTag))




typedef enum
{
    ERRMSG_MC_LIST = 0,
    ERRMSG_MAX_SENSOR_DATA_SIZE = 1,
    ERRMSG_MAX_FRU_DATA_SIZE,
    ERRMSG_NONE,
    ERRMSG_NO_SDR,
    ERRMSG_EEPROM_ERR,
    ERRMSG_CSUM_ERR
} ERRMSG;

typedef struct
{
	BYTE		b_slave_addr;
	BOOLEAN		bl_enable_msg_gen;
}at_s_MSG_CTRL_LIST;

typedef struct
{
	BYTE	device_access_addr;
	BYTE	fru_device_id_slash_device_slave_addr;
	BYTE	logical_physical_slash_access_lun_slash_bus_id;
	BYTE	channel_num;
	BYTE	device_type;
	BYTE	device_type_modify;
	BYTE	fru_entity_id;
	BYTE	fru_entity_instance;
}St_FruDataTag;

typedef struct
{
	WORD			w_last_sdr_ptr;
	WORD			w_last_ptr;
	BYTE			b_sensor_data_num;
	BYTE			sdr_record_num;
	BYTE			b_fru_data_num;
	STATUS			status;
	ERRMSG		errmsg;
}at_s_INIT_AGENT_RESULT;


#ifdef _SENSORINITAGENT_C_                     
BYTE SearchSensorOffset(BYTE b_sensorlun, BYTE b_sensornum);
#else
#ifdef OEM_CALLBACK_FUNCTION
extern St_SensorDataTag *at_St_OEMCallBack_SensorData;
#endif

extern St_SensorDataTag *at_St_SensorData;
extern at_s_INIT_AGENT_RESULT at_St_InitAgentResult;
extern St_FruDataTag *at_p_St_FruData;
extern at_s_MSG_CTRL_LIST at_a_St_McList[MAX_MSG_CTRL_NUM];
#endif  
#endif  





