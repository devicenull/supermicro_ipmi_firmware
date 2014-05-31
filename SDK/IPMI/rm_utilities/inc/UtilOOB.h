#ifndef __UTILOOB_H_
#define __UTILOOB_H_

#define OOB_DEBUG 2

#if OOB_DEBUG > 0 	
#define oob_log(fmt,args...)	console_log("[OOB %s]:" fmt, __func__, ##args)
#else
#define oob_log(fmt, ...)
#endif

enum {
	OOB_DAT,
	OOB_BIN,
	OOB_SMBIOS,
	OOB_DMITABLE
};

#define BIOS_NEW_BIN_FILE   	"/tmp/biosnvfile"
#define BIOS_CUR_BIN_FILE 		"/tmp/bios_setting"
#define BIOSDAT_FILE 			"/nv/bios_dat"
#define BIOS_CAPABILITY  		"/nv/BIOS_Caps"
#define BIOS_CUR_SMBIOS 		"/tmp/bios_old_smibios"
#define BIOS_NEW_SMBIOS 		"/tmp/bios_new_smibios"
#define BIOS_DMITABLE 		    "/nv/bios_dmitable"
#define BIOS_LICENSE_SOURCE 	"/nv/bios_license_src"
#define BIOS_LICENSE_FILE 		"/nv/bios_license"

#define MAX_OOB_FILE_NUM 4
#ifdef __UTILOOB_C_

#else 

extern int  smbios_file_checksum(char *filename);
extern int  bin_file_checksum(char *filename);
extern int  dat_file_Checksum(char *filename);
extern void oob_event_log(BYTE oob_event_id, BYTE oob_event_type,BYTE oob_status, BYTE bIsBiosGeneratedEvent);
extern void slavetask_oob_checksum(DWORD dw_event, void *ptr);
extern int get_bin_checksum(unsigned short *checksum);
extern int get_dmi_checksum(unsigned short *checksum);
extern int get_dat_checksum(unsigned short *checksum);
extern int get_smbios_checksum(unsigned short *checksum);
extern int unlock_oob_files_mutex(void);
#endif


#endif
