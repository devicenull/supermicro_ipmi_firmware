
#ifndef _OEMCMD_H_
#define _OEMCMD_H_  

#define GETOFFSET(x,y,z,n) (x | (y << 8) | (z << 16) | (n <<24))
#define GETCHUNKSIZE(x,y)  GETOFFSET(x,y,0,0)

#define BIOS_UPDATE_TIMEOUT					300
#define MAX_FW_CHUNK_SIZE					128
#define PARAM_MF_NAME               				1
#define PARAM_MB_ID                 				2
#define PARAM_IPMI_CID              				3
#define PARAM_IPMI_CVER             				4
#define PARAM_BMC_MODEL             				5
#define PARAM_EVENT_LOG_SWITCHING   				6	

#define NET_FN_OEM      							0x2E
#define SMC_FN_OEM	    							0x30
#define SMC_FN_OEM1									0x3C			
#define SMC_FN_OEM2									0x3A		
#define OEM_SET_CFG_CMD								0x01
#define OEM_GET_CFG_CMD								0x02
#define OEM_SET_FLOWCTRL_CMD						0x03
#define OEM_SET_EXTRA_ALERT_STRING					0x04
#define OEM_GET_EXTRA_ALERT_STRING					0x05
#define OEM_Update_SDR								0x06
#define OEM_Update_SDR_CTL							0x07
#define OEM_SET_SERIAL_MODE_CMD						0x09
#define OEM_LAN_INTERFACE_CHANGE					0x0a
#define OEM_TFTPD_CFG_CMD							0x0b

#define OEM_SMBUS_RELEASE_CMD						0x02
#define OEM_CLEAR_CHASSIS_INTRUSION					0x03
#define OEM_SMBUS_REQUEST_CMD						0x05
#define OEM_WRITE_FRU_CAPABILITY					0x06
#define OEM_GET_SMBUS_STATUS						0x07
#define OEM_GET_UID_STATUS							0x0C
#define OEM_ENABLE_UID								0x0D
#define OEM_DISABLE_UID								0x0E
#define OEM_GET_CHASSIS_INTRUSION_VALUE				0x0F
#define OEM_GET_UID_SUPPORT_STATUS					0x10
#define OEM_SET_HW_INFO								0x20
#define OEM_GET_HW_INFO								0x21
#define OEM_GET_SET_IP_CTRL	    					0x71    // Set or Get IP control
#define OEM_RAKP_CTRL_CMD	   						0x72    // Enable OEM RAKP

//Linda begin --- read/write i2c command
#define OEM_READ_I2C_CMD							0x90
#define OEM_WRITE_I2C_CMD							0x91
//Linda end ---
#define OEM_PS_STATUS_CMD							0x92	//linda: get ps status cmd
#define OEM_ME_CPU_TEMP_STATUS_CMD					0x93	//linda: get ME CPU cmd
#define OEM_ME_MEM_TEMP_STATUS_CMD					0x94	//linda: get ME MEM cmd
#define OEM_ME_PCH_TEMP_STATUS_CMD					0x95	//linda: get ME MEM cmd
#define OEM_ME_CPU_TJMAX_STATUS_CMD					0x96	//linda: get ME CPU Tjmax cmd
#define OEM_SET_NCT7904D_WDT_CMD					0x97	//linda: set nct7904d wdt cmd
#define OEM_RESET_NCT7904D_WDT_CMD					0x98	//linda: reset nct7904d wdt cmd
#define OEM_TOTAL_POWER_CONSUMPTION_CMD				0x9A	//linda: total power consumption
#define OEM_SET_SYS_CONFIG							0x99    // Set System Configuration
#define OEM_GET_SYS_CONFIG							0x9B    // Get System Configuration
#define OEM_SET_SYS_MAC_ADDRESS						0x9E    // Set System MAC Address
#define OEM_GET_SYS_MAC_ADDRESS						0x9F    // Get System MAC Address

