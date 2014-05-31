
#ifndef _OEMGPARAM_H_
#define _OEMGPARAM_H_

#include "UtilOOB.h"

#define MAX_BIOS_SIZE	16*1024*1024	// 16 MB

#define MAX_GPU_SUPPORT 15

#define QWORD		unsigned long long	// for 64 bits define

enum 
{
	STANDARD_SPEED,
	FULL_SPEED,
	OPTIMAL_SPEED,
  PUE_OPTIMAL_SPEED,
  HEAVYIO_SPEED    
};
//linda begin --- HDD Status
typedef struct HDDStatusVarStruct
{
	DWORD dw_OverallHDDStatus;
	DWORD dw_PreHDDStatus;
	DWORD dw_CurHDDStatus;
}St_HDDStatusVarTag;
//linda end --- HDD Status

typedef struct PowerSensorVarStruct
{
	DWORD dw_PWRConsumption;
	DWORD dw_PWRInputCurrent;
	DWORD dw_PWRInputPower;		
}St_PowerSensorVarTag;

typedef struct St_METempTblStruct
{
	BYTE CPUTemp[4];		// CPU0~3
	BYTE PreCPUTemp[4];		// CPU0~3
	BYTE MemTemp[64];	// 4CPU, 4Ch, 4DIMM
	BYTE PCHTemp;
	BYTE b_Tjmax[4];
	// Jack
	BYTE b_10GTemp;
	BYTE b_IBTemp;
	BYTE b_MaxTemp;
  BYTE b_MaxMEMTemp_CPU1;
  BYTE b_MaxMEMTemp_CPU2;
  BYTE b_RT1Temp;
  BYTE b_RT2Temp;
	BYTE b_GPUTemp[8];
}St_METempTblTag;

// [Jack] KNC Property
typedef struct __attribute__ ((packed))
{
  BYTE	slave_address;
  BYTE 	sensor_no;
  BYTE	is_sensor_found;
} St_KNC_InfoTag;

// [Jack] GPU Info Struct for X9DRG
typedef struct __attribute__ ((packed))
{
  unsigned short	dw_GPU_Status[MAX_GPU_SUPPORT];
  unsigned short 	dw_GPU_ID[MAX_GPU_SUPPORT];
  unsigned short 	dw_Vendor_ID[MAX_GPU_SUPPORT];
  unsigned short 	dw_GPU_ActiveType[MAX_GPU_SUPPORT];
  unsigned char		b_Config1[MAX_GPU_SUPPORT];
  unsigned char		b_Config2[MAX_GPU_SUPPORT];
} OEMX9GPUInfoType;


typedef struct 
{
  BYTE slot_no;
  BYTE status_bits;    
  // Definition:
  // bit 0: Present/Not Present
  // bit 1: Throttling Assert/Deassert
  // bit 2: Full Speed Triggered by failure reading using Read/Write
  BYTE event_data2;
  
} XeonPhi_ThrottlingType;

typedef struct 
{
  BYTE sim_enable;
  BYTE temperature;  
  BYTE knc_status_bit;
  BYTE ext_conf;
} XeonPhi_SimType;

// [Jack] Zone Info
typedef struct{
	BYTE zone_temperature;
	DWORD hwInfo;
} GPU_ZONE_INFO;

//Linda --- begin HWInfo
typedef struct SMCHWInfoVarStruct
{
	int BoardId;		// mb id
	int HWMonitor;		// hardware monitor chip number
	int NumLan;			// number of lan chip on mb				
	int MBMac[6];			// system mac address
}St_SMCHWInfoVarTag;
//Linda --- end


//Linda --- begin Sensor Data
typedef struct SMCSensorDataVarStruct
{
	BYTE SlaveAddr;
	BYTE Bank;
	BYTE Offset;
	BYTE Value;
}St_SMCSensorDataVarTag;
//Linda --- end


//Linda --- begin Memory Sensors
typedef struct SMCDIMMTempVarStruct
{
	signed char tempVal;		// mem temp value
	char overheatVal;			// mem overheat value
	char cpuGroup;				
	char returnIdx;				// mem dimm sensor return index
}St_SMCDIMMTempVarTag;

typedef struct SMCDIMMGpioTblStruct
{
	BYTE Gpio21;			// gpio21 0/1
	BYTE Gpio22;			// gpio22 0/1
	BYTE GpioFlag;			// bit 0 = 0 (No need Gpio21), 1 (need), Bit 1=0 (No need Gpio22), 1 (need), Bit 2=1 need to set as input 1st, then output when we use it
}St_SMCDIMMGpioTblTag;

typedef struct SMCDIMMRefreshTblStruct
{
	BYTE Gpio74;			// gpio74 0/1
	BYTE Gpio75;			// gpio75 0/1
	BYTE GpioFlag;			// bit 0 = 0 (No need Gpio74), 1 (need), Bit 1=0 (No need Gpio75), 1 (need)
}St_SMCDIMMRefreshTblTag;

typedef struct SMCDIMMIndexTblStruct
{
	BYTE cpuGroup;
	BYTE size;				// actual used dimm size
	BYTE SlaveAddr[8];		// slave addr table
	BYTE return_index[8];	// mem dimm sensor return index
}St_SMCDIMMIndexTblTag;
//Linda --- end

// joe --- begin 8-10-2012 for multi-node PS
#define TOTAL_PS		2
#define TOTAL_PSDATA	50
#define PS_DETECT_COUNT		5	

typedef struct __attribute__ ((packed))
{
	BYTE	offset;
	BYTE	data[2];
} PSData_t;

typedef struct __attribute__ ((packed))
{
	BYTE	PSFruBitMap[32];
	BYTE	PSFruData[256];
	PSData_t PSData[TOTAL_PSDATA];
} PSFru_t;
// joe --- end 8-10-2012

// joe --- begin 2-5-2013 for BIOS passing in more than 3 mac address 
typedef struct __attribute__ ((packed))
{
	BYTE	NumLan;
	BYTE	MacAddr[36];	// assume max mac address is 6
} St_SysMacAddressTag;
// joe --- end 2-5-2013

// joe --- begin 9-18-2012 for HDD
#define LSI_SLAVE_ADDR				0x02
#define MAX_API_LOGICAL_DRIVES      64      // 64. But if all 64 are configured then reconstruction 
#define MAX_API_ROW_SIZE            32      // maximum disks per array
#define MAX_SPAN_DEPTH              8       // maximum spans per logical drive
#define MAX_API_ARRAYS_DEDICATED    16      // maximum number of arrays a hot spare may be dedicated to
#define MAX_API_PHYSICAL_DEVICES	256     // only 255 physical devices may be used. Also can be got from 
#define MAX_HDD_SUPPORT				240
#define MAX_ENCLOSURE				10
#define KNC_FAN_FAIL_INIT		0
#define KNC_FAN_FAIL_STATUS_BIT		1
#define KNC_FAN_FAIL_I2CBUS		2

#define VOLTAGE_TRACKING_INTERVAL	39
#define VOLTAGE_FAIL_LIMIT		10

typedef struct __attribute__ ((packed))
{
	BYTE	BMCAddr;
	BYTE	AppMsgTag;
	WORD	CtrlCmd:4;
	WORD	Payload:12;
} LSICtrlCmd_t;

typedef struct __attribute__ ((packed))
{
	BYTE	BMCAddr;
	BYTE	reserved;
	BYTE	LSIMsg:4;
	BYTE	IC:1;
	BYTE	reserved1:3;
	BYTE	ChkSum;
	WORD	SeqNum:12;
	WORD	reserved2:4;
	BYTE	AppMsgTag;
	BYTE	MFI_CMD_OP_Code;
	DWORD	LenCmdPayload;
	WORD	LenRspPayload;
	WORD	reserved3;
	DWORD	DCMD_Opcode;
	BYTE	mbox[12];
} LSICmd_t;

typedef struct __attribute__ ((packed))
{
	BYTE	BMCAddr;
	BYTE	AppMsgTag;
	WORD	ControlCmd:4;
	WORD	Payload:12;
} LSISubsequentCmd_t;
/*
 * defines the physical drive reference structure
 */
typedef union  __attribute__ ((packed))
{
    struct __attribute__ ((packed))
	{
        WORD     deviceId;   // PD Device Id
        WORD     seqNum;     // Sequence Number
    } mrPdRef;              // unnamed struct:  search for NO_UNNAMED_STRUCT for usage
    DWORD     ref;            // shorthand reference to full 32-bits
} MR_PD_REF;                // 4 bytes

/*
 * define the DDF Type bit structure
 */
typedef union __attribute__ ((packed))
{
    struct __attribute__ ((packed))
	{
        union __attribute__ ((packed))
		{
            struct __attribute__ ((packed))
			{
#ifndef MFI_BIG_ENDIAN
                WORD     forcedPDGUID    : 1;    // TRUE if PD GUID was forced
                WORD     inVD            : 1;    // TRUE if this PD is used in a VD
                WORD     isGlobalSpare   : 1;    // TRUE if this is a global hot spare
                WORD     isSpare         : 1;    // TRUE if this is a dedicated hot spare
                WORD     isForeign       : 1;    // TRUE if this drive contains a foreign configuration
                WORD     reserved        : 7;    // reserved for future use
                WORD     intf            : 4;    // 0-Unknown, 1-parallel SCSI, 2-SAS, 3-SATA, 4-FC
#else
                WORD     intf            : 4;    // 0-Unknown, 1-parallel SCSI, 2-SAS, 3-SATA, 4-FC
                WORD     reserved        : 7;    // reserved for future use
                WORD     isForeign       : 1;    // TRUE if this drive contains a foreign configuration
                WORD     isSpare         : 1;    // TRUE if this is a dedicated hot spare
                WORD     isGlobalSpare   : 1;    // TRUE if this is a global hot spare
                WORD     inVD            : 1;    // TRUE if this PD is used in a VD
                WORD     forcedPDGUID    : 1;    // TRUE if PD GUID was forced
#endif  // MFI_BIG_ENDIAN
            } pdType;
            WORD     type;                       // alternate name for full 16-bits
        };
        WORD     reserved;                       // reserved
    } ddf;                                      // All the fields are from DDF
    struct __attribute__ ((packed))
	{
        DWORD    reserved;                        // reserved for non-disk devices
    } nonDisk;
    DWORD     type;                               // shorthand reference to full 32 bits
} MR_PD_DDF_TYPE;

/*
 * defines the physical drive allowed operations structure
 */
typedef struct __attribute__ ((packed))
{
#ifndef MFI_BIG_ENDIAN
    DWORD makeOnline          : 1;
    DWORD makeOffline         : 1;
    DWORD makeFailed          : 1;
    DWORD makeBad             : 1;
    DWORD makeUnconfigured    : 1;
    DWORD makeSpare           : 1;
    DWORD removeSpare         : 1;
    DWORD replaceMissing      : 1;
    DWORD markMissing         : 1;
    DWORD startRebuild        : 1;
    DWORD stopRebuild         : 1;
    DWORD locate              : 1;
    DWORD pdClear             : 1;
    DWORD foreignImportNotAllowed : 1;    // This field is valid if MR_PD_INFO.state.ddf.pdType.isForeign is set
    DWORD startCopyBack       : 1;
    DWORD stopCopyBack        : 1;
    DWORD fwDownloadNotAllowed    : 1;
    DWORD reprovision         : 1;
    DWORD makeSystem          : 1;
    DWORD T10PowerModeSupported : 1;
    DWORD suspendRebuild        : 1;  // 1=suspend Rebuild operation
    DWORD resumeRebuild         : 1;  // 1=resume Rebuild operation
    DWORD suspendCopyback       : 1;  // 1=suspend Copyback operation
    DWORD resumeCopyback        : 1;  // 1=resume Copyback operation
    DWORD startSecureEraseNonSED: 1;  // 1=start: secure erase for non SED drives
    DWORD stopSecureEraseNonSED : 1;  // 1=stop: secure erase for non SED drives
    DWORD reserved            :6;
#else
    DWORD reserved            :6;
    DWORD stopSecureEraseNonSED : 1;  // 1=stop: secure erase for non SED drives
    DWORD startSecureEraseNonSED: 1;  // 1=start: secure erase for non SED drives
    DWORD resumeCopyback        : 1;  // 1=resume Copyback operation
    DWORD suspendCopyback       : 1;  // 1=suspend Copyback operation
    DWORD resumeRebuild         : 1;  // 1=resume Rebuild operation
    DWORD suspendRebuild        : 1;  // 1=suspend Rebuild operation
    DWORD T10PowerModeSupported : 1;
    DWORD makeSystem          : 1;
    DWORD reprovision         : 1;
    DWORD fwDownloadNotAllowed    : 1;
    DWORD stopCopyBack        : 1;
    DWORD startCopyBack       : 1;
    DWORD foreignImportNotAllowed : 1;    // This field is valid if MR_PD_INFO.state.ddf.pdType.isForeign is set
    DWORD pdClear             : 1;
    DWORD locate              : 1;
    DWORD stopRebuild         : 1;
    DWORD startRebuild        : 1;
    DWORD markMissing         : 1;
    DWORD replaceMissing      : 1;
    DWORD removeSpare         : 1;
    DWORD makeSpare           : 1;
    DWORD makeUnconfigured    : 1;
    DWORD makeBad             : 1;
    DWORD makeFailed          : 1;
    DWORD makeOffline         : 1;
    DWORD makeOnline          : 1;
#endif  // MFI_BIG_ENDIAN
} MR_PD_ALLOWED_OPS;

