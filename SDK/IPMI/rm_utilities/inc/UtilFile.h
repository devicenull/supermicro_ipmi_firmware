

#define FILEACTION_UPLOADCMD    0x00
#define FILEACTION_DOWNLOADCMD  0x80


typedef enum {
    /*options for upload command*/
    FILEACTION_OP_RESTORE_CONFIG		=1,
	FILEACTION_OP_RESTOR_BIOSSETING		,
	FILEACTION_OP_RESTOR_BIOSDATFILE 	,	
	FILEACTION_OP_RESTOR_SMBIOS			,	
	FILEACTION_OP_RESTOR_PS_OOB			,
	/*options for download command	*/
	FILEACTION_OP_SAVE_CONFIG			=129,
	FILEACTION_OP_SAVE_BIOSCUR_SETING,	
	FILEACTION_OP_SAVE_BIOSDATFILE	 ,    
	FILEACTION_OP_SAVE_SMBIOS		 ,   
	FILEACTION_OP_SAVE_PS_OOB 		 ,
	FILEACTION_OP_SAVE_BIOS_FW			=255,	
} ENUM_ST_FILEACTION_OP;

#ifdef __UTILFILE_C_

void RestoreConfig(WORD u16ID);
void RestoreBIOSSetting(WORD u16ID);
void SaveConfig(WORD u16ID);
void SaveBIOSConfig(WORD u16ID);
void SaveBIOSDATFile(WORD u16ID);
void RestorePSOOBBFile(WORD u16ID);
void SavePSOOBBFile(WORD u16ID);
void SaveSMBIOSFile(WORD u16ID);
void RestoreSMBIOSFile(WORD u16ID);
#else 
extern int ReadFile(BYTE *p_fileName,BYTE *p_buffer,int offset,int ChunkSize);
extern int WriteFile(BYTE *p_fileName,BYTE *p_buffer,int offset,int ChunkSize);
extern int WriteConfigFile(char *p_fileName,BYTE *data,int size,BYTE *commit);
#endif	
	