#define OEM_DEBUG_MODE_CMD				0xB0	
#define OEM_DIS_EN_SERVICE        0xB1

//linda begin ---  OEM Command for X9DRX ---
#define OEM_SET_BIOS_Ver							0x04
#define OEM_GET_BIOS_Ver							0x03

#define	OEM_SET_PCIE_ID								0x3F
#define OEM_GET_PCIE_ID								0x3E

#define	OEM_WRITE_MANUF_DATA						0x08
#define OEM_READ_MANUF_DATA							0x09
//linda end ---  OEM Command for X9DRX ---

//linda begin ---  OEM Command for AI034 ---
#define OEM_SET_BIOS_Info							0xAC
//linda end ---  OEM Command for AI034 ---

#define OEM_SD3_GET_WORD							0x23
#define OEM_GET_SNOOPING_BYTE						0x2A
#define OEM_SD3_GETSET_BYTE							0x2C
#define OEM_SET_MOUSE_MODE_CMD			    		0x30
#define OEM_SET_MEDIA_ATTACH_MODE_CMD    			0x31
#define RESET_TO_FACTORY_DEFAULT_SETTING			0x40
#define OEM_RESTORE_FRU_TO_DEFAULT					0x41
#define RESET_TO_FACTORY_DEFAULT_SETTING_NOLAN		0x42
#define OEM_X7SB3_SET_SERIAL_MODE_CMD				0x70
#define OEM_RECOVER_LOCKED_USR_CMD					0x0B
#define OEM_SET_USER_ACCESS_CMD						0x43
#define OEM_SET_USER_NAME_CMD						0x45
#define OEM_RESTORE_SDR_TO_DEFAULT					0x44
#define OEM_HOSTNAME_OPERATION_CMD					0x47
#define OEM_FANSPEED_MODE_CMD              	 		0x45


#define OEM_FILE_TRANSACTION 						0xA0
#define OEM_SUBCMD_GETUPLOAD_RESERVE_ID				0x01
#define OEM_SUBCMD_START_FILEUPLOAD					0x02
#define	OEM_SUBCMD_FILEUPLOAD						0x03
#define	OEM_SUBCMD_APPLY_FILECOMMAND				0x04
#define	OEM_SUBCMD_PREPARE_FILEDOWNLOAD				0x05
#define	OEM_SUBCMD_PREPARE_FILEDOWNLOAD_DONE		0x06
#define	OEM_SUBCMD_FILEDOWNLOAD						0x07
#define OEM_SUBCMD_FILEDOWNLOAD_DONE				0x08
#define OEM_SUBCMD_BIOSGETFILE_STATUS 				0x09
#define OEM_SUBCMD_BIOSFILEDOWNLOAD					0x0A
#define OEM_SUBCMD_BIOSFILEDOWNLOAD_DONE			0x0B
#define OEM_SUBCMD_GETBIOSNV_UPDATE_STATUS			0x0C
#define OEM_SUBCMD_STARTBIOSCUR_SETTING_UPLOAD		0x0D
#define OEM_SUBCMD_BIOSCUR_SETTING_UPLOAD			0x0E
#define OEM_SUBCMD_BIOSCUR_SETTING_UPLOAD_DONE		0x0F
#define OEM_SUBCMD_GETBIOSCUR_SETTING_STATUS		0x10
#define OEM_SUBCMD_GETSETBIOSOOB_CAPABILITY			0x11
#define OEM_SUBCMD_GETSETBIOSOOB_IDENTIFY			0x12
#define OEM_SUBCMD_STARTBIOSDATFILEUPLOAD			0X13
#define OEM_SUBCMD_BIOSDATFILEUPLOAD				0X14
#define OEM_SUBCMD_BIOSDATFILEUPLOADDONE			0X15
#define OEM_SUBCMD_GETBIOSDATFILESTATUS				0X16
#define OEM_SUBCMD_GETBIOSDATFILEINFO				0x17
#define OEM_SUBCMD_GETPOWERSTATUS	  				0X18
#define OEM_SUBCMD_GETSMBIOSFILESTATUS	  			0X19
#define OEM_SUBCMD_SMBIOSDOWNLOAD					0x1A
#define OEM_SUBCMD_SMBIOSDOWNLOADDONE				0x1B
#define OEM_SUBCMD_GETOOBUPDATESTATUS				0x1C
#define OEM_SUBCMD_STARTSMBIOSUPLOAD				0x1D
#define OEM_SUBCMD_SMBIOSUPLOAD						0x1E
#define OEM_SUBCMD_SMBIOSUPLOADDONE					0x1F
#define OEM_SUBCMD_GETOOBFILESTATUS					0x20
#define OEM_SUBCMD_DATATOBMCREADYNOTIFY				0x21
#define OEM_SUBCMD_DATATOBIOSREADYNOTIFY			0x22
#define OEM_SUBCMD_DATATOBIOSDONE					0x23
#define OEM_SUBCMD_DATATOBMCDONE					0x24
#define OEM_SUBCMD_GETOOBFILECHECKSUM				0x25
#define OEM_SUBCMD_STARTBIOSUPLOADTOBMC				0x26
#define OEM_SUBCMD_BIOSUPLOADTOBMC					0x27
#define OEM_SUBCMD_BIOSUPLOADTOBMCDONE				0x28
#define OEM_SUBCMD_GETBIOSFILESIZE					0x29
#define OEM_SUBCMD_BIOSDOWNLOADFROMBMC				0x30
#define OEM_SUBCMD_BIOSDOWNLOADFROMBMCDONE			0x31
#define OEM_SUBCMD_DEBUG_FLAG						0xFF