/*
 * defines the physical drive allowed operations list structure
 */
typedef struct __attribute__ ((packed))
{
    DWORD                 count;                                  // count of valid entries in this list
    DWORD                 pad;
    MR_PD_ALLOWED_OPS   allowedOpsList[MAX_API_PHYSICAL_DEVICES];   // Use deviceId to index into this array
} MR_PD_ALLOWED_OPS_LIST;                                       // This structure is 1032 bytes

/*
 * defines the progress structure
 */
typedef union __attribute__ ((packed))
{
    struct __attribute__ ((packed))
	{
        WORD     progress;       // 0-0xFFFF - %complete: progress*100/0xFFFF
        WORD     elapsedSecs;    // how long the operation is in progress in secs
    } mrProgress;               // unnamed struct:  search for NO_UNNAMED_STRUCT for usage
    DWORD     w;
} MR_PROGRESS;              // 4 bytes
#define MR_PROGRESS_DONE    0xffff      // value for 100%

/*
 * defines the time-stamp structure
 */
typedef struct __attribute__ ((packed))
{
    WORD     year;           // year starting from 2000
    BYTE      month;          // month
    BYTE      day;            // day
    BYTE      hour;           // hour
    BYTE      min;            // minutes
    BYTE      sec;            // seconds
    BYTE      reserved;       // pad
} MR_TIME_STAMP;            // 8 bytes

/*
 * defines the physical drive progress structure
 */
typedef struct __attribute__ ((packed))
{
    struct __attribute__ ((packed))
	{
#ifndef MFI_BIG_ENDIAN
        DWORD     rbld    : 1;    // rebuild
        DWORD     patrol  : 1;    // patrol read
        DWORD     clear   : 1;    // clear all data
        DWORD     copyBack: 1;    // copy all data to the destination PD
        DWORD     erase   : 1;    // secure erase
        DWORD     locate  : 1;    // locate drive
        DWORD     reserved:26;
#else
        DWORD     reserved:26;
        DWORD     locate  : 1;    // locate drive
        DWORD     erase   : 1;    // secure erase
        DWORD     copyBack: 1;    // copy all data to the destination PD
        DWORD     clear   : 1;    // clear all data
        DWORD     patrol  : 1;    // patrol read
        DWORD     rbld    : 1;    // rebuild
#endif  // MFI_BIG_ENDIAN
    } active;
    MR_PROGRESS     rbld;       // rbld/copyBack progress
    MR_PROGRESS     patrol;     // patrol read progress
    union __attribute__ ((packed))
	{
        MR_PROGRESS     clear;      // clear progress
        MR_PROGRESS     erase;      // erase progress
    };

    struct __attribute__ ((packed)) 
	{
#ifndef MFI_BIG_ENDIAN
        DWORD     rbld    : 1;    // rebuild
        DWORD     patrol  : 1;    // patrol read
        DWORD     clear   : 1;    // clear all data
        DWORD     copyBack: 1;    // copy all data to the destination PD
        DWORD     erase   : 1;    // secure erase
        DWORD     reserved:27;
#else
        DWORD     reserved:27;
        DWORD     erase   : 1;    // secure erase
        DWORD     copyBack: 1;    // copy all data to the destination PD
        DWORD     clear   : 1;    // clear all data
        DWORD     patrol  : 1;    // patrol read
        DWORD     rbld    : 1;    // rebuild
#endif  // MFI_BIG_ENDIAN
    } pause;

    MR_PROGRESS     reserved[3];
} MR_PD_PROGRESS;               // size = 4 * (4 * number of bits defined) bytes, today it is 20 bytes

/*
 * inquiry data
 */
typedef struct __attribute__ ((packed))
{
	BYTE	Unknown[8];		// unknown
 	BYTE	Vendor[8];	   	// Vnedor Name
	BYTE	ModelName[16];	// Model Name
	BYTE	Revision[4];	// Revision
	BYTE	info[32];		// vendor specific info
} MR_InquiryData;
/*
 * defines the physical drive information structure
 */
typedef struct __attribute__ ((packed))
{
    MR_PD_REF   ref;
    BYTE      inquiryData[96];
    BYTE      vpdPage83[64];              // VPD page 83. Also, see below for vpdPage83Ext.

    BYTE      notSupported;               // Set, if this drive is not supported. Like SATA disk if SAS only supported...etc
                                        // Non-certified drive may or may not fall under this category.
    BYTE      scsiDevType;                // Disk, Tape, CDROM


    union __attribute__ ((packed))
	{
        BYTE      connectedPortBitmap;    // bitmap of ports on which this device is connected (bit 0=port 0, bit 1=port 1, etc).
        BYTE      connectedPortNumbers;   // If MR_CTRL_INFO.adapterOperations.supportMoreThan8Phys=1, use connectedPortNumbers
                                        // Lower nibble is Port Number for path 0, Upper Nibble is Port Number for path 1.
    };

    BYTE      deviceSpeed;                // device maximum supported speed (MR_PD_SPEED)

    DWORD     mediaErrCount;
    DWORD     otherErrCount;              // transient errors detected, while communicating with this device
    DWORD     predFailCount;
    DWORD     lastPredFailEventSeqNum;    // sequence number of the last pred fail event on this pd

    WORD     fwState;                    // MR_PD_STATE. Firmware's interpretation of drive state
    BYTE      disabledForRemoval;         // If set, drive is ready for removal
    BYTE      linkSpeed;                  // negotiated link speed (MR_PD_SPEED)
                                        // For dual-ported drives,
                                        // if only one port is connected or active, then it is the active ports link speed
                                        // if both ports are present and only one port is used, then it is that ports link speed
                                        // if both ports are present and both ports are simultaneously used,
                                        // then it is that ports whose bit is unset (lsb) in the pathInfo isPathBroken.
                                        // see linkSpeedOther below


    MR_PD_DDF_TYPE  state;              // DDF state bits

    struct __attribute__ ((packed))
	{
        BYTE      count;                  // 0-none is valid, 1,2 can be a valid value
#ifndef MFI_BIG_ENDIAN
        BYTE      isPathBroken    : 4;    // Each bit will tell if the path (address) is broken or ok
        BYTE      reserved3       : 3;
        BYTE      widePortCapable : 1;    // Device is wide port capable, see sasAddr as well
#else
        BYTE      widePortCapable : 1;    // Device is wide port capable, see sasAddr as well
        BYTE      reserved3       : 3;
        BYTE      isPathBroken    : 4;    // Each bit will tell if the path (address) is broken or ok
#endif  // MFI_BIG_ENDIAN

        BYTE      connectorIndex[2];      // Index into MR_SAS_CONNECTORS.connectors, to which this path is attached.
                                        // Only used if MR_CTRL_INFO.adapterOperations.supportMultipath is set.

        BYTE      reserved[4];            // pad for 64-bit alignment
        //U64     sasAddr[2];             // Device SAS addresses, upto 2 (both may be same if its a wide port)
		QWORD		sasAddr[2];
        //DWORD     sasAddr0_h;            
        //DWORD     sasAddr0_l;            
        //DWORD     sasAddr1_h;            
        //DWORD     sasAddr1_l;            
        BYTE      reserved2[16];
    } pathInfo;

    //U64     rawSize;                    // size before COD and coercion
	QWORD	rawSize;
	//DWORD	rawSize_h;
	//DWORD	rawSize_l;
    //U64     nonCoercedSize;             // size after COD is reserved but before coercion
	QWORD	nonCoercedSize;
    //DWORD    nonCoercedSize_h;             // size after COD is reserved but before coercion
    //DWORD    nonCoercedSize_l;             // size after COD is reserved but before coercion
    //U64     coercedSize;                // size after COD is reserved and after coercion
	QWORD	coercedSize;
    //DWORD     coercedSize_h;                // size after COD is reserved and after coercion
    //DWORD     coercedSize_l;                // size after COD is reserved and after coercion

    WORD     enclDeviceId;               // enclosure's device Id; if device is not within enclosure value is 0xFFFF
    BYTE      enclIndex;                  // DEPRECATED   Firmware internal enclosure index

    union __attribute__ ((packed))
	{
        BYTE      slotNumber;             // slot number of this drive, if this PD is an enclosure then this has no meaning

        BYTE      enclConnectorIndex;     // DEPRECATED
                                        // MR_SAS_CONNECTORS.connectors[enclConnectorIndex] to which this enclosure (chain) is attached.
                                        // Only used if MR_CTRL_INFO.adapterOperations.supportEnclEnumeration is set,
                                        // and this MR_PD_INFO pertains to an enclosurePD.Also see MR_PD_INFO.enclPostion
    };



    MR_PD_PROGRESS progInfo;            // drive progress information, for non-disk devices, no meaning

    BYTE      badBlockTableFull;          // flag indicating this drive's bad block table is full (primarily used for MPT IM/IR)
    BYTE      unusableInCurrentConfig;    // TRUE when this drive can?t be used as currently configured

    BYTE      vpdPage83Ext[64];           // This field works in conjunction with vpdPage83 above.

    BYTE      powerState;                 // power condition of the device (MR_PD_POWER_STATE)
    BYTE      enclPosition;               // Position of the enclosure in a cascaded chain.
                                        // Only used if MR_CTRL_INFO.adapterOperations.supportEnclEnumeration is set,
                                        // and this MR_PD_INFO pertains to an enclosurePD. Also see MR_PD_INFO.enclConnectorIndex.


    MR_PD_ALLOWED_OPS   allowedOps;     // Allowed operations on the PD.
    WORD     copyBackPartnerId;          // copyBack Partner device Id during copyBack, else MR_PD_INVALID

    WORD     enclPartnerDeviceId;        // If drive, and it is in an enclosure with dual paths, this is the deviceId of the redundant enclosure module.
                                        // If enclosure, and there are 2 EMMs this field is deviceId of redundant EMM.
                                        // Set to MR_PD_INVALID in all other cases.
                                        // This field is valid only if MR_CTRL_INFO.adapterOperations.supportMultipath is set.

    struct __attribute__ ((packed)) 
	{
#ifndef MFI_BIG_ENDIAN
        WORD fdeCapable      : 1; // 1=FDE drive, 0=non-FDE
        WORD fdeEnabled      : 1; // FDE drive with 1=security enabled, 0=security disabled
        WORD secured         : 1; // 1=drive is secured, 0=not secured
        WORD locked          : 1; // 1=drive is locked, 0=unlocked
        WORD foreign         : 1; // 1=drive is secured by a foreign lock key
        WORD needsEKM        : 1; // 1=drive requires EKM
        WORD reserved        :10;
#else
        WORD reserved        :10;
        WORD needsEKM        : 1; // 1=drive requires EKM
        WORD foreign         : 1; // 1=drive is secured by a foreign lock key
        WORD locked          : 1; // 1=drive is locked, 0=unlocked
        WORD secured         : 1; // 1=drive is secured, 0=not secured
        WORD fdeEnabled      : 1; // FDE drive with 1=security enabled, 0=security disabled
        WORD fdeCapable      : 1; // 1=FDE drive, 0=non-FDE
#endif  // MFI_BIG_ENDIAN
    } security;

    BYTE      mediaType;                  // type of device media (MR_PD_MEDIA_TYPE)

    BYTE      notCertified;               // Set if the drive is not certified

    BYTE      bridgeVendor[8];                 // per VPD89 bytes 8-15
    BYTE      bridgeProductIdentification[16]; // per VPD89 bytes 16-31
    BYTE      bridgeProductRevisionLevel[4];   // per VPD89 bytes 32-35
    BYTE      satBridgeExists;                 // Indicates that SATA device is behind a SAS-SATA bridge

    BYTE      interfaceType;              // 0-Unknown, 1-parallel SCSI, 2-SAS, 3-SATA, 4-FC
    BYTE      temperature;                // in Celsius, if MR_CTLR_INFO.pdOperations.supportTemperature (0xFF - invalid temp)
    BYTE      emulatedBlockSize;          // Ratio of physical block size to logical block size, expressed as a power of 2
    WORD     userDataBlockSize;          // Number of bytes of user data in LBA
    WORD     reserved2;

    struct __attribute__ ((packed))
	{
#ifndef MFI_BIG_ENDIAN
        DWORD piType              : 3; // SCSI Protection Information Type (MR_PROT_INFO_TYPE)
        DWORD piFormatted         : 1; // 1=drive is formatted for PI information, 0=no PI data
        DWORD piEligible          : 1; // 1=drive can be used for PD Enabled LD
        DWORD NCQ                 : 1; // SATA drives NCQ setting
        DWORD WCE                 : 1; // configured drives write-cache setting
        DWORD commissionedSpare   : 1; // this PD is a commissioned spare. Ex: previously this was a revertible spare and now
                                     // is commisioned in the array.
        DWORD emergencySpare      : 1; // this PD is commsisioned as emergency spare

        DWORD reserved            : 23;
#else
        DWORD reserved            : 23;
        DWORD emergencySpare      : 1; // this PD is commsisioned as emergency spare
        DWORD commissionedSpare   : 1; // this PD is a commissioned spare. Ex: previously this was a revertible spare and now
                                     // is commisioned in the array.
        DWORD WCE                 : 1; // configured drives write-cache setting
        DWORD NCQ                 : 1; // SATA drives NCQ setting
        DWORD piEligible          : 1; // 1=drive can be used for PD Enabled LD
        DWORD piFormatted         : 1; // 1=drive is formatted for PI information, 0=no PI data
        DWORD piType              : 3; // SCSI Protection Information Type (MR_PROT_INFO_TYPE)
#endif
    } properties;

    MR_TIME_STAMP   shieldDiagCompletionTime;   // last successful Diagnostics completion time. 0 is N/A
    BYTE              shieldCounter;              // how many times the shield diagnostics was run before it was marked failed
    BYTE      linkSpeedOther;     // negotiated link speed (MR_PD_SPEED): Applies to only dual ported drives
                                // This would be the passive ports link speed if only one is being used as active
                                // If both ports are simultaneously used, then it is that ports whose bit
                                // is unset after the first unset lsb in the pathInfo isPathBroken


    BYTE      reserved1[512-422];
} MR_PD_INFO;   // Total size 512 bytes  

