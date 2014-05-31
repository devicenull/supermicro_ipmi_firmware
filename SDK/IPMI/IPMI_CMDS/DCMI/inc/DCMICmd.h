
#ifndef _DCMICMD_H_
#define _DCMICMD_H_

#define b_NoMoreInfo    0xC1

#define NET_FN_DCMI						0x2C


#define	DCMI_GROUP_CODE					0xDC

#define	SUPPORT_DCMI_CAPABILITIES		0x01
#define	MANDATORY_PLATFORM_ATTRIBUTES	0x02
#define	OPTIONAL_PLATFORM_ATTRIBUTES	0x03
#define	MANAGEABILITY_ACCESS_ATTRIBUTES	0x04
#define ENHANCED_SYS_PWR_STATISTICS_ATTRIBUTES   0x05

#define DCMI_SPEC_MAJOR_VER             0x01
#define DCMI_SPEC_MINOR_VER             0x01
#define PARAMETER_REVISION              0x02


/* Mandatory Platform Capabilities */
#define	IDENTIFICATION_SUPPORT			(1 << 0)
#define	SEL_LOGGING						(1 << 1)
#define	CHASSIS_POWER					(1 << 2)
#define	TEMPERATURE_MONITOR				(1 << 3)

/* Optional Platform Capabilities*/
#define	POWER_MANAGEMENT				(1 << 0)

/* Manageability Access Capabilities*/
#define	IN_BAND_KCS						(1 << 0)
#define	OO_BAND_TMODE					(1 << 1)
#define	OO_SECONDARY_LAN				(1 << 2)
#define	OO_PRIMARY_LAN					(1 << 3)
#define	SOL_SUPPORT						(1 << 4)
#define	VLAN_CAPABLE					(1 << 5)

/* SEL Attributes */
#define	SEL_AUTOMATIC_ROLLOVER_ENABLE	 0x8000

/* Identification Attributes */
#define	ASSET_TAG_SUPPORT				(1 << 2)
#define	DHCP_HOST_NAME_SUPPORT			(1 << 1)
#define	GUID_SUPPORT					(1 << 0)

/* 	Temperature Monitoring */
#define	BASEBOARD_TEMP					(1 << 2)
#define	PROCESSORS_TEMP					(1 << 1)
#define	INLET_TEMP						(1 << 0)

//#define HOSTNAME_FILE "/etc/hostname_for_dhcp"
#define MaxMCIDLength	64 

typedef struct
{
    BYTE b_ManufactLen;
    BYTE b_ProductNameLen;
    BYTE b_ProductPartNumLen;  
    BYTE b_ProductVersionLen;    
    BYTE b_ProductSerialNumLen;
    BYTE b_AssetTagLen;
    BYTE b_FruFileIDLen;	
}St_DCMIFruInfoType;

#ifdef _DCMICMD_C_
St_DCMIFruInfoType St_DCMIFruInfo;
CMD_STATUS GetDCMICapabilitiesInfoCmd(MsgBufFmt *MsgBufIn,MsgBufFmt *MsgBufOut);
CMD_STATUS GetAssetTagCmd(MsgBufFmt *MsgBufIn,MsgBufFmt *MsgBufOut);
CMD_STATUS SetAssetTagCmd(MsgBufFmt *MsgBufIn,MsgBufFmt *MsgBufOut);
CMD_STATUS GetDCMISensorInfoCmd(MsgBufFmt *MsgBufIn,MsgBufFmt *MsgBufOut);
CMD_STATUS GetPowerReadingCmd(MsgBufFmt *MsgBufIn,MsgBufFmt *MsgBufOut);
CMD_STATUS GetManagementControllerIDCmd(MsgBufFmt *MsgBufIn,MsgBufFmt *MsgBufOut);
CMD_STATUS SetManagementControllerIDCmd(MsgBufFmt *MsgBufIn,MsgBufFmt *MsgBufOut);
CMD_STATUS GetPowerLimitCmd(MsgBufFmt *MsgBufIn,MsgBufFmt *MsgBufOut);
CMD_STATUS SetPowerLimitCmd(MsgBufFmt *MsgBufIn,MsgBufFmt *MsgBufOut);
CMD_STATUS ADctivateCmd(MsgBufFmt *MsgBufIn,MsgBufFmt *MsgBufOut);

#else
extern St_DCMIFruInfoType St_DCMIFruInfo;
extern CMD_STATUS GetDCMICapabilitiesInfoCmd(MsgBufFmt *MsgBufIn,MsgBufFmt *MsgBufOut);
extern CMD_STATUS GetAssetTagCmd(MsgBufFmt *MsgBufIn,MsgBufFmt *MsgBufOut);
extern CMD_STATUS SetAssetTagCmd(MsgBufFmt *MsgBufIn,MsgBufFmt *MsgBufOut);
extern CMD_STATUS GetDCMISensorInfoCmd(MsgBufFmt *MsgBufIn,MsgBufFmt *MsgBufOut);
extern CMD_STATUS GetPowerReadingCmd(MsgBufFmt *MsgBufIn,MsgBufFmt *MsgBufOut);
extern CMD_STATUS GetManagementControllerIDCmd(MsgBufFmt *MsgBufIn,MsgBufFmt *MsgBufOut);
extern CMD_STATUS SetManagementControllerIDCmd(MsgBufFmt *MsgBufIn,MsgBufFmt *MsgBufOut);
extern CMD_STATUS GetPowerLimitCmd(MsgBufFmt *MsgBufIn,MsgBufFmt *MsgBufOut);
extern CMD_STATUS SetPowerLimitCmd(MsgBufFmt *MsgBufIn,MsgBufFmt *MsgBufOut);
extern CMD_STATUS ADctivateCmd(MsgBufFmt *MsgBufIn,MsgBufFmt *MsgBufOut);

#endif
#endif


/* End of file */