#define OEM_GETPOWERSUPPLYFRU						0xA1
#define OEM_GETPOWERSUPPLYSTATUS					0x9D
// For cmd line BIOS update
#define OEM_GET_FW_INFO_CMD							0x60
#define OEM_START_FW_UPGRADE_CMD					0x61
#define OEM_UPLOAD_FW_CMD							0x62
#define OEM_FLASH_FW_CMD							0x63
#define OEM_CANCEL_FW_UPGRADE_CMD					0x64
#define OEM_FINALIZE_FW_UPGRADE_CMD					0x65
#define OEM_GET_FW_UPDATE_PROCESS_CMD				0x66
#define OEM_SET_BIOS_PRODUCTION_KEY         		0x69
#define OEM_GET_BIOS_LICENSE_STATUS         		0x6A

#define UPLOADFILE_PREFIX		"uploadfile_"
#define UPLOADFILE 	 			"/tmp/"UPLOADFILE_PREFIX
#define DOWNLOADFILE_PREFIX 	"downloadfile_"
#define DOWNLOADFILE 			"/tmp/"DOWNLOADFILE_PREFIX
#define BIOSNVFile   			"/tmp/biosnvfile"
#define BIOSCUR_SETTING_FILE 	"/tmp/bios_setting"
#define BIOSDAT_FILE 			"/nv/bios_dat"
#define BIOS_CAPABILITY  		"/nv/BIOS_Caps"
#define BIOSCUR_SMIBIOS 		"/tmp/BIOS_cur_smibios"
#define BIOSNEW_SMBIOS 			"/tmp/BIOS_new_smibios"
#define PS_OOB_FILE  			"/nv/PS.xml"