/*
 * defines the physical drive address structure
 */
typedef struct __attribute__ ((packed))
{
    WORD     deviceId;           // sticky device Id
    WORD     enclDeviceId;       // enclosure's device Id; if device is not within enclosure value is 0xFFFF
                                // If same as deviceId, it is the enclosure itself
    union __attribute__ ((packed))
	{
        struct __attribute__ ((packed))
		{                // For legacy usage, if MR_CTRL_INF0.adapterOperations.supportEnclEnumeration is not set.
                                // Or for Physical Drive, if MR_CTRL_INF0.adapterOperations.supportEnclEnumeration is set
            BYTE  enclIndex;      // for enclosures, 0 means not within an enclosure. DEPRECATED
            BYTE  slotNumber;     // Slot number of the drive.
        } mrPdAddress;          // unnamed struct:  search for NO_UNNAMED_STRUCT for usage

        struct __attribute__ ((packed))
		{                // For Enclosures, if MR_CTRL_INF0.adapterOperations.supportEnclEnumeration is set
            BYTE  enclPosition;   // Position of an enclosure in a cascaded chain.
            BYTE  enclConnectorIndex; // MR_SAS_CONNECTORS.connectors[enclConnectorIndex] to which this enclosure (chain) is attached.
        } mrEnclAddress;        // unnamed struct:  search for NO_UNNAMED_STRUCT for usage
    };

    BYTE      scsiDevType;        // 0-disk,

    union __attribute__ ((packed))
	{
        BYTE      connectedPortBitmap;    // bitmap of ports on which this device is connected (bit 0=port 0, bit 1=port 1, etc).
        BYTE      connectedPortNumbers;   // If MR_CTRL_INFO.adapterOperations.supportMoreThan8Phys=1, use connectedPortNumbers
                                        // Lower nibble is Port Number for path 0, Upper Nibble is Port Number for path 1.
    };
    //U64     sasAddr[2];         // SAS addresses for port 0/1 (0 means port not used)
	QWORD	sasAddr[2];
	//DWORD	sasAddr_0_h;
	//DWORD	sasAddr_0_l;
	//DWORD	sasAddr_1_h;
	//DWORD	sasAddr_1_l;
} MR_PD_ADDRESS;                // 24 bytes

/*
 * defines the physical drive list structure
 */
typedef struct __attribute__ ((packed))
{
    DWORD            size;       // size of total data structure (including size/count fields)
    DWORD            count;      // count of number of drives in the system (i.e. entries in this list)
    MR_PD_ADDRESS   addr[1];    // Instances of count number of PD addresses
} MR_PD_LIST;                   // size = count * sizeof(MR_PD_ADDRESS) bytes, today for 256 devices it is 6152 bytes
                     
typedef struct __attribute__ ((packed))
{
	BYTE	Vendor[8];					// HDD vendor (in ascii format)
	BYTE	ModelName[16];				// HDD model name (in ascii format)
	BYTE	Revision[4];				// HDD Revision (in ascii format)
	DWORD	MediaErrCount;				// Media Error Count
	DWORD	OtherErrCount;				// Other Error Count
	DWORD	PredFailCount;				// Predicted Fail Count
	DWORD	LastPredFailEventSeqNum;	// Last Predicated Fail Event Seq Num

	WORD	fwState;					// MR_PD_STATE_UNCONFIGURED_GOOD = 0x00, Unconfigured good drive
										// MR_PD_STATE_UNCONFIGURED_BAD  = 0x01, Unconfigured bad drive
										// MR_PD_STATE_HOT_SPARE         = 0x02, Hot spare drive
										// MR_PD_STATE_OFFLINE           = 0x10, Configured-good drive(data invalid)
										// MR_PD_STATE_FAILED            = 0x11, Configured-bad drive (data invalid)
										// MR_PD_STATE_REBUILD           = 0x14, Configured-drive is rebuilding
										// MR_PD_STATE_ONLINE            = 0x18, Configured-drive is online
										// MR_PD_STATE_COPYBACK          = 0x20, drive is getting copied
										// MR_PD_STATE_SYSTEM            = 0x40, drive is exposed and controlled by host
										// MR_PD_STATE_SHIELD_UNCONFIGURED = 0x80, UnConfigured - shielded
										// MR_PD_STATE_SHIELD_HOT_SPARE    = 0x82, Hot Spare - shielded
										// MR_PD_STATE_SHIELD_CONFIGURED   = 0x90, Configured - shielded

	BYTE	LinkSpeed;					// SPEED_UNKNOWN             = 0,     not defined
    									// SPEED_1p5G                = 1,     1.5Gb/s - SATA 150
    									// SPEED_3G                  = 2,     3.0Gb/s
    									// SPEED_6G                  = 3,     6.0Gb/s
   										// SPEED_12G                 = 4,     12.0Gb/s

	QWORD	CoercedSize;				// high DWORD size after COD is reserved and after coercion
										// calculate HDD GB size ((CoercedSize_h << 32) | CoercedSize_l)/(1024*1024*2)

	BYTE	Temp;						// temperature
	BYTE	EnclosureIndex;				// Enclosure Index
	BYTE	reserved[80-57];			// 80 bytes of this strucure
} HDDInfo_t;

typedef struct __attribute__ ((packed))
{
	DWORD	size_l;				// LD size/(1024*1024*2)=xxxGB
	DWORD	size_h;				// LD size/(1024*1024*2)=xxxGB
	BYTE	PRL;				// primary raid level
    BYTE	RLQ;				// raid level qaulifier
	BYTE	SRL;				// secondary raid level
	BYTE	StripeSize;			// LD strip size (per DDF: 4=8K, 5=16K, 7=64K, 11=1MB, etc.)
	BYTE	NumDrives;			// number of drives per span
	BYTE	SpanDepth;			// span depth
	BYTE	state;				// logical drive state
								// MR_LD_STATE_OFFLINE             = 0,
    							// MR_LD_STATE_PARTIALLY_DEGRADED  = 1,
    							// MR_LD_STATE_DEGRADED            = 2,
    							// MR_LD_STATE_OPTIMAL             = 3
	BYTE	reserved[32-15];	// reserved
} LogHDDInfo_t;

// RAID structure
/*
 * defines the logical drive reference structure
 */
typedef union  __attribute__ ((packed))
{        // LD reference structure
    struct __attribute__ ((packed))
	{
        BYTE      targetId;           // LD target id (0 to MAX_TARGET_ID)
        BYTE      reserved;           // reserved to make in line with MR_PD_REF
        WORD     seqNum;             // Sequence Number
    } mrLdRef;                      // unnamed struct:  search for NO_UNNAMED_STRUCT for usage
    DWORD     ref;                    // shorthand reference to full 32-bits
} MR_LD_REF;                        // 4 bytes

/*
 * defines the logical drive list structure
 */
typedef struct __attribute__ ((packed))
{
    DWORD     ldCount;                // number of LDs
    DWORD     reserved;               // pad to 8-byte boundary
    struct __attribute__ ((packed))
	{
        MR_LD_REF   ref;            // LD reference
        BYTE          state;          // current LD state (MR_LD_STATE)
        BYTE          reserved[3];    // pad to 8-byte boundary
        QWORD         size;           // LD size
    } ldList[MAX_API_LOGICAL_DRIVES];
} MR_LD_LIST;                       // 4 + 4 + (MAX_API_LOGICAL_DRIVES * 16), for 40LD it is = 648 bytes

/*
 * defines the logical drive targetId list structure
 */
typedef struct __attribute__ ((packed))
{
    DWORD             size;           // size of total data structure (including size/count fields)
    DWORD             count;          // count of number of targetId entries in this list
    BYTE              pad[3];
    BYTE              targetId[1];    // Instances of count number of LD targetId
} MR_LD_TARGETID_LIST;              // size =  11 + count * sizeof(U8) bytes, today for 64 LDs it is = 75 bytes

/*
 * define constants for logical drive target id list query options
 */
typedef enum 
{
    MR_LD_QUERY_TYPE_ALL                = 0,    // return all LDs (no value, result maybe greater than MAX_LOGICAL_DRIVES)
    MR_LD_QUERY_TYPE_EXPOSED_TO_HOST    = 1,    // query for all LDs that are exposed to OS (no value)
    MR_LD_QUERY_TYPE_USED_TGT_IDS       = 2,    // query for all LD target Ids that are in use (no value),
                                                // such as those pinned by cache or orphan PiTs, and all LDs exposed to OS, etc.
} MR_LD_QUERY_TYPE;
#define MR_LD_QUERY_TYPE_MAX              2     // maximum permissible value

/*
 * define disk write cache policy settings
 */
typedef enum 
{
    MR_PD_CACHE_UNCHANGED   = 0,    // don't change disk cache policy - use disk's default
    MR_PD_CACHE_ENABLE      = 1,    // enable disk write cache
    MR_PD_CACHE_DISABLE     = 2,    // disable disk write cache
} MR_PD_CACHE;
#define MR_PD_CACHE_MAX     2       // maximum permissible value

/*
 * define the LD cache policy settings
 */
typedef enum 
{         // LD Cache Policy bit definitions
    MR_LD_CACHE_WRITE_BACK          = 0x01,     // write back
    MR_LD_CACHE_WRITE_ADAPTIVE      = 0x02,     // Adaptive Write (not currently implemented)
    MR_LD_CACHE_READ_AHEAD          = 0x04,     // Read Ahead
    MR_LD_CACHE_READ_ADAPTIVE       = 0x08,     // Adaptive Read ahead
    MR_LD_CACHE_WRITE_CACHE_BAD_BBU = 0x10,     // write caching ok if bad BBU
    MR_LD_CACHE_ALLOW_WRITE_CACHE   = 0x20,     // write caching allowed
    MR_LD_CACHE_ALLOW_READ_CACHE    = 0x40,     // read caching allowed
} MR_LD_CACHE;
#define MR_LD_CACHE_MASK            0x7f        // mask of all valid bits

/*
 * define helper macros for traditional MegaRAID cache policies.  The read, write, and IO
 * policies may be ored together to define the complete cache policy.
 */
#define MR_LD_CACHE_POLICY_READ_AHEAD_NONE      0
#define MR_LD_CACHE_POLICY_READ_AHEAD_ALWAYS    MR_LD_CACHE_READ_AHEAD
#define MR_LD_CACHE_POLICY_READ_AHEAD_ADAPTIVE  (MR_LD_CACHE_READ_AHEAD | MR_LD_CACHE_READ_ADAPTIVE)

#define MR_LD_CACHE_POLICY_WRITE_THROUGH        0
#define MR_LD_CACHE_POLICY_WRITE_BACK           MR_LD_CACHE_WRITE_BACK
#define MR_LD_CACHE_POLICY_WRITE_CACHE_BAD_BBU  (MR_LD_CACHE_WRITE_BACK | MR_LD_CACHE_WRITE_CACHE_BAD_BBU)

