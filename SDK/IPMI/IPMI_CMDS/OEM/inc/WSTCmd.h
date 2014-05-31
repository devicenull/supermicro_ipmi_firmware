
#ifndef _WSTCMD_H_
#define _WSTCMD_H_


#define TMP_FW_DATA "/tmp/tmpFWDATA"
#define FW_16M_SIZE	0x1000000

#define TMP_BTL_DATA "/nv/BTL"
typedef struct
{
	BYTE	SensorLUN;
	BYTE	SensorNum;
	BYTE	SensorOffset;
}SENSOROFFSET;

typedef struct {
    long       mtype;
} St_FWUploadMessageTag;
#define MSG_NORMAL	1
#define MAXMSG	(sizeof(St_FWUploadMessageTag) - sizeof(long))

enum  RBCmdType
{
	PREPARE_READ_CMD = 0,  
	PREPARE_WRITE_CMD,	   
	START_CMD,			   
	TRANSMIT_CMD,          
	RESET_CMD,			   
	CANCEL_CMD,			   
	GET_STATUS_CMD,		   
	UNKNOWN_CMD			   
};

enum RBStateType
{
	IDLE_STATE = 0,		
	PREPARE_STATE,		
	TRANSMIT_STATE,		
	UPDATING_STATE,		
	TASK_DONE_STATE,	
	TASK_FAIL_STATE,	
	INVALID_CMD_STATE,	
	SYS_BUSY_STATE		
};

enum RBDirection
{
	DIR_READ = 0,
	DIR_WRITE	
};

enum RBFileType
{
	RB_CONFIG_TYPE = 0,
	RB_FWIMG_TYPE,
	RB_SSH_AUTH_TYPE,
	RB_BIOS_UPLOAD_TYPE
};

typedef struct RBInfoStruct
{
	BYTE b_cmd;
	BYTE serv_id; 
	BYTE file_type;
	BYTE direction; 
	BYTE retry;		
	BYTE *data_buf;
	DWORD seq_num; 
	DWORD data_length;	
	BYTE bios_board_id[4]; 
	BYTE (*p_callback_func)(void *param);
	BYTE (*p_release_func)(void *param);
}RBInfoVar;

typedef struct RBFileTypeStruct
{
	BYTE b_type;
	BYTE wait_after_upd;
	BYTE *tmpfile_path;
	BYTE (*p_prepare_func)(void *param);
	BYTE (*p_transmit_func)(void *param);
	BYTE (*p_update_func)(void *param);
	BYTE (*p_release_func)(void *param);			
}RBFileTypeTag;

typedef struct {
    long       mtype;
} St_WebUploadMessageTag;


#define FILL_HDR_BYTE1(w,x,y,z) w=((x << 4)&0xf0) | \
								  ((y << 1)&0x0e) | \
							      (z & 0x1)

#define SEG_SIZE 		45
#define TIMEOUT_TIME 	30 

#define FSTATE_NORMAL			0x0
#define FSTATE_PREPARE_OK		0x1
#define FSTATE_UPDATE_OK		0x2
#define FSTATE_FAIL				0x4

#define BIOS_UPD_WAIT			0x0
#define	BIOS_UPD_NOWAIT			0x1

#define MAX_SEQ_NUM				0x01000000
#define MAX_SERVID				16

#define XML_FILE_NAME "ps.xml"
#define TARGET_FOLDER "preserve_config"


#ifdef _WSTCMD_C_
extern unsigned int compare_bios_board_id(unsigned char *new_board_id, unsigned int board_id_size);
extern int load_nvram_data(int dev_fd, unsigned char *buf_ptr);
extern int update_bios(unsigned char *buf_ptr, unsigned char *progress_arg, unsigned int interface);
extern unsigned int bios_update_pretask(int *biosupd_shmid);
extern unsigned int bios_update_posttask(int biosupd_shmid);
extern void* bios_shmat(int BIOSUpload_ShareMID);
extern void  bios_shmdt(void *bios_shm_ptr);
#else
extern CMD_STATUS OEMGetTAGName(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS OEMPortNumber(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS SendBlockInfo(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS ReceiveFWData(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetFWUpdateInfo(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetFWUpdateStatus(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS UpdateBTL(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS SendBTLInfo(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS ReceiveBTLData(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS PrepareDumpFlash(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS CheckDumpFlashComplete(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS SendFlashContent2Utility(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS DataRestoreBackupCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
#endif
#endif