//Miko , 01252013 add for power comsumption monitor
#define at_a_St_PwPeakValueMax			at_St_OEMPS.at_PwConsumption.St_PwPeakValue[0]
#define at_a_St_PwPeakValueMin			at_St_OEMPS.at_PwConsumption.St_PwPeakValue[1]
#define at_b_LastRecordIdx           	at_St_OEMPS.at_PwConsumption.b_LastRecordIdx  
#define at_w_HourAvr					at_St_OEMPS.at_PwConsumption.a_St_PwRecord.Hour.w_AvrRecord
#define at_w_HourMax					at_St_OEMPS.at_PwConsumption.a_St_PwRecord.Hour.w_MaxRecord
#define at_w_HourMin					at_St_OEMPS.at_PwConsumption.a_St_PwRecord.Hour.w_MinRecord
#define at_w_HourMaxTime				at_St_OEMPS.at_PwConsumption.dw_PowerTimeRecord[0].w_TimeMax
#define at_w_HourMinTime				at_St_OEMPS.at_PwConsumption.dw_PowerTimeRecord[0].w_TimeMin
#define at_w_DayAvr 	   				at_St_OEMPS.at_PwConsumption.a_St_PwRecord.Day.w_AvrRecord
#define at_w_DayMax 	   				at_St_OEMPS.at_PwConsumption.a_St_PwRecord.Day.w_MaxRecord
#define at_w_DayMin 	   				at_St_OEMPS.at_PwConsumption.a_St_PwRecord.Day.w_MinRecord
#define at_w_DayMaxTime	   				at_St_OEMPS.at_PwConsumption.dw_PowerTimeRecord[1].w_TimeMax
#define at_w_DayMinTime	   				at_St_OEMPS.at_PwConsumption.dw_PowerTimeRecord[1].w_TimeMin
#define at_w_WeekAvr					at_St_OEMPS.at_PwConsumption.a_St_PwRecord.Week.w_AvrRecord
#define at_w_WeekMax					at_St_OEMPS.at_PwConsumption.a_St_PwRecord.Week.w_MaxRecord
#define at_w_WeekMin					at_St_OEMPS.at_PwConsumption.a_St_PwRecord.Week.w_MinRecord
#define at_w_WeekMaxTime				at_St_OEMPS.at_PwConsumption.dw_PowerTimeRecord[2].w_TimeMax
#define at_w_WeekMinTime				at_St_OEMPS.at_PwConsumption.dw_PowerTimeRecord[2].w_TimeMin
#define at_a_St_HourPwConsumption       at_St_OEMPS.at_PwConsumption.a_St_HourPwConsumption    
#define at_a_St_DayPwConsumption       	at_St_OEMPS.at_PwConsumption.a_St_DayPwConsumption     
#define at_a_St_WeekPwConsumption       at_St_OEMPS.at_PwConsumption.a_St_WeekPwConsumption     
#define	at_St_NowPwConsumption			at_St_OEMPS.at_PwConsumption.St_NowPwConsumption
				

typedef struct 
{
	WORD	w_AvrRecord;
	WORD	w_MaxRecord;
	WORD	w_MinRecord;	
}St_PwRecordTag;

typedef struct 
{
	DWORD	w_TimeMax;
	DWORD	w_TimeMin;	
}St_TimePwTag;

typedef struct 
{
	St_PwRecordTag Hour;
	St_PwRecordTag Day;
	St_PwRecordTag Week;	
}St_PwTimeRecordTag;

typedef struct 
{
	WORD	w_PwValue;
	DWORD	dw_PwTime;	
}St_PeakInfoTag;

// Jack:
typedef struct __attribute__ ((packed))
{
  unsigned short	dw_GPU_Status[6];
  unsigned short 	dw_GPU_ID[6];
  unsigned short 	dw_Vendor_ID[6];
  unsigned short 	dw_GPU_ActiveType[6];
  unsigned char		b_Config1[6];
  unsigned char		b_Config2[6];
} OEMX9GPUInfoTag;


// end of Jack

typedef struct PowerConsumptionStruct 
{
	BYTE	b_LastRecordIdx[3];     
	St_PeakInfoTag    St_PwPeakValue[2];    
	St_TimePwTag   dw_PowerTimeRecord[3];	
	St_PwRecordTag St_NowPwConsumption;
	St_PwTimeRecordTag a_St_PwRecord;
	St_PwRecordTag a_St_HourPwConsumption[12];
	St_PwRecordTag a_St_DayPwConsumption[24];
	St_PwRecordTag a_St_WeekPwConsumption[14];
} St_PowerConsumptionTag;


#pragma pack(push,1) 