#define MR_LD_CACHE_POLICY_IO_CACHED            (MR_LD_CACHE_ALLOW_WRITE_CACHE | MR_LD_CACHE_ALLOW_READ_CACHE)
#define MR_LD_CACHE_POLICY_IO_DIRECT            0

/*
 * define a bitmap structure for alternate access to cache policy settings
 */
typedef union __attribute__ ((packed))
{
    struct __attribute__ ((packed))
	{
#ifndef MFI_BIG_ENDIAN
        BYTE      writeBack           : 1;
        BYTE      writeBackAdaptive   : 1;
        BYTE      readAhead           : 1;
        BYTE      readAheadAdaptive   : 1;
        BYTE      writeCacheBadBbu    : 1;
        BYTE      allowWriteCache     : 1;
        BYTE      allowReadCache      : 1;
        BYTE      reserved            : 1;
#else
        BYTE      reserved            : 1;
        BYTE      allowReadCache      : 1;
        BYTE      allowWriteCache     : 1;
        BYTE      writeCacheBadBbu    : 1;
        BYTE      readAheadAdaptive   : 1;
        BYTE      readAhead           : 1;
        BYTE      writeBackAdaptive   : 1;
        BYTE      writeBack           : 1;
#endif  // MFI_BIG_ENDIAN
    } mrLdCachePolicy;                     // unnamed struct:  search for NO_UNNAMED_STRUCT for usage
    BYTE      b;
} MR_LD_CACHE_STRUCT;

/*
 * define the LD access policy settings
 */
typedef enum 
{        // LD Access Policy definitions
    MR_LD_ACCESS_RW        = 0,
    MR_LD_ACCESS_READ_ONLY = 2,
    MR_LD_ACCESS_BLOCKED   = 3,
} MR_LD_ACCESS;
#define MR_LD_ACCESS_MASK   3

/*
 * define the reason/status for LD access policy settings
 */
typedef enum
{ // LD Access Policy Status definitions
    MR_LD_ACCESS_STATUS_USER            = 0x00,     // policy set by user
    MR_LD_ACCESS_STATUS_SSC_OFFLINE     = 0x01,     // secondary cache is offline, some data is unavailable
} MR_LD_ACCESS_STATUS;

/*
 * define the enum for logical drive states
 */
typedef enum 
{
    MR_LD_STATE_OFFLINE             = 0,
    MR_LD_STATE_PARTIALLY_DEGRADED  = 1,
    MR_LD_STATE_DEGRADED            = 2,
    MR_LD_STATE_OPTIMAL             = 3
} MR_LD_STATE;

/*
 * define the enum for logical drive encryption type
 */
typedef enum
{
    MR_LD_ENCRYPTION_TYPE_NONE = 0,     // No encryption is used
    MR_LD_ENCRYPTION_TYPE_FDE  = 1,     // FDE drive encryption
    MR_LD_ENCRYPTION_TYPE_CTRL = 2      // Controller-based encryption
} MR_LD_ENCRYPTION_TYPE;

/*
 * define the enum for SCSI protection type
 */
typedef enum 
{
    MR_PROT_INFO_TYPE_NONE = 0,         // No SCSI protection type used
    MR_PROT_INFO_TYPE_1 = 1,            // SCSI PI Type 1
    MR_PROT_INFO_TYPE_2 = 2,            // SCSI PI Type 2
    MR_PROT_INFO_TYPE_3 = 3,            // SCSI PI Type 3
    MR_PROT_INFO_TYPE_RES_4 = 4,        // Reserved SCSI PI Type 4
    MR_PROT_INFO_TYPE_RES_5 = 5,        // Reserved SCSI PI Type 5
    MR_PROT_INFO_TYPE_RES_6 = 6,        // Reserved SCSI PI Type 6
    MR_PROT_INFO_TYPE_RES_7 = 7,        // Reserved SCSI PI Type 7
    // Subsequent values are only valid for LDs
    MR_PROT_INFO_TYPE_CONTROLLER = 8,   // Proprietary method - LD is type 0, but controller appends and strips PI
} MR_PROT_INFO_TYPE;


/*
 * define the enum for logical drive power saving params
 */
typedef enum 
{
    MR_LD_PS_PARAMS_SPINUP60        = 0x01,     // LD can spin up in 60 seconds, recommended for max power savings
    MR_LD_PS_PARAMS_T10SUPPORT      = 0x02,     // LD has drives that support T10 power conditions
    MR_LD_PS_PARAMS_MAX_WITHCACHE   = 0x04,     // LD's IO profile support MAX power savings with cached writes
} MR_LD_PS_PARAMS;

/*
 * define the enum for logical drive power saving policy
 */
typedef enum 
{
    MR_LD_PS_POLICY_CTRL_DEFAULT    = 0x00,     // LD shall use controller default power saving policy
    MR_LD_PS_POLICY_AUTO            = 0x01,     // LD power savings are managed by FW (will use T10 power conditions and max as appropriate)
    MR_LD_PS_POLICY_MAX             = 0x02,     // LD should use MAX power savings (not recommended if MR_LD_PS_PARAMS_SPINUP60=0)
    MR_LD_PS_POLICY_DONTCACHE       = 0x04,     // LD shall not cache writes to maximise power savings
    MR_LD_PS_POLICY_T10             = 0x08,     // LD should use T10 power conditions for power savings
    MR_LD_PS_POLICY_NONE            = 0xfe,     // LD power savings are disabled
    MR_LD_PS_POLICY_RESERVED        = 0xff,     // This value is reserved
} MR_LD_PS_POLICY;

/*
 * defines the logical drive properties structure
 */
typedef struct __attribute__ ((packed))
{
    MR_LD_REF   ldRef;              // LD reference
    char    name[16];               // LD name (NULL-terminated, so max length is 15+NULL)
    BYTE      defaultCachePolicy;     // default cache policy settings (MR_LD_CACHE)
    BYTE      accessPolicy;           // access policy settings (MR_LD_ACCESS)
    BYTE      diskCachePolicy;        // physical disk cache policy (MR_PD_CACHE)
    BYTE      currentCachePolicy;     // current cache policy settings (MR_LD_CACHE) - can differ from default due to bad battery
    BYTE      noBGI;                  // set TRUE to disable BGI on this LD
#ifndef MFI_BIG_ENDIAN
    BYTE      autoSnapshot    : 1;    // set TRUE to enable auto Snapshot
    BYTE      disableAutoDeleteOldestPIT  : 1;     // Auto delete oldest PIT, 0 = enabled, 1 - disabled. FW will also auto delete View if
                                                 // one exists on this PiT
    BYTE      reserved        : 6;
#else
    BYTE      reserved        : 6;
    BYTE      disableAutoDeleteOldestPIT  : 1;
    BYTE      autoSnapshot    : 1;
#endif  // MFI_BIG_ENDIAN

    BYTE      defaultPSPolicy;        // power savings policy set for this LD (bitmap of MR_LD_PS_POLICY)
    BYTE      currentPSPolicy;        // current power savings mode for this LD (bitmap of MR_LD_PS_POLICY)
    BYTE      reserved1[4];           // pad
} MR_LD_PROPERTIES;                 // 32 bytes

/*
 * defines the logical drive parameters structure
 */
typedef struct __attribute__ ((packed))
{
    BYTE      PRL;                    // DDF - primay raid level
    BYTE      RLQ;                    // DDF - raid level qaulifier
    BYTE      SRL;                    // DDF - secondary raid level
    BYTE      stripeSize;             // Strip_Size per DDF (0=512, 1=1K, 2=2K, 3=4K, 7=64K, 11=1MB, etc.)  -  (should be named stripSize)
    BYTE      numDrives;              // number of drivers per span
    BYTE      spanDepth;              // number of spans. If only one array then it is considered as 1 span
    BYTE      state;                  // logical drive state (MR_LD_STATE)
    BYTE      initState;              // 0=no init, 1=quick init, 2=full init
    BYTE      isConsistent;           // 0=LD not consistent (or n/a), 1=LD is consistent
    BYTE      isMorphing;             // 0=LD is not morphing, 1=LD is morphing (CE/RLM is active)
    BYTE      encryptionType;         // logical drive encryption type (MR_LD_ENCRYPTION_TYPE)
    BYTE      isCachePinned;          // 0=LD doesn't have pinned cache, 1=LD has pinned controller cache
    BYTE      badBlocksExist;         // 0=LD bad block table is empty, 1=LD bad block table is not empty
    BYTE      snapshotEnabled;        // 0=snapshot is not enabled, 1=snapshot is enabled
    BYTE      notReady;               // 1=LD is not ready to process OS requests
    BYTE      isSSCD;                 // 1=LD is used for Secondary Cache
    BYTE      powerSavingOptions;     // Parameters controlling power savings for this LD (bitmap of MR_LD_PS_PARAMS)
    BYTE      piType;                 // logical drive protection type (MR_PROT_INFO_TYPE)
    BYTE      accessPolicyStatus;     // Status bits indicating reason for MR_LD_ACCESS (MR_LD_ACCESS_STATUS)
    BYTE      currentAccessPolicy;    // current access policy (MR_LD_ACCESS)
                                    // see accessPolicyStatus if different from MR_LD_PROPERTIES.accessPolicy
    BYTE      rsvd2[12];              // pad
} MR_LD_PARAMETERS;                 // 32 bytes


/*
 * defines the logical drive progress structure
 */
typedef struct __attribute__ ((packed))
{
    struct __attribute__ ((packed))
	{
#ifndef MFI_BIG_ENDIAN
        DWORD     cc      : 1;    // check consistency is in progress
        DWORD     bgi     : 1;    // Background Init is in progress
        DWORD     fgi     : 1;    // Foreground Init is in progress
        DWORD     recon   : 1;    // CE/RLM is in progress
        DWORD     erase   : 1;    // secure erase
        DWORD     disassociate :1;// SSC disassociate is in progress
        DWORD     locate  : 1;    // Locate 
        DWORD     reserved:25;
#else
        DWORD     reserved:25;
        DWORD     locate  : 1;    // Locate
        DWORD     disassociate :1;// SSC disassociate is in progress
        DWORD     erase   : 1;    // secure erase
        DWORD     recon   : 1;    // CE/RLM is in progress
        DWORD     fgi     : 1;    // Foreground Init is in progress
        DWORD     bgi     : 1;    // Background Init is in progress
        DWORD     cc      : 1;    // check consistency is in progress
#endif  // MFI_BIG_ENDIAN
    } active;
    union __attribute__ ((packed))
	{
        MR_PROGRESS     cc;         // progress for CC
        MR_PROGRESS     erase;      // progress for erase
    };
    MR_PROGRESS     bgi;        // progress for BGI
    MR_PROGRESS     fgi;        // progress for FGI
    MR_PROGRESS     recon;      // progress for CE/RLM
    MR_PROGRESS     disassociate; // progress for LD disassociate from SSC
                                  // If LD is an SSCD, then progress of disassociating all LDs from this SSCD
    struct __attribute__ ((packed))
	{
#ifndef MFI_BIG_ENDIAN
        DWORD     cc      : 1;    // check consistency is in pause
        DWORD     bgi     : 1;    // Background Init is in pause
        DWORD     fgi     : 1;    // Foreground Init is in pause
        DWORD     recon   : 1;    // CE/RLM is in pause
        DWORD     erase   : 1;    // secure erase in pause
        DWORD     disassociate :1;// SSC disassociate is in pause
        DWORD     reserved:26;
#else
        DWORD     reserved:26;
        DWORD     disassociate :1;// SSC disassociate is in pause
        DWORD     erase   : 1;    // secure erase in pause
        DWORD     recon   : 1;    // CE/RLM is in pause
        DWORD     fgi     : 1;    // Foreground Init is in pause
        DWORD     bgi     : 1;    // Background Init is in pause
        DWORD     cc      : 1;    // check consistency is in pause
#endif  // MFI_BIG_ENDIAN
    } pause;

    MR_PROGRESS     reserved[2];
} MR_LD_PROGRESS;               // 4 + (4 * number of bits defined) bytes. Today it is 36 bytes


/*
 * defines the span structure
 */
typedef struct __attribute__ ((packed))
{
    QWORD     startBlock;             // span physical disk starting block
    QWORD     numBlocks;              // span physical disk block count
    WORD     arrayRef;               // array reference (index)
#ifndef MFI_BIG_ENDIAN
    BYTE      stateValid : 1;
    BYTE      state      : 7;         // span state (MR_LD_STATE)
#else
    BYTE      state      : 7;         // span state (MR_LD_STATE)
    BYTE      stateValid : 1;
#endif  // MFI_BIG_ENDIAN
    BYTE      reserved[5];
} MR_SPAN;                          // 24 bytes


