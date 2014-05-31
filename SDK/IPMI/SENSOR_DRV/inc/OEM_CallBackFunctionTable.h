
#ifndef _OEM_CallBackFunction_TABLE_H_
#define _OEM_CallBackFunction_TABLE_H_

#define	TABLE_TAIL										0xFF
#define OEM_CallBack_TABLE_TAIL                         0xFF
#define ARRAY_INFO_END									0xFF
#define Reserved										0
#define MAX_REACTED_NUMBER	 							4
#define MAX_RELATED_NUMBER	 							4


typedef struct
{
	BYTE b_RSNSnrNo;
	
	BYTE a_b_RACTSnrNo[MAX_REACTED_NUMBER] ;
}SensorRelationTableType;

typedef struct
{
	WORD dw_event_mask ;
	
	BYTE b_hysteresis_Pvalue;	
	
	BYTE b_hysteresis_Nvalue;		

	BYTE a_b_threshold_value[6];		

	BYTE b_control_value[12];
}OEMEventControlTableType;

typedef struct
{
	BYTE b_RACTSnrNo;
	
	BYTE b_reg_address;

	BYTE a_b_ctrl_value_controller[MAX_RELATED_NUMBER];

	BYTE a_b_ctrl_value[MAX_RELATED_NUMBER];
}SensorReactInfoTableType;

extern SensorRelationTableType SensorRelationTable[];
extern OEMEventControlTableType OEMEventControlTable[][MAX_REACTED_NUMBER];
extern SensorReactInfoTableType SensorReactInfoTable[];


#endif