typedef struct __attribute__ ((packed))
{
    BYTE 		    a_b_SDRVer[2];
    BYTE 		    a_b_FRUVer[2];
    unsigned long   dw_SDR_last_add_time;
    unsigned long   dw_SDR_last_erase_time;
    unsigned short  dw_GPU_ID1;   // [Farida] 
    unsigned short  dw_GPU_ID2;   // [Farida] 	
    //  OEMX9GPUInfoType oemX9GPUInfo; // [Jack]
    BYTE 			at_b_FanMode;

    //linda begin ---  OEM Command for X9DRX ---
    BYTE 			a_b_ManufData[64];
    BYTE 			a_b_BIOSVer[8];
    BYTE 			a_b_PCIEId[11][11];
    //linda end ---  OEM Command for X9DRX ---

    //linda begin ---  OEM Command for AI034 ---
    BYTE 			a_b_BIOSReleaseDate[16];
    //linda begin ---  OEM Command for AI034 ---
    // [Jack]
    BYTE 			a_b_IBAddOn;
    BYTE 			a_b_HotSwapController;
    BYTE 			a_b_12VFailure;
    BYTE 			a_b_FanLed_Control;    
    BYTE 			a_b_PSTimeoutEnable;
    WORD			a_w_PSTimeoutValue;

    BYTE 			a_b_OnBoard_LSI_HDD;
    BYTE 			a_b_Add_On_LSI_HDD;
    BYTE 			a_b_Check_GPIO77;

    BYTE            a_u8BIOSCapability[6];
    BYTE            a_u8BIOSIdentify[16];

    
    // Jack:
//    OEMX9GPUInfoTag		at_GPUInfo;		// nobody used, just commented to avoid confusion
	

	BYTE 			a_b_Link_Config;
	//Miko , 01252013 add for power comsumptionmonitor
	St_PowerConsumptionTag		at_PwConsumption; 
	/* Tony, 02/06/2013 add ac power on log  { */
	BYTE            a_b_system_reset_status_flag;
	/* Tony, 02/06/2013 add ac power on log  }*/
	BYTE			a_b_session_timeout;	/* for storing Web timeout value */
	BYTE			at_b_EnableSMCRAKP;
} OEMPSInfoType;


#pragma pack(pop)

#define CHASSIS_INTRUSION_SEN_NUM			0x54

#ifdef _OEMCMD_C_

#ifndef OPEN_BLADE
#define SERVICE_INDEX_NUM					3
#define SERIAL_REDIRECTION					0
#define KVM_REDIRECTION						1
#define STORAGE_REDIRECTION					2
#define VENDOR_DEFINED						3

typedef struct
{
	BYTE ServiceClass;
	BYTE Transport;
	BYTE IANAEnNum[3];
	BYTE ProtocolNum[2];
	BYTE ProtocolCap;
	BYTE MaxSession;
	BYTE ServiceName[32];
	BYTE NumOfPort;
	BYTE PortNums[10][2];
}ServiceInfoTag;

const ServiceInfoTag ServiceInfoTable[] =
{
	{0,0,{0,0,0},{0,0},0x07,1,{0x53,0x65,0x72,0x69,0x61,0x6c,0x5f,0x52,0x65,0x64,0x69,0x72,0x65,0x63,0x74,0x69,0x6f,0x6e,0,0,0,0,0,0,0,0,0,0,0,0,0,0},1,
	{{0x6f,0x02},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}}},
	{1,1,{0,0,0},{0,0},0x07,0,{0x4b,0x56,0x4d,0x5f,0x52,0x65,0x64,0x69,0x72,0x65,0x63,0x74,0x69,0x6f,0x6e,   0,   0,   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},1,{{0x17,0x0c},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}}},
  	{2,1,{0,0,0},{0,0},0x07,1,{0x53,0x74,0x6f,0x72,0x61,0x67,0x65,0x5f,0x52,0x65,0x64,0x69,0x72,0x65,0x63,0x74,0x69,0x6f,0x6e,0,0,0,0,0,0,0,0,0,0,0,0,0},1,{{0x6f,0x02},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}}}
};