/*
 * defines the logical drive config structure
 */
typedef struct __attribute__ ((packed))
{
    MR_LD_PROPERTIES    properties;
    MR_LD_PARAMETERS    params;
    MR_SPAN             span[MAX_SPAN_DEPTH];
} MR_LD_CONFIG;         // 32 +32 + (MAX_SPAN_DEPTH * 24) bytes. For 8 span, it is 256 bytes


/*
 * defines the logical drive allowed operations structure
 */
typedef struct __attribute__ ((packed))
{
#ifndef MFI_BIG_ENDIAN
    DWORD startFGI            : 1;  // Please see fullFGDisallow also
    DWORD stopFGI             : 1;
    DWORD startCC             : 1;
    DWORD stopCC              : 1;
    DWORD startRecon          : 1;
    DWORD deleteLD            : 1;
    DWORD secure              : 1;  // 1=unsecured LD, made up of FDE drives, can be secured
    DWORD enableSnapshot      : 1;  // 0=if SnapVD, OR snapVD doesn't exist OR exists but no space
    DWORD disableSnapshot     : 1;  // 0=when rollback is in progress OR snapShot is not enabled.
    DWORD rollbackPIT         : 1;  // 0=when rollback is in progress, OR PIT doesn't exist.
    DWORD createPITView       : 1;  // 1=a PIT and optionally VIEW can be created
    DWORD deleteView          : 1;  // 1=there is a View for a PIT on the VD
    DWORD enableAutoSnapshot  : 1;  // 1=auto snapshot cannot be enabled
    DWORD disableAutoSnapshot : 1;  // 0=auto snapshot cannot be disabled
    DWORD SSCCacheable        : 1;  // 1=LD can be associated with an SSC
    DWORD breakMirror         : 1;  // 1=break mirror is allowed
    DWORD joinMirror          : 1;  // 1=join mirror is allowed
    DWORD suspendCCBGI        : 1;  // 1=suspend BGI and CC operation
    DWORD resumeCCBGI         : 1;  // 1=resume BGI and CC operation
    DWORD suspendRecon        : 1;  // 1=suspend Reconstruction operation
    DWORD resumeRecon         : 1;  // 1=resume Reconstruction operation
    DWORD startSecureEraseNonSED: 1;  // 1=start: secure erase for LDs (non SED drives)
    DWORD stopSecureEraseNonSED : 1;  // 1=stop: secure erase for LDs (non SED drives)
    DWORD forceDeleteLD       : 1;  // 1=force LD delete, as normal LD delete is not allowed, or will take long time.
    DWORD fullFGDisallow      : 1;  //
                                  // startFGI fullFGDisallow             Meaning
                                  //  ---------------------------------------------------
                                  //     0	     -	         No foreground Init operation allowed
                                  //     1	     1	         Only Fast Init allowed
                                  //     1	     0	         Both fast and full foreground init allowed
    DWORD disassociatedSSC    : 1;  // 1=Disassociation from cache pool allowed, 0=not allowed
    DWORD disablePI           : 1;  // 1=disable PI allowed, there is no separate enable PI
    DWORD reserved            : 5;
#else
    DWORD reserved            : 5;
    DWORD disablePI           : 1;  // 1=disable PI allowed, there is no separate enable PI
    DWORD disassociatedSSC    : 1;  // 1=Disassociation from cache pool allowed, 0=not allowed
    DWORD fullFGDisallow      : 1;  // Described above
    DWORD forceDeleteLD       : 1;  // 1=force LD delete, as normal LD delete is not allowed, or will take long time.
    DWORD stopSecureEraseNonSED : 1;  // 1=stop: secure erase for LDs (non SED drives)
    DWORD startSecureEraseNonSED: 1;  // 1=start: secure erase for LDs (non SED drives)
    DWORD resumeRecon         : 1;  // 1=resume Reconstruction operation
    DWORD suspendRecon        : 1;  // 1=suspend Reconstruction operation
    DWORD resumeCCBGI         : 1;  // 1=resume BGI and CC operation
    DWORD suspendCCBGI        : 1;  // 1=suspend BGI and CC operation
    DWORD joinMirror          : 1;  // 1=join mirror is allowed
    DWORD breakMirror         : 1;  // 1=break mirror is allowed
    DWORD SSCCacheable        : 1;  // 1=LD can be associated with an SSC
    DWORD disableAutoSnapshot : 1;  // 0=auto snapshot cannot be disabled
    DWORD enableAutoSnapshot  : 1;  // 1=auto snapshot cannot be enabled
    DWORD deleteView          : 1;  // 1=there is a View for a PIT on the VD
    DWORD createPITView       : 1;  // 1=a PIT and optionally VIEW can be created
    DWORD rollbackPIT         : 1;  // 0=when rollback is in progress, OR PIT doesn't exist.
    DWORD disableSnapshot     : 1;  // 0=when rollback is in progress OR snapShot is not enabled.
    DWORD enableSnapshot      : 1;  // 0=if SnapVD, OR snapVD doesn't exist OR exists but no space
    DWORD secure              : 1;  // 1=unsecured LD, made up of FDE drives, can be secured
    DWORD deleteLD            : 1;
    DWORD startRecon          : 1;
    DWORD stopCC              : 1;
    DWORD startCC             : 1;
    DWORD stopFGI             : 1;
    DWORD startFGI            : 1;  // Please see fullFGDisallow also
#endif  // MFI_BIG_ENDIAN
} MR_LD_ALLOWED_OPS;

/*
 * defines the logical drive allowed operations list structure
 */
typedef struct __attribute__ ((packed))
{
    DWORD                 count;                              // count of valid entries in this list
    DWORD                 pad;
    MR_LD_ALLOWED_OPS   allowedOpsList[MAX_API_LOGICAL_DRIVES]; // Use targetId to index into this array
} MR_LD_ALLOWED_OPS_LIST;                                   // This structure is 264 bytes

/*
 * defines the logical drive information structure
 */
typedef struct __attribute__ ((packed))
{
    MR_LD_CONFIG        ldConfig;
    DWORD               size_l;                   // Total size in 512-byte blocks of the logical drive
	DWORD				size_h;				
    MR_LD_PROGRESS      progInfo;
    WORD                 clusterOwnerDeviceId;   // LD owner's device Id
    BYTE                  reconstructActive;      // TRUE when reconstruction is active on this LD
#ifndef MFI_BIG_ENDIAN
    BYTE                  snapshotRepository  : 1;    // This LD is a snapshot repository
    BYTE                  autoSnapshotEnabled : 1;    // auto Snapshot is enabled
    BYTE                  reserved1           : 6;
#else
    BYTE                  reserved1           : 6;
    BYTE                  autoSnapshotEnabled : 1;    // this LD has auto Snapshot enabled
    BYTE                  snapshotRepository  : 1;    // This LD is a snapshot repository
#endif  // MFI_BIG_ENDIAN
    BYTE                  logicalVPDPage83[64];   // Firmware generated VPD page for the LD
    MR_LD_ALLOWED_OPS   allowedOps;             // Operations allowed on this LD
    BYTE                  reserved2[12];
} MR_LD_INFO;                                   // Today it is 384 bytes

/*
 * defines the logical drive join-mirror operations
 */
typedef enum 
{             // LD join-mirror operations
    MR_LD_MIRROR_OP_NORMAL          = 0,    // Rebuild broken mirror after join-mirror
    MR_LD_MIRROR_OP_RETAIN_MIRROR   = 1,    // Use broken mirror as rebuild source after join-mirror
    MR_LD_MIRROR_OP_CREATE_VD       = 2,    // Import broken mirror as a new VD
} MR_LD_MIRROR_OP;


/*
 * defines the logical drive reconstruction (CE/RLM) structure
 */
typedef enum 
{                 // reconstruct operations
    MR_RECON_OP_NONE    = 0,                // no drives added/removed - 'a RAID level change OR OCE within the array for the LD'
    MR_RECON_OP_INSERT  = 1,                // add new drives
    MR_RECON_OP_DELETE  = 2,                // remove drives
} MR_RECON_OP;

typedef struct __attribute__ ((packed))
{
    MR_LD_REF   ldRef;                      // LD reference
    BYTE          operation;                  // reconstruct operation(type MR_RECON_OP)
    BYTE          newRaidLevel;               // new level after RLM
    BYTE          newPdCount;                 // Number of drives to be added
    union __attribute__ ((packed))
	{
        BYTE      percentExpansion;           // This today applies to MR_RECON_OP_NONE with a OCE requested within that array for the LD.
                                            // The percent is the empty space left in the array for the LD to expand. FW will adjust the
                                            // expansion size to match to the nearest multiple of RAID level row data size.
                                            // A value of of 0 and 100 will be assumed 100%
        BYTE      reserved;                   // pad
    };
    MR_PD_REF   newPd[MAX_API_ROW_SIZE];    // PD device Id's of newPdCount
} MR_RECON;                                 // 136 bytes

/*
 * define the array structure
 */
typedef struct __attribute__ ((packed))
{
    QWORD         size;                   // size of smallest PD in array (one drive's block count)
    BYTE          numDrives;              // number of drives for this array
    BYTE         reserved;               // pad
    WORD         arrayRef;               // tag for the array (may change when logical drives are deleted)
    BYTE          pad[20];                // for 64-bit alignment
    struct __attribute__ ((packed))
	{
        MR_PD_REF   ref;                // The device IDs of the drives (MR_PD_INVALID(FFFF) = missing drive)
        WORD         fwState;            // (type MR_PD_STATE) current PD state for read operations, desired PD state for CONFIG_ADD operations
        struct __attribute__ ((packed))
		{                        // enclosure info - meaningful for configuration reads only - ignored for config writes
            BYTE      pd;                 // DeviceID of containing enclosure ((U8)MR_PD_INVALID(FF) = not within an enclosure).
            BYTE      slot;               // Slot number within enclosure. If not within an enclosure, slot = (U8)ref.deviceId
                                        //
                                        // HACK: 2-Byte deviceId, is shoe horned into a byte in both these fields.
                                        // This is ok as long as MAX_API_PHYSICAL_DEVICES is 256. Notice the casts above.
        } encl;
    } pd[MAX_API_ROW_SIZE];
} MR_ARRAY; 
                            // Today for 32 MAX_ROW_SIZE, it is 256+32=288 bytes
typedef union __attribute__ ((packed))
{          // define spare type bitmap
    struct __attribute__ ((packed))
	{
#ifndef MFI_BIG_ENDIAN
        BYTE      isDedicated    : 1;     // dedicated bit
        BYTE      isRevertible   : 1;     // revertibe bit
        BYTE      isEnclAffinity : 1;     // enclosure affinity bit. Should only be set, if Spare is part of an enclosure.
        BYTE      reserved       : 5;
#else
        BYTE      reserved       : 5;
        BYTE      isEnclAffinity : 1;     // enclosure affinity bit. Should only be set, if Spare is part of an enclosure.
        BYTE      isRevertible   : 1;     // revertibe bit
        BYTE      isDedicated    : 1;     // dedicated bit
#endif  // MFI_BIG_ENDIAN
    } mrSpareType;                      // unnamed struct:  search for NO_UNNAMED_STRUCT for usage
    BYTE      type;
} MR_SPARE_TYPE;

typedef struct  __attribute__ ((packed))
{
    MR_PD_REF       ref;                // PD reference
    MR_SPARE_TYPE   spareType;          // hot-spare type qualifiers
    BYTE              reserved[2];
    BYTE              arrayCount;         // the number of arrays to which the PD can be dedicated
    WORD             arrayRef[MAX_API_ARRAYS_DEDICATED]; // arrayReference numbers of arrayCount
} MR_SPARE;                             // Today for MAX_API_ARRAYS_DEDICATED=16 it is 40 bytes

/*
 * defines the firmware RAID configuration structure
 */
typedef struct __attribute__ ((packed))
{
    DWORD             size;           // total size of the structure
    WORD              arrayCount;
    WORD             arraySize;      // Size of each array struct
    WORD             logDrvCount;
    WORD             logDrvSize;     // Size of each logical drive struct
    WORD             sparesCount;
    WORD             sparesSize;     // Size of each spare struct
    BYTE             reserved[16];   // reserved for future use - pad to 32 bytes

    MR_ARRAY        array[1];       // variable count of MR_ARRAY entries
    MR_LD_CONFIG    ld[1];          // variable count of MR_LD_CONFIG entries
    MR_SPARE        spare[1];       // variable count of MR_SPARE entries
} MR_CONFIG_DATA;

// joe --- end 9-18-2012

#define MFSEL1	0xb000000C	//Multiple Function Pin Select Register 1 (MFSEL1)

