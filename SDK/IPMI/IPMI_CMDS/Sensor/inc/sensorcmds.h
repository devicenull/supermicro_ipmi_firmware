
#ifndef _SENSORCMDS_H_
#define _SENSORCMDS_H_




#ifdef _SENSORCMDS_C_
CMD_STATUS SetSensorHysteresisCmd(MsgBufFmt *, MsgBufFmt *);
CMD_STATUS GetSensorHysteresisCmd(MsgBufFmt *, MsgBufFmt *);
CMD_STATUS SetSensorThresholdCmd(MsgBufFmt *, MsgBufFmt *);
CMD_STATUS GetSensorThresholdCmd(MsgBufFmt *, MsgBufFmt *);
CMD_STATUS SetSensorEventEnableCmd(MsgBufFmt *, MsgBufFmt *);
CMD_STATUS GetSensorEventEnableCmd(MsgBufFmt *, MsgBufFmt *);
CMD_STATUS RearmSensorEventsCmd(MsgBufFmt *, MsgBufFmt *);
CMD_STATUS GetSensorEventStatusCmd(MsgBufFmt *, MsgBufFmt *);
CMD_STATUS GetSensorReadingCmd(MsgBufFmt *, MsgBufFmt *);
CMD_STATUS OEMSetSensorReadingCmd(MsgBufFmt *, MsgBufFmt *);

#else

extern CMD_STATUS SetSensorHysteresisCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS GetSensorHysteresisCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS SetSensorThresholdCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS GetSensorThresholdCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS SetSensorEventEnableCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS GetSensorEventEnableCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS RearmSensorEventsCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS GetSensorEventStatusCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS GetSensorReadingCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS SetSensorTypeCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS GetSensorTypeCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS OEMSetSensorReadingCmd(MsgBufFmt *, MsgBufFmt *);


#endif
#endif