#endif
#else

#ifdef RAM_SENSOR_EN
extern CMD_STATUS SetOEMSnRawCmd(MsgBufFmt *MsgBufIn,MsgBufFmt *MsgBufOut);
extern CMD_STATUS GetOEMSnRawCmd(MsgBufFmt *MsgBufIn,MsgBufFmt *MsgBufOut);
#endif

extern CMD_STATUS SetOEMCfgCmd(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS GetOEMCfgCmd(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);  
extern CMD_STATUS SetOEMBoardIDCmd(MsgBufFmt *MsgBufIn,MsgBufFmt *MsgBufOut);
extern CMD_STATUS SetGetOEMSOLFlowCtrlCmd(MsgBufFmt *MsgBufIn,MsgBufFmt *MsgBufOut);
extern CMD_STATUS SetOEMRestoreDefaultCmd(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS SetOEMExtraAlertString(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS GetOEMExtraAlertString(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS OEMUpdateSDR(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS OEMUpdateSDRCTL(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS OEMLanInterfaceSwitch(MsgBufFmt *MsgBufIn, MsgBufFmt *MsgBufOut);

#ifdef ATEN_TFTP_FEATURE
extern CMD_STATUS OEMTFTPDCmd(MsgBufFmt *MsgBufIn, MsgBufFmt *MsgBufOut);
#endif

extern CMD_STATUS SetOEMSerialPortModeCmd(MsgBufFmt *MsgBufIn,MsgBufFmt *MsgBufOut);
extern CMD_STATUS OEMGetSetIPCtrl(MsgBufFmt * p_St_MsgBufIn,MsgBufFmt *MsgBufOut);
extern CMD_STATUS OEMRAKPCtrlCmd(MsgBufFmt * p_St_MsgBufIn,MsgBufFmt *MsgBufOut);
extern CMD_STATUS SetOEMRecoverLockedUserCmd(MsgBufFmt *MsgBufIn,MsgBufFmt *MsgBufOut);

#ifndef OPEN_BLADE
extern CMD_STATUS GetServiceInfoCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
#endif	

#ifdef FW_CHKSUM
extern CMD_STATUS GetFWChkSumCmd(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
#endif

#ifdef NM_DBG
extern CMD_STATUS SetOEMPwrCmd(MsgBufFmt *MsgBufIn,MsgBufFmt *MsgBufOut);
extern CMD_STATUS SetOEMAirTempCmd(MsgBufFmt *MsgBufIn,MsgBufFmt *MsgBufOut);
#endif

#ifdef NODE_MAN_EN
extern CMD_STATUS GetPowerConsumption(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS MEPowerStateChange(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS GetInletAirTemperature(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
#endif

extern CMD_STATUS SMBusReleaseCmd(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS ClearChassisIntrusion(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS SMBusRequestCmd(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS WriteFruCapability(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS GetSMBusStatus(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS GetUIDStatus(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS EnableUID(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS DisableUID(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS GetChassisIntrusion(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS GetUIDSupportStatus(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS SetHWInfo(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS GetHWInfo(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);

//linda begin ---  OEM Command for AI034 ---
extern CMD_STATUS GetSetBIOSInfo(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
//linda end ---  OEM Command for AI034 ---

//linda begin ---  OEM Command for X9DRX ---
extern CMD_STATUS SetBIOSVer(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS GetBIOSVer(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);

extern CMD_STATUS SetPcieId(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS GetPcieId(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);

extern CMD_STATUS WriteManufData(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS ReadManufData(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
//linda end ---  OEM Command for X9DRX ---

//Linda begin --- read/write i2c command
extern CMD_STATUS ReadI2CCmd(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS WriteI2CCmd(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
//Linda end --- 
extern CMD_STATUS GetPSStatusCmd(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);	//linda: get ps status cmd
extern CMD_STATUS GetMECPUTempCmd(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);	//linda: get ME CPU temp cmd
extern CMD_STATUS GetMEMEMTempCmd(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);	//linda: get ME MEM temp cmd
extern CMD_STATUS GetMEPCHTempCmd(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);	//linda: get ME PCH temp cmd
extern CMD_STATUS GetMECPUTjmaxCmd(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);	//linda: get ME CPU Tjmax cmd
extern CMD_STATUS SetNCT7904DSoftWDTimerCmd(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);	//linda: set WDTimer cmd
extern CMD_STATUS ResetNCT7904DSoftWDTimerCmd(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);	//linda: reset WDTimer cmd
extern CMD_STATUS GetTotalPowerConsumption(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);	//linda: reset WDTimer cmd
extern CMD_STATUS SetSYSConfig(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS GetSYSConfig(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS SetSysMacAddress(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS GetSysMacAddress(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);

extern CMD_STATUS SD3GetWord(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS GetSnoopingByte(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS SD3GetSetByte(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS ResetToFactoryDefaultSetting(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS ResetToFactoryDefaultSettingNoLAN(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS SetMouseModeCmd(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS SetMediaAttachModeCmd(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS OEMSetUserAccessCmd(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS RestoreSDR2DefaultCmd(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS  RestoreFRUDefaultCmd(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS OEMHostNameOperationCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetSetOEMFanSpeedMode(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
//[Jack]
extern CMD_STATUS OEMDebugMode(MsgBufFmt *pMsgBuf, MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS OEM_DisEnSERVICE(MsgBufFmt *pMsgBuf, MsgBufFmt *pRespMsgBuf);

// For cmd line BIOS update
extern CMD_STATUS OEMGetFWInfoCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS OEMStartFWUpgradeCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS OEMUploadFWCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS OEMFlashFWCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS OEMCancelFWUpgradeCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS OEMFinalizeFWUpgradCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS OEMGetFWUpdateProcess(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS OEMSetProductionKey(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS OEMGetProductionKeyStatus(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS OEMEnableBIOSUPdate(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS FileTransactions(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS OEMGetPowerSupplyFRU(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS OEMGetPowerSupplyStatus(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS OEMSetSMIReady(MsgBufFmt * p_St_MsgBufIn,MsgBufFmt * p_St_MsgBufOut);
extern CMD_STATUS OEMGetSMIReason(MsgBufFmt * p_St_MsgBufIn,MsgBufFmt * p_St_MsgBufOut);
extern CMD_STATUS OEMSetSMIAck(MsgBufFmt * p_St_MsgBufIn,MsgBufFmt * p_St_MsgBufOut);
extern CMD_STATUS SetSysMacAddress(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS GetSysMacAddress(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS GetUploadResvID(MsgBufFmt * p_St_MsgBufIn,MsgBufFmt * p_St_MsgBufOut);
extern CMD_STATUS StartFileUpload(MsgBufFmt * p_St_MsgBufIn,MsgBufFmt * p_St_MsgBufOut);
extern CMD_STATUS FileUpload(MsgBufFmt * p_St_MsgBufIn,MsgBufFmt * p_St_MsgBufOut);
extern CMD_STATUS ApplyFileCommand(MsgBufFmt * p_St_MsgBufIn,MsgBufFmt * p_St_MsgBufOut);
extern CMD_STATUS PrepareFileDownload(MsgBufFmt * p_St_MsgBufIn,MsgBufFmt * p_St_MsgBufOut);
extern CMD_STATUS PrepareFileDownloadDone(MsgBufFmt * p_St_MsgBufIn,MsgBufFmt * p_St_MsgBufOut);
extern CMD_STATUS FileDownload(MsgBufFmt * p_St_MsgBufIn,MsgBufFmt * p_St_MsgBufOut);
extern CMD_STATUS FileDownloadDone(MsgBufFmt * p_St_MsgBufIn,MsgBufFmt * p_St_MsgBufOut);
#endif   

#endif  