#define GP1CFG0		(0xB8003000+0x24)	//GPIO Port1 Configuration Register 0 
#define GP1CFG1		(0xB8003000+0x28)	//GPIO Port1 Configuration Register 1 
#define GP1DOUT		(0xB8003000+0x34)	//GPIO Port1 Data Output Register		
#define GP1DIN		(0xB8003000+0x38)	//GPIO Port1 Data Input Register		
 
#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)

typedef struct OEMGlobalVarStruct
{
	BYTE 						at_b_PlatformHWconfig;
	BYTE						at_b_HWmonitorType;
	DWORD						at_dw_PltformCapability;
	BYTE						at_b_HWM_NCT7904DType;		//Linda: support A2 & A4 chip
	BYTE 						at_b_CPUOffset;
	BYTE 						at_b_PreviousGpio81State;
	BYTE						at_b_SensorScanningEnable;
//	BYTE						at_b_gWorkAroundEnable;
	BYTE        				at_b_LEDTimer;
	unsigned short                  at_GPU_ID1;   // [Farida] -->
	unsigned short                  at_GPU_ID2;  
	BYTE at_GPU1;
	BYTE at_GPU2;
	BYTE at_GPU3;
	BYTE at_GPU4;  
	BYTE at_setADTReg1;
	BYTE at_setADTReg2;   // [Farida] <--
	
	St_PowerSensorVarTag        at_St_PowerSensorInfo;

	St_HDDStatusVarTag        at_St_HDDStatusInfo;	//linda --- HDD Status

	//linda begin ---
	OS_TIMER at_CPLDTimer;
	// Jack
	OS_TIMER at_FPLEDTimer;
	OS_TIMER at_UIDLEDTimer;
	
	unsigned char at_dw_CPLDStatus[4];
	DWORD 		  at_dw_CPLD_low_pos;		// joe --- 4-24-2012

	unsigned char a_b_PS_Status;
	unsigned char a_b_PS_Status_I2C[4];		// linda -- added for power supply status through i2c

	St_METempTblTag at_St_METempTbl;

	//unsigned char a_b_CPLD_INT_gpiodrv_flag;
	//linda end---

	BYTE 			at_b_ForceUpdate[2];
	OS_MUTEX			at_FanCtrlMTX;
		BYTE 					at_b_ChassisIntruValue;   // [Farida]
	BYTE						at_b_DisableSensorPolling;   // [Farida]
	unsigned short				at_u16_MiscThermalTrip;		// [Joe] 0-3 bit CPU present, 4-7 bit Overheat Occur, 8-11 bit Thermal Trip occur
	BYTE						at_b_ThermalTripStatus;
//Linda --- begin Sensor Data
	St_SMCSensorDataVarTag		SMCSensorData[100];
//Linda --- end

//Linda --- begin Memory Sensors
	St_SMCDIMMTempVarTag		SMCDIMMTempTable[32];
	St_SMCDIMMIndexTblTag		SMCDIMMIndexTbl[4];			// for 4 mem dimm
	St_SMCDIMMGpioTblTag		SMCDIMMGpioTbl[4];			// for 4 mem dimm
	St_SMCDIMMRefreshTblTag		SMCDIMMRefreshTbl[4];		// for 4 mem dimm
	BYTE						at_b_dimm_total_size;
	unsigned int				at_u32_smbus_mux_0_bmc;
	unsigned int				at_u32_smbus_mux_1_bmc;
	unsigned int				at_u32_smbus_mux_0_bmc_input;
	unsigned int				at_u32_smbus_mux_1_bmc_input;
	unsigned int				at_u32_dimm_refresh_0_bmc;
	unsigned int				at_u32_dimm_refresh_1_bmc;
//Linda --- end

//Linda --- begin 01-05-2011 flag to stop sensor polling
unsigned char					at_b_OemEnableSensor;
//Linda --- end

//Linda --- begin 01-23-2012 flag to skip ps failure false alarm
unsigned char					a_b_PS_Fail_Count;
//Linda --- end

// joe --- begin 4-12-2010
	OS_MUTEX					at_GPUMtx;
// joe --- end 4-12-2010

// joe --- begin 7-1-2010
	unsigned char				at_b_gpio13_resetin;
	unsigned char				at_b_gpio13_resetin_count;
	
	OEMX9GPUInfoType 			oemX9GPUInfo; //Jack
	// [Jack]
	unsigned char				at_w_PlatformRevNum;
	unsigned char               		at_b_IBAddOn;
	unsigned char 				at_b_HotSwapController;
	unsigned char 				at_b_FanLed_Control;
	unsigned char 				at_b_HDD_Sensor;
	unsigned char 				at_b_HDD_LSI_Sensor;	//joe  HDD is on Board
	unsigned char         at_b_HDD_LSI_AOC_Sensor;  // HDD is on Add On Card
	unsigned char 				at_b_CheckGPIO77;
  unsigned char         at_b_X9DRD_IF_LF_Check;
	unsigned char				at_b_FatTwin_or_Hadoop;
	DWORD	 				at_b_SysConfOverride;
	XeonPhi_ThrottlingType			at_b_Bit4XeonPhi[8];
	XeonPhi_ThrottlingType			at_b_Bit5XeonPhi[8];
	BYTE                                    at_b_CPLDImageBits80;
	XeonPhi_SimType				at_St_XeonPhi_Sim[8];
	
	// Move from 7904
	BYTE 					at_b_GPU_Temp[MAX_GPU_SUPPORT];
	DWORD 					at_dw_FanInitialState;
	DWORD					at_dw_InitTick;
	BYTE					at_b_Volt_Fail_Counter;
	BYTE  					at_b_KeplerGPUCore;
	BYTE					at_b_KNC_GPU_SensorNoList[6];
	BYTE 					at_b_chassis_value;
	GPU_ZONE_INFO 				at_St_zone_info_X9DRG[2];
	GPU_ZONE_INFO				at_St_zone_info_X9SRG[2];	
	GPU_ZONE_INFO 				at_St_zone_info_X9QR7_TF[2];
	GPU_ZONE_INFO 				at_St_zone_info_X9DRG_HTFPLUS[2];
	GPU_ZONE_INFO 				at_St_zone_info_X9DRFF7GP[2];
	STATUS 					at_St_gpu_state;
	St_KNC_InfoTag				at_St_KNC_Info[8];
	// [Jack] End
	
	unsigned char               		at_b_PECI_CPU_TEMP;
	unsigned char 				at_b_PECI_PRE_CPU_TEMP;
	
// joe --- end 7-1-2010

	// joe --- begin 4-24-2012
	BYTE						at_b_PROCHot0_Flag;
	BYTE						at_b_PROCHot1_Flag;
	OS_TIMER 					at_PROCHot0Timer;
	OS_TIMER 					at_PROCHot1Timer;
	// joe --- end 4-24-2012
    // For cmd line BIOS update
	DWORD						at_dw_fw_size;
	WORD						at_w_reserve_id;
	BYTE						at_bios_fw_buf[MAX_BIOS_SIZE];
	DWORD						at_dw_uploaded_size;
	DWORD						at_dw_bios_size;
	DWORD 						dw_PowerSupplyBitmap;
	DWORD 						u32PowerSupplyFlagBitmap;
	BYTE						u8StoragePageEnable;
	WORD                        u16UploadFileReserveID;
	DWORD						u32UploadFileSize;
	WORD                        u16DownloadFileReserveID;
	DWORD						u32DownloadFileSize;
	BYTE 						u8PrepareFileDone;
	BYTE 						b_OOBDebug;
	BYTE 						b_OOBChecksum[MAX_OOB_FILE_NUM];
	BYTE 						b_OOBChecksumFlag[MAX_OOB_FILE_NUM];
	BYTE 						b_OOBFileUpdateStatus[MAX_OOB_FILE_NUM];
	BYTE 						b_OOBFileAvailable[MAX_OOB_FILE_NUM];
	DWORD						dw_OOBFileSize[MAX_OOB_FILE_NUM];
	OS_MUTEX					at_SaveBIOSFileMutex[MAX_OOB_FILE_NUM];
	OS_MUTEX					at_RestoreBIOSFileMutex[MAX_OOB_FILE_NUM];
        OS_MUTEX					at_RestoreFactoryMutex;
	BYTE						at_b_HWM_FAN_init_done;	// joe --- 8-5-2012
	// joe --- begin 8-10-2012 for multi-node PS
	BYTE						u8PSInitFlag;
	OS_TIMER 					at_PSInitTimer;
	PSFru_t						PSFru[TOTAL_PS];
	BYTE						at_b_AllPSFruDone;
	BYTE						at_b_PSDelay;
	OS_TIMER 					at_PSMonitorTimer;
	BYTE						at_b_PSTimerOverLimit;
	BYTE						at_b_lastPSNum;
	BYTE						at_b_lastPSLiveDataPos;
	// joe --- end 8-10-2012

	/* Tony, 09/05/2012 Add motherboard mac address { */	
	St_SMCHWInfoVarTag			at_SMCHWInfo_cur;	
	/* Tony, 09/05/2012 Add motherboard mac address } */
	/* Tony, 10/23/2012 add battery backup power { */
	BYTE						at_b_PowerSupplyType;	//0 is power supply, 1 is battery (BBP)
	BYTE						at_b_BBPTemp;
	BYTE						at_b_BBPPowerStatus;
	BYTE						at_b_BBPRemainBattEnergy;
	WORD						at_w_BBPBattVoltage;
	short						at_w_BBPBattCurrent;
	WORD						at_w_BBPCycleCount;
	BYTE						at_b_BBPDischargeResponse;
	BYTE						at_b_BBPDischargeCommand;	
        /* Tony, 10/23/2012 add battery backup power } */

	// joe --- begin 9-18-2012 for HDD
	OS_MUTEX					at_HDDMutex;
	BYTE						at_b_HDDEnclosureIndex[MAX_ENCLOSURE*2];
	DWORD						at_dw_PrevHDDExistBitMap[MAX_ENCLOSURE];
	DWORD						at_dw_HDDExistBitMap[MAX_ENCLOSURE];
	DWORD						at_dw_HDDErrBitMap[MAX_ENCLOSURE];
	DWORD						at_dw_PrevHDDErrBitMap[MAX_ENCLOSURE];
	DWORD						at_dw_HDDRebuildBitMap[MAX_ENCLOSURE];
	DWORD						at_dw_PrevHDDRebuildBitMap[MAX_ENCLOSURE];
	HDDInfo_t					at_HDDInfo[MAX_HDD_SUPPORT];
	OS_TIMER 					at_HDDTimer;
	LogHDDInfo_t					at_LogHDDInfo[MAX_API_LOGICAL_DRIVES];
	DWORD						at_dw_LogHDDTargetIdExistBitMap[2];
	OS_MUTEX					at_HDDGPUMutex;
	DWORD						at_dw_CurLogHDDDegradedBitMap[2];
	DWORD						at_dw_PrevLogHDDDegradedBitMap[2];
	BYTE						at_b_HDDErrCount;
	// joe --- end 9-18-2012
	
	// Jack >>>
	BYTE						at_b_GPUErrCount[6];
	BYTE						at_b_LRZoneFailStatus;
	DWORD						at_dw_KNCStatus;
	BYTE						at_b_NoOfAvailableGPU;
	BYTE 						at_b_Simulation_XeonPhiTemp;
	// Jack <<<
	
	/* Tony, 12/24/2012 add bbp timeout when all power supply are failed { */
	OS_TIMER 					at_PowerFailTimer;
	// Jack: Move to OEM PS WORD						at_w_PSTimeoutValue;
	WORD						at_w_PSTimeoutCurrentValue;
	BYTE						at_b_PSNumber;
	BYTE						at_b_BBPNumber;
	WORD						at_w_ACInputPower;
	// Jack: Move to OEM PS BYTE						at_b_PSTimeoutEnable;	
	/* Tony, 12/24/2012 add bbp timeout when all power supply are failed } */	
	/* Tony, 01/03/2013 Add power source info { */
	BYTE						at_bb_PSInfoValue[16][2];
	BYTE						at_bb_PSInfoIndex[2][16];
	BYTE						at_bb_PSInfoName[16];
	BYTE						at_b_PSSlaveAddr[4];
	BYTE						at_b_PSType[4];	//0 is power supply, 1 is bbp
	BYTE						at_b_PSStatusByte[4];
	/* Tony, 01/03/2013 Add power source info } */
	/* Tony, 01/14/2013 Add ps oob  { */
	OS_MUTEX					at_PSOOBFileMutex;
	/* Tony, 01/14/2013 Add ps oob  } */
	//Miko , 01072013 add for power comsumption monitor
	WORD						at_w_BBPTimeOut;

	// joe added heartbeat timer for production
	OS_TIMER					at_HeartBeatLedTimer;

	// joe added for Ghose fan nosie issue 6-24-2013
	BYTE						at_dw_FanInitialStateCount[32];
	

}St_OEMGlobalVarTag;

