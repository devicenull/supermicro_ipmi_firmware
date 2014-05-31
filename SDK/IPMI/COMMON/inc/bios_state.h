#define FID_SIG				"$FID"  	//signature pattern
#define FID_SIG_SIZE		4			//Signature size
#define BID_OFFSET			7			//Board ID offset from FID_SIG
#define BID_SIZE			5			//Board ID length
#define BDATE_OFFSET		0x2C		//BIOS FW date

#define FVH_SIG				"_FVH"  //A signature for recognize a NVRAM block
#define FVH_SIG_SIZE		4		//Signature size
#define FVH_SIG_OFFSET		40		//Signature offset in a FVH header
#define FV_LEN_OFFSET		32		//NVRAM length field offset in a FVH header	



#define WEB_BIOS_UPDATE_TIMEOUT		300	//maximum timeout value (seconds)

typedef enum
{   	
	BIOS_NORNAL = 0,		//normal state.			(0x0)	
	BIOS_ULOAD_FAILED,		//upload failed.		(0x1)
	BIOS_FVH_NOT_FOUND,		//FVH header not found. (0x2)
	BIOS_FID_NOT_FOUND,		//FID header not found. (0x3)
	BIOS_BID_MISMATCH,		//Board ID mismatch.	(0x4)
	BIOS_LOAD_DESC_FAIL,	//load descriptor fail. (0x5)
	BIOS_ABNORMAL_SIZE,		//abnormal BIOS size.   (0x6)	
	BIOS_UPDFAIL_STATE,		//BIOS update failed.	(0x7)
//	ME_RECOVERY_MODE_FAIL,	//force ME to recovery mode failed  (0x8)
	BIOS_UPDATING_STATE,	//BIOS is updating.		(0x8)	
	BIOS_CHECK_NVRAM,		//check NVRAM.			(0x9)	
	BIOS_ERASE_FLASH,		//erase flash block		(0xa)
	BIOS_TIMEOUT,			//BIOS update timeout	(0xb)
	BIOS_LEN_MISMATCH,		//Board Length mismatch (0xd)
	BIOS_BOUNDARY_NOT_FOUND, //boundary not found   (0xe)
	BIOS_RESERVED_STATE	    //unknown state			(0xf)
} BIOS_STATUS_ENUM;

typedef enum
{
	PROGRAM_VIA_WEB = 0,	//programming the BIOS flash through WEB-UI
	PROGRAM_VIA_TOOL		//programming the BIOS flash through update tool
} BIOS_PROGRAM_METHOD;

#define FDBAR           0					//Flash Descriptor Base Address
#define FLVALSIG        (FDBAR+0x10)		//Signature base address
#define FLMAP0          (FDBAR+0x14)		//Flash Map0 Register Base Address
#define FLREG_BIOS(r)   ((r)+0x4)			//BIOS Region Base Address
#define FLREG_ME(r)     ((r)+0x8)			//ME Region Base Address

//#define REG_GETVAL(r)   ( *( (unsigned int *) (r) ) )
#define REG_GETVAL(r)   ( (unsigned int) ( (*(r)) | (*(r+1)<<8) | (*(r+2)<<16) | (*(r+3)<<24) )  )

#define VALID_SIG       0x0FF0A55A			//A signature for recognizing the valid flash descriptor
#define REG_SIZE        0x10				//register size  0x10 = 16 bytes
#define FRBA(r)         ( ((r)>>12) & 0xff0  ) //Flash Region Base Address 
//#define REVERSE_3B(v)   (( ((v)>>16) &0xff) | ( (v)&0xff00) | ( (((v)&0xff)<<16))
//#define REGION_BA(r)  REVERSE_3B( (r) & 0x1fff ) //region's base address
//#define REGION_EA(r)    REVERSE_3B( ((r)>>16) & 0x1fff )       //region's end address
#define REGION_BA(r)    (( (r) & 0x1fff )<<12)  //region's base address
#define REGION_EA(r)    ( (( (r) & 0x1fff0000 ) >> 4) + 0xfff)        //region's end address   >>4 : >>16 then << 12

#ifndef _ATEN_UPDATE_LIB_C
extern unsigned char UpdateTimeoutCheck(int timeout, unsigned char no_update);
#else
unsigned char UpdateTimeoutCheck(int timeout, unsigned char refresh);
#endif
