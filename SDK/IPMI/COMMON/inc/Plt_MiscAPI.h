


#define ZERO        0




typedef struct
{
    unsigned long int dw_RTS_CTS;                      
    unsigned long int dw_DTR_DSR;                      
}UART_HW_FlowControl_InfoTag;



typedef struct {
	unsigned char	DevID;
	unsigned char   DeviceRev;
	unsigned char   FirmwareRev1;
	unsigned char   FirmwareRev2;	
	unsigned char   IPMIVer;
	unsigned char   AddtnlDevSup;
	unsigned long	ManfID;
	unsigned int	PdtID;
}Dev_Info_tag;



typedef struct
{
    unsigned char FilterConfiguration;
    unsigned char EventFilterAction;
    unsigned char AlertPolicyNumber;
    unsigned char EventSeverity;
    unsigned char GeneratorID_Byte1;
    unsigned char GeneratorID_Byte2;
    unsigned char SensorType;
    unsigned char SensorNo;
    unsigned char EventTrigger;
    unsigned char EventDataOneEventOffsetMask_Data1;
    unsigned char EventDataOneEventOffsetMask_Data2;
    unsigned char EventDataOneANDMask;
    unsigned char EventData1Compare_1;
    unsigned char EventData1Compare_2;
    unsigned char EventData2ANDMask;
    unsigned char EventData2Compare_1;
    unsigned char EventData2Compare_2;
    unsigned char EventData3ANDMask;
    unsigned char EventData3Compare_1;
    unsigned char EventData3Compare_2;
}PEF_Entry_Table_Tag;







extern unsigned char OEMPltGPIO_Setup( unsigned long int dw_HWInfo_1, unsigned long int dw_HWInfo_2, void *p_HWInfoPtr, void *p_DataBuf);

extern unsigned char OEMPltGPIO_Write( unsigned long int dw_HWInfo_1, unsigned long int dw_HWInfo_2, void *p_HWInfoPtr, void *p_DataBuf);

extern unsigned char OEMPltGPIO_Read( unsigned long int dw_HWInfo_1, unsigned long int dw_HWInfo_2, void *p_HWInfoPtr, void *p_DataBuf);





typedef struct I2CFrameType
{
	unsigned char b_channel;
	unsigned char b_slaveaddress;
	unsigned char b_writebytecount1;
	unsigned char b_writebytecount2;
	unsigned char b_readbytecount;
	unsigned char *p_b_writebuf1;
	unsigned char *p_b_writebuf2;
	unsigned char *p_b_readbuf;
	unsigned char b_MUXInfo;
} St_I2CFrameTag;


extern unsigned char OEMI2C_MasterWriteRead(St_I2CFrameTag s_I2CFrame);


extern unsigned char OEMPltI2C_ChannelInit(unsigned char ChannelNo);




unsigned char OEMWritePWMFANDrv( unsigned long int dw_HWInfo_1, unsigned long int dw_HWInfo_2, void *p_HWInfoPtr, void *p_DataBuf);
unsigned char OEMWriteDAFANDrv( unsigned long int dw_HWInfo_1, unsigned long int dw_HWInfo_2, void *p_HWInfoPtr, void *p_DataBuf);






extern void *HeapAlloc(unsigned long int);



extern void OEMPSInit( unsigned int , void * , void *);
extern unsigned char OEMPSCommit( unsigned int , unsigned int , unsigned int , void *);  


extern void LAN_MAC_Address_Get(unsigned char b_DevID, unsigned long int *, unsigned long int *);
extern void LAN_IP_Address_Get(unsigned char b_DevID, unsigned long int *);
extern void LAN_HeaderInfo_Get(unsigned char b_DevID, void **p_Packet, void **p_IP, void *ptr);
extern void LAN_PacketLength_Get( void *, unsigned short int * );


extern unsigned char LAN_IncomingPacketAppend(unsigned char *, unsigned short int ,void *,void *);
extern unsigned char LAN_OutgoingPacketGet(void *packet_ptr, unsigned char *buf_ptr );



extern unsigned char SlaveRegisterEvent(void *HandleFuncPtr, void *EventID);
extern unsigned char SlaveTaskEventSet(unsigned long int u32EventID, void *Ptr);




unsigned char OEMReadSDR(
				 unsigned char sensor_num,		
				 unsigned char b_Offset,		
				 unsigned char b_Length,	
				 unsigned char *p_b_ReadBuf	
			   );