#define at_dw_FanInitialStateCount	at_p_St_OEMGlobalVar.at_dw_FanInitialStateCount	// joe added for ghost fan noise issue

#define at_HeartBeatLedTimer	at_p_St_OEMGlobalVar.at_HeartBeatLedTimer	// joe added heartbeat timer for production



// joe --- begin 9-18-2012 for HDD
#define at_HDDGPUMutex					at_p_St_OEMGlobalVar.at_HDDGPUMutex
#define at_HDDMutex						at_p_St_OEMGlobalVar.at_HDDMutex
#define at_HDDTimer						at_p_St_OEMGlobalVar.at_HDDTimer
#define at_b_HDDEnclosureIndex			at_p_St_OEMGlobalVar.at_b_HDDEnclosureIndex
#define at_dw_PrevHDDExistBitMap		at_p_St_OEMGlobalVar.at_dw_PrevHDDExistBitMap
#define at_dw_HDDExistBitMap			at_p_St_OEMGlobalVar.at_dw_HDDExistBitMap
#define at_dw_HDDErrBitMap				at_p_St_OEMGlobalVar.at_dw_HDDErrBitMap
#define at_dw_PrevHDDErrBitMap			at_p_St_OEMGlobalVar.at_dw_PrevHDDErrBitMap
#define at_dw_HDDRebuildBitMap			at_p_St_OEMGlobalVar.at_dw_HDDRebuildBitMap
#define at_dw_PrevHDDRebuildBitMap		at_p_St_OEMGlobalVar.at_dw_PrevHDDRebuildBitMap
#define at_HDDInfo						at_p_St_OEMGlobalVar.at_HDDInfo
#define at_LogHDDInfo					at_p_St_OEMGlobalVar.at_LogHDDInfo
#define at_dw_LogHDDTargetIdExistBitMap	at_p_St_OEMGlobalVar.at_dw_LogHDDTargetIdExistBitMap
#define at_dw_CurLogHDDDegradedBitMap	at_p_St_OEMGlobalVar.at_dw_CurLogHDDDegradedBitMap
#define at_dw_PrevLogHDDDegradedBitMap	at_p_St_OEMGlobalVar.at_dw_PrevLogHDDDegradedBitMap
#define at_b_HDDErrCount				at_p_St_OEMGlobalVar.at_b_HDDErrCount
// joe --- end 9-18-2012

// joe --- begin 8-10-2012 for multi-node PS
#define at_b_lastPSNum		  			at_p_St_OEMGlobalVar.at_b_lastPSNum
#define at_b_lastPSLiveDataPos 			at_p_St_OEMGlobalVar.at_b_lastPSLiveDataPos
#define at_b_PSTimerOverLimit  			at_p_St_OEMGlobalVar.at_b_PSTimerOverLimit
#define at_PSMonitorTimer     			at_p_St_OEMGlobalVar.at_PSMonitorTimer
#define at_b_PSDelay					at_p_St_OEMGlobalVar.at_b_PSDelay
#define at_b_AllPSFruDone				at_p_St_OEMGlobalVar.at_b_AllPSFruDone

#define PSFru							at_p_St_OEMGlobalVar.PSFru
#define at_PSInitTimer       			at_p_St_OEMGlobalVar.at_PSInitTimer
#define at_b_u8PSInitFlag				at_p_St_OEMGlobalVar.u8PSInitFlag
// joe --- end 8-10-2012

#define at_b_HWM_FAN_init_done			at_p_St_OEMGlobalVar.at_b_HWM_FAN_init_done
#define at_b_PROCHot0_Flag				at_p_St_OEMGlobalVar.at_b_PROCHot0_Flag	// joe --- 4-24-2012
#define at_b_PROCHot1_Flag				at_p_St_OEMGlobalVar.at_b_PROCHot1_Flag	// joe --- 4-24-2012
#define at_PROCHot0Timer       			at_p_St_OEMGlobalVar.at_PROCHot0Timer	// joe --- 4-24-2012
#define at_PROCHot1Timer       			at_p_St_OEMGlobalVar.at_PROCHot1Timer	// joe --- 4-24-2012

#define at_p_St_OEMGlobalVar  			at_p_St_GlobalVar->at_OEMGlobalVarInfo
#define at_b_PlatformHWconfig 			at_p_St_OEMGlobalVar.at_b_PlatformHWconfig
#define at_b_HWmonitorType    			at_p_St_OEMGlobalVar.at_b_HWmonitorType
// bit 0 : support UID
// bit 1 : support Power reading 

#define at_dw_PltformCapability		  	at_p_St_OEMGlobalVar.at_dw_PltformCapability
#define at_b_HWM_NCT7904DType		 	at_p_St_OEMGlobalVar.at_b_HWM_NCT7904DType	//Linda: support A2 & A4 chip
#define at_b_CPUOffset        			at_p_St_OEMGlobalVar.at_b_CPUOffset
#define at_b_PreviousGpio81State 		at_p_St_OEMGlobalVar.at_b_PreviousGpio81State
#define at_b_SensorScanningEnable 		at_p_St_OEMGlobalVar.at_b_SensorScanningEnable
// [Jack] 
#define at_b_gWorkAroundEnable			at_p_St_OEMGlobalVar.at_b_gWorkAroundEnable //to prevent sensor polling during hwm init

#define at_b_LEDTimer         			at_p_St_OEMGlobalVar.at_b_LEDTimer
#define at_GPU_ID1  					at_p_St_OEMGlobalVar.at_GPU_ID1   // [Farida] -->
#define at_GPU_ID2  					at_p_St_OEMGlobalVar.at_GPU_ID2    
#define at_GPU1  						at_p_St_OEMGlobalVar.at_GPU1    
#define at_GPU2 						at_p_St_OEMGlobalVar.at_GPU2
#define at_GPU3  						at_p_St_OEMGlobalVar.at_GPU3
#define at_GPU4  						at_p_St_OEMGlobalVar.at_GPU4 
#define at_setADTReg1  					at_p_St_OEMGlobalVar.at_setADTReg1  
#define at_setADTReg2  					at_p_St_OEMGlobalVar.at_setADTReg2  // [Farida] <--
#define at_b_ForceUpdate 				at_p_St_OEMGlobalVar.at_b_ForceUpdate
#define at_FanCtrlMTX	 				at_p_St_OEMGlobalVar.at_FanCtrlMTX
#define at_dw_PWRConsumption  			at_p_St_OEMGlobalVar.at_St_PowerSensorInfo.dw_PWRConsumption
#define at_dw_PWRInputCurrent 			at_p_St_OEMGlobalVar.at_St_PowerSensorInfo.dw_PWRInputCurrent
#define at_dw_PWRInputPower   			at_p_St_OEMGlobalVar.at_St_PowerSensorInfo.dw_PWRInputPower
#define at_b_ThermalTripStatus 			at_p_St_OEMGlobalVar.at_b_ThermalTripStatus

//linda begin --- HDD Status
#define at_dw_OverallHDDStatus  			at_p_St_OEMGlobalVar.at_St_HDDStatusInfo.dw_OverallHDDStatus
#define at_dw_PreHDDStatus  			at_p_St_OEMGlobalVar.at_St_HDDStatusInfo.dw_PreHDDStatus
#define at_dw_CurHDDStatus 				at_p_St_OEMGlobalVar.at_St_HDDStatusInfo.dw_CurHDDStatus
//linda end --- HDD status

#define at_b_IBAddOn  				at_p_St_OEMGlobalVar.at_b_IBAddOn
#define at_b_HotSwapController   		at_p_St_OEMGlobalVar.at_b_HotSwapController
#define at_b_FanLed_Control			at_p_St_OEMGlobalVar.at_b_FanLed_Control
#define at_b_HDD_Sensor  			at_p_St_OEMGlobalVar.at_b_HDD_Sensor
#define at_b_HDD_LSI_Sensor  			at_p_St_OEMGlobalVar.at_b_HDD_LSI_Sensor //joe
#define at_b_HDD_LSI_AOC_Sensor       at_p_St_OEMGlobalVar.at_b_HDD_LSI_AOC_Sensor
#define at_b_CheckGPIO77  			at_p_St_OEMGlobalVar.at_b_CheckGPIO77
#define at_b_X9DRD_IF_LF_Check        at_p_St_OEMGlobalVar.at_b_X9DRD_IF_LF_Check

#define at_b_FatTwin_or_Hadoop  		at_p_St_OEMGlobalVar.at_b_FatTwin_or_Hadoop
#define at_b_SysConfOverride			at_p_St_OEMGlobalVar.at_b_SysConfOverride
#define at_b_Bit4XeonPhi			at_p_St_OEMGlobalVar.at_b_Bit4XeonPhi
#define at_b_Bit5XeonPhi			at_p_St_OEMGlobalVar.at_b_Bit5XeonPhi
#define at_b_CPLDImageBits80			at_p_St_OEMGlobalVar.at_b_CPLDImageBits80
#define at_St_XeonPhi_Sim			at_p_St_OEMGlobalVar.at_St_XeonPhi_Sim
#define at_b_GPU_Temp				at_p_St_OEMGlobalVar.at_b_GPU_Temp
#define at_dw_FanInitialState			at_p_St_OEMGlobalVar.at_dw_FanInitialState
#define at_dw_InitTick				at_p_St_OEMGlobalVar.at_dw_InitTick
#define at_b_Volt_Fail_Counter			at_p_St_OEMGlobalVar.at_b_Volt_Fail_Counter
#define at_b_KeplerGPUCore			at_p_St_OEMGlobalVar.at_b_KeplerGPUCore
#define at_b_KNC_GPU_SensorNoList		at_p_St_OEMGlobalVar.at_b_KNC_GPU_SensorNoList
#define at_b_chassis_value			at_p_St_OEMGlobalVar.at_b_chassis_value
#define at_St_zone_info_X9DRG			at_p_St_OEMGlobalVar.at_St_zone_info_X9DRG
#define at_St_zone_info_X9SRG			at_p_St_OEMGlobalVar.at_St_zone_info_X9SRG
#define at_St_zone_info_X9QR7_TF		at_p_St_OEMGlobalVar.at_St_zone_info_X9QR7_TF
#define at_St_zone_info_X9DRG_HTFPLUS		at_p_St_OEMGlobalVar.at_St_zone_info_X9DRG_HTFPLUS
#define at_St_zone_info_X9DRFF7GP		at_p_St_OEMGlobalVar.at_St_zone_info_X9DRFF7GP
#define at_St_gpu_state				at_p_St_OEMGlobalVar.at_St_gpu_state
#define at_St_KNC_Info				at_p_St_OEMGlobalVar.at_St_KNC_Info

#define at_b_PECI_CPU_TEMP   			at_p_St_OEMGlobalVar.at_b_PECI_CPU_TEMP
#define at_b_PECI_PRE_CPU_TEMP   		at_p_St_OEMGlobalVar.at_b_PECI_PRE_CPU_TEMP



//Linda begin--- X9 GPIO--- 06-02-11
#define at_CPLDTimer	        		at_p_St_OEMGlobalVar.at_CPLDTimer
#define at_dw_CPLDStatus	        	at_p_St_OEMGlobalVar.at_dw_CPLDStatus
#define at_dw_CPLD_low_pos	        	at_p_St_OEMGlobalVar.at_dw_CPLD_low_pos	// joe --- 4-24-2012
#define at_St_oemX9GPUInfo          		at_p_St_OEMGlobalVar.oemX9GPUInfo   
//#define a_b_CPLD_INT_gpiodrv_flag	    at_p_St_OEMGlobalVar.a_b_CPLD_INT_gpiodrv_flag
//Linda end--- X9 GPIO--- 06-02-11

// [Jack] >>>
#define at_FPLEDTimer					at_p_St_OEMGlobalVar.at_FPLEDTimer
#define at_UIDLEDTimer					at_p_St_OEMGlobalVar.at_UIDLEDTimer
#define at_b_GPUErrCount				at_p_St_OEMGlobalVar.at_b_GPUErrCount
#define at_b_LRZoneFailStatus				at_p_St_OEMGlobalVar.at_b_LRZoneFailStatus
#define at_dw_KNCStatus					at_p_St_OEMGlobalVar.at_dw_KNCStatus
#define at_b_NoOfAvailableGPU				at_p_St_OEMGlobalVar.at_b_NoOfAvailableGPU
#define at_b_Simulation_XeonPhiTemp			at_p_St_OEMGlobalVar.at_b_Simulation_XeonPhiTemp

// [Jack] <<<

//Linda begin --- define var for SD3 to get ps status 07-12-11
#define a_b_PS_Status	        		at_p_St_OEMGlobalVar.a_b_PS_Status
//Linda end --- define var for SD3 to get ps stats 07-12-11

//Linda begin --- define var for SD3/app to get ps status 08-02-11
#define a_b_PS_Status_I2C	        		at_p_St_OEMGlobalVar.a_b_PS_Status_I2C
//Linda end --- define var for SD3/app to get ps stats 08-02-11

//Linda begin --- define var for ME Temp
#define at_St_METempTbl	       			at_p_St_OEMGlobalVar.at_St_METempTbl
//Linda end --- define var for ME Temp



#define at_u16_MiscThermalTrip 			at_p_St_OEMGlobalVar.at_u16_MiscThermalTrip  // joe added
#define at_b_ChassisIntruValue 			at_p_St_OEMGlobalVar.at_b_ChassisIntruValue   // [Farida] added
#define at_b_DisableSensorPolling 		at_p_St_OEMGlobalVar.at_b_DisableSensorPolling   // [Farida] added
#define at_b_gpio13_resetin 			at_p_St_OEMGlobalVar.at_b_gpio13_resetin    // [Joe] added
#define at_b_gpio13_resetin_count 		at_p_St_OEMGlobalVar.at_b_gpio13_resetin_count  // [Joe] added 

// linda --- begin Sensor Data	
#define SMCSensorData 					at_p_St_OEMGlobalVar.SMCSensorData
// linda --- end

// linda --- begin	Memory Sensors
#define SMCDIMMTempTable 				at_p_St_OEMGlobalVar.SMCDIMMTempTable
#define SMCDIMMIndexTbl					at_p_St_OEMGlobalVar.SMCDIMMIndexTbl
#define SMCDIMMGpioTbl					at_p_St_OEMGlobalVar.SMCDIMMGpioTbl	
#define SMCDIMMRefreshTbl				at_p_St_OEMGlobalVar.SMCDIMMRefreshTbl	
#define at_u32_smbus_mux_0_bmc			at_p_St_OEMGlobalVar.at_u32_smbus_mux_0_bmc
#define at_u32_smbus_mux_1_bmc			at_p_St_OEMGlobalVar.at_u32_smbus_mux_1_bmc
#define at_u32_smbus_mux_0_bmc_input	at_p_St_OEMGlobalVar.at_u32_smbus_mux_0_bmc_input
#define at_u32_smbus_mux_1_bmc_input	at_p_St_OEMGlobalVar.at_u32_smbus_mux_1_bmc_input
#define at_u32_dimm_refresh_0_bmc		at_p_St_OEMGlobalVar.at_u32_dimm_refresh_0_bmc
#define at_u32_dimm_refresh_1_bmc		at_p_St_OEMGlobalVar.at_u32_dimm_refresh_1_bmc
#define at_b_dimm_total_size			at_p_St_OEMGlobalVar.at_b_dimm_total_size

//Linda --- 01-05-2011 flag to stop sensor polling
#define at_b_OemEnableSensor			at_p_St_OEMGlobalVar.at_b_OemEnableSensor
// linda --- end 01-05-2011

//Linda --- begin 01-23-2012 flag to skip ps failure false alarm
#define a_b_PS_Fail_Count				at_p_St_OEMGlobalVar.a_b_PS_Fail_Count
// linda --- end 01-23-2012

// joe --- begin 4-12-2010
#define at_GPUMtx						at_p_St_OEMGlobalVar.at_GPUMtx
// joe --- end 4-12-2010

// [Jack]
#define at_w_PlatformRevNum 			at_p_St_OEMGlobalVar.at_w_PlatformRevNum
// For cmd line BIOS update
#define at_dw_fw_size					at_p_St_OEMGlobalVar.at_dw_fw_size
#define at_w_reserve_id					at_p_St_OEMGlobalVar.at_w_reserve_id
#define at_bios_fw_buf					at_p_St_OEMGlobalVar.at_bios_fw_buf
#define at_dw_uploaded_size				at_p_St_OEMGlobalVar.at_dw_uploaded_size
#define at_dw_bios_size					at_p_St_OEMGlobalVar.at_dw_bios_size
#define at_dw_PowerSupplyBitmap			at_p_St_OEMGlobalVar.dw_PowerSupplyBitmap
#define at_dw_PowerSupplyFlagBitmap		at_p_St_OEMGlobalVar.u32PowerSupplyFlagBitmap
#define at_w_UploadFileReserveID		at_p_St_OEMGlobalVar.u16UploadFileReserveID
#define at_dw_UploadFileSize     		at_p_St_OEMGlobalVar.u32UploadFileSize
#define at_w_DownloadFileReserveID		at_p_St_OEMGlobalVar.u16DownloadFileReserveID
#define at_dw_DownloadFileSize			at_p_St_OEMGlobalVar.u32DownloadFileSize
#define at_b_PrepareFileDone 			at_p_St_OEMGlobalVar.u8PrepareFileDone
#define at_b_OOBDebug 					at_p_St_OEMGlobalVar.b_OOBDebug
#define at_b_OOBChecksum 			    at_p_St_OEMGlobalVar.b_OOBChecksum
#define at_b_OOBChecksumFlag 			at_p_St_OEMGlobalVar.b_OOBChecksumFlag
#define	at_b_OOBFileUpdateStatus		at_p_St_OEMGlobalVar.b_OOBFileUpdateStatus
#define at_SaveBIOSFileMutex  			at_p_St_OEMGlobalVar.at_SaveBIOSFileMutex
#define at_RestoreBIOSFileMutex  		at_p_St_OEMGlobalVar.at_RestoreBIOSFileMutex
#define	at_dw_OOBFileSize				at_p_St_OEMGlobalVar.dw_OOBFileSize
#define at_b_OOBFileAvailable			at_p_St_OEMGlobalVar.b_OOBFileAvailable
#define at_RestoreFactoryMutex			at_p_St_OEMGlobalVar.at_RestoreFactoryMutex
//Jack: Add Storage Enable
#define	at_b_StoragePageEnable		at_p_St_OEMGlobalVar.u8StoragePageEnable
// :Jack

/* Tony, 09/05/2012 Add motherboard mac address { */
#define at_SMCHWInfo_cur				at_p_St_OEMGlobalVar.at_SMCHWInfo_cur
/* Tony, 09/05/2012 Add motherboard mac address } */
/* Tony, 10/23/2012 add battery backup power { */
#define at_b_PowerSupplyType			at_p_St_OEMGlobalVar.at_b_PowerSupplyType		//0 is power supply, 1 is battery (BBP) 
#define at_b_BBPTemp					at_p_St_OEMGlobalVar.at_b_BBPTemp				//BBP_REG_TEMP
#define at_b_BBPPowerStatus				at_p_St_OEMGlobalVar.at_b_BBPPowerStatus		//BBP_REG_POWER_STATUS
#define at_b_BBPRemainBattEnergy		at_p_St_OEMGlobalVar.at_b_BBPRemainBattEnergy	//BBP_REG_REAMIN_BATT_ENERGY
#define at_w_BBPBattVoltage				at_p_St_OEMGlobalVar.at_w_BBPBattVoltage	//BBP_REG_VOLTAGE_LOW_BYTE, BBP_REG_VOLTAGE_HIGH_BYTE
#define at_w_BBPBattCurrent				at_p_St_OEMGlobalVar.at_w_BBPBattCurrent	//BBP_REG_CURRENT_LOW_BYTE, BBP_REG_CURRENT_HIGH_BYTE
#define at_w_BBPCycleCount				at_p_St_OEMGlobalVar.at_w_BBPCycleCount		//BBP_REG_CYCLECOUNT_LOW_BYTE, BBP_REG_CYCLECOUNT_HIGH_BYTE
#define at_b_BBPDischargeResponse		at_p_St_OEMGlobalVar.at_b_BBPDischargeResponse	//BBP_REG_DISCHARGE_RESPONSE
#define at_b_BBPDischargeCommand		at_p_St_OEMGlobalVar.at_b_BBPDischargeCommand	//BBP_REG_DISCHARGE_COMMAND
/* Tony, 10/23/2012 add battery backup power } */
/* Tony, 12/24/2012 add bbp timeout when all power supply are failed { */
#define at_PowerFailTimer				at_p_St_OEMGlobalVar.at_PowerFailTimer
#define at_w_PSTimeoutValue				at_p_St_OEMGlobalVar.at_w_PSTimeoutValue
#define at_w_PSTimeoutCurrentValue			at_p_St_OEMGlobalVar.at_w_PSTimeoutCurrentValue
#define at_w_ACInputPower				at_p_St_OEMGlobalVar.at_w_ACInputPower
#define at_b_PSNumber					at_p_St_OEMGlobalVar.at_b_PSNumber
#define at_b_BBPNumber					at_p_St_OEMGlobalVar.at_b_BBPNumber
#define at_b_PSTimeoutEnable			at_p_St_OEMGlobalVar.at_b_PSTimeoutEnable
/* Tony, 12/24/2012 add bbp timeout when all power supply are failed } */
/* Tony, 01/03/2013 Add power source info { */
#define at_bb_PSInfoValue				at_p_St_OEMGlobalVar.at_bb_PSInfoValue
#define at_bb_PSInfoIndex				at_p_St_OEMGlobalVar.at_bb_PSInfoIndex
#define at_bb_PSInfoName				at_p_St_OEMGlobalVar.at_bb_PSInfoName
#define at_b_PSSlaveAddr				at_p_St_OEMGlobalVar.at_b_PSSlaveAddr
#define at_b_PSType						at_p_St_OEMGlobalVar.at_b_PSType
#define at_b_PSStatusByte				at_p_St_OEMGlobalVar.at_b_PSStatusByte
/* Tony, 01/03/2013 Add power source info } */
/* Tony, 01/14/2013 Add ps oob	{ */
#define at_PSOOBFileMutex				at_p_St_OEMGlobalVar.at_PSOOBFileMutex
/* Tony, 01/14/2013 Add ps oob	} */
/* Miko 01252013 add bbb timeout value*/
#define at_BBPTimeOutValue	 			at_p_St_OEMGlobalVar.at_w_BBPTimeOut

#define GPIO125_PWR_BTN						0x7D800000		// GPIO125 as output and default low
#define SMBUS_MUX_0_BMC						0x15800000		// GPIO21 output and low
#define SMBUS_MUX_1_BMC						0x16800000		// GPIO22 output and low
#define SMBUS_MUX_0_BMC_INPUT				0x15000000		// GPIO21 input
#define SMBUS_MUX_1_BMC_INPUT				0x16000000		// GPIO22 input
#define GPIO74_DOUBLE_REFRESH_OUTPUT		0x4ac00000		// GPIO74 output and high
#define GPIO75_DOUBLE_REFRESH_OUTPUT		0x4bc00000		// GPIO75 output and high
// linda --- end
#define BIOS_POST_CMPLT						0x51000000		
#define GPIO21_GPU_MUX_LOW   				0x15800000  // [Farida] ->
#define GPIO22_GPU_MUX_LOW					0x16800000
#define GPIO21_GPU_MUX_HIGH   				0x15C00000  
#define GPIO22_GPU_MUX_HIGH					0x16C00000
#define GPIO77_GPU_MUX_LOW					0x4D800000
#define GPIO77_GPU_MUX_HIGH					0x4DC00000  
#define GPIO8_SMI_MUX_HIGH					0x08C00000  
#define GPIO34_APML_MUX_LOW			    	0x22800000 
#define GPIO34_APML_MUX_HIGH				0x22C00000  // <-
#define GPIO126_APML_MUX_LOW				0x7E800000  // joe 11-30-2010
#define GPIO126_APML_MUX_HIGH				0x7EC00000  // joe 11-30-2010
//Linda begin--- X9 GPIO--- 06-02-11
#define GPIO21_SMI_MUX_HIGH					0x15C00000	//not used
//#define DIAGNOSTIC_INT					0x16800000			// defined in ChassisDriv.c
//#define GPIO7_OVT_LED_OUTPUT_LOW			0x07800000
#define GPIO7_OVT_LED_OUTPUT_LOW_OPDRAIN	0x07900000	//output low & open drain
#define GPIO7_OVT_LED_INPUT					0x07000000
#define PET_ACTION_MACRO 					OEM_PET_Action

// [Jack] For X9DRG GPIO Pin
#define GPIO16_GPU_MUX_LOW					0x10800000
#define GPIO16_GPU_MUX_HIGH					0x10C00000
#define GPIO82_GPU_MUX_LOW					0x52800000
#define GPIO82_GPU_MUX_HIGH					0x52C00000  

// [Jack] Platform Revision Number
#define GPIO66_PLATFORM_REV1				0x42000000
#define GPIO67_PLATFORM_REV2				0x43000000  

#define GPIO77_FATTWIN_OR_HANDOOP			0x4D000000

extern unsigned char OEMPostOS_Init(void);
#endif  



