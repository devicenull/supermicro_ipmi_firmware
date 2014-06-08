#ifndef _SMASH_CLPXML_H_
#define _SMASH_CLPXML_H_

#define CD_CMD		0x0001
#define SHOW_CMD	0x0002
#define HELP_CMD	0x0004
#define VERSION_CMD	0x0008
#define EXIT_CMD	0x0010
#define RESET_CMD	0x0020
#define STOP_CMD	0x0040
#define START_CMD	0x0080
#define DELETE_CMD	0x0100
#define DUMP_CMD	0x0200
#define LOAD_CMD	0x0400
#define SET_CMD		0x0800 
#define CREATE_CMD	0x1000 



#define MAX_TARGES_ARRAT_SZ		(10*1024)	
#define SM_MEM_MAX_SZ			(512*1024) 
#define SM_DEF_MSG_SZ			(8*1024) 
#define FULL_PATH_NAME_MAX_LEN	(256)
#define UFCT_MAX_LEN			(128)
#define MAX_JOBID_NUM			(65535+1) 

#define SMASH_OPT_ALL			(0x800)	
#define SMASH_OPT_DESTINATION	(0x400)	
#define SMASH_OPT_DISPLAY		(0x200)
#define SMASH_OPT_EXAMINE		(0x100)
#define SMASH_OPT_FORCE			(0x080)
#define SMASH_OPT_HELP			(0x040)
#define SMASH_OPT_KEEP			(0x020)
#define SMASH_OPT_LEVEL			(0x010)
#define SMASH_OPT_OUTPUT		(0x008)
#define SMASH_OPT_SOURCE		(0x004)
#define SMASH_OPT_VERSION		(0x002)
#define SMASH_OPT_WAIT			(0x001)


typedef struct _cmd_usage_msg{
	char *cmd_name;
	char *cmd_state;
	char *usage_msg;
}CMD_USAGE_MSG_TAG;



typedef struct _cmd_status_msg{
	int		err_no;
	char 	*err_msg;
}CMD_ST_MSG_TAG;




typedef enum _sm_cmd_st{
	CMD_ST_COMPLETED =0,
	CMD_ST_SPAWNED=1,
	CMD_ST_PROC_FAILED=2,
	CMD_ST_EXEC_FAILED=3,
	CMD_ST_NO_SETTING = 255
}SM_CMD_STATUS;


typedef struct _sm_error_tbl{
	int error;
	char *err_tag;
	int  str_len;
}SM_ERROR_TBL;

typedef struct _sm_error_type_tbl{
	char *err_desc;
	int  str_len;
}SM_ERROR_TYPE_TBL;

typedef struct _sm_cim_status_tbl{
	int cimstat; 
	char *cimstat_desc;
	int str_len;
}SM_CIM_STATUS_TBL;

typedef struct _sm_severity_tbl{
	int sev_num;
	char *sev_desc;
	int str_len;
}SM_SEVERITY_TBL;


typedef struct _sm_prob_cause_tbl{
	char *probcause_desc;
	int  len;
}SM_PROB_CAUSE_TBL;



typedef enum _el_severity{
	Unknown=0,
	Low=2,
	Medium=3,
	High=4,
	Fatal=5,
	EL_SERVERITY_NO_SETTING = 255
}EL_SERVERITY;



typedef struct _cmd_status_tag_tbl{
	char *status_tag;
	unsigned int str_len;
}SM_CMD_STATUS_TAG_TBL;




#if 0
typedef struct _cmd_status_tag_tbl{
	char *status_tag;
	unsigned int str_len;
}SM_CMD_STATUS_TAG_TBL;


const SM_CMD_STATUS_TAG_TBL sm_cmd_status_tag_tbl[]={
	{"COMMAND COMPLETED",strlen("COMMAND COMPLETED")},
	{"COMMAND SPAWNED",strlen("COMMAND COMPLETED")},
	{"COMMAND PROCESSING FAILED",strlen("COMMAND PROCESSING FAILED")},
	{"COMMAND EXECUTION FAILED",strlen("COMMAND EXECUTION FAILED")}	
};
#endif

typedef struct _rsp_msg_buf{
	char *data;
	unsigned int total_sz;
	unsigned int msg_sz;
	long int job_id;
	unsigned int ppid;
	unsigned char out_fmt;
	char *ufct;
	char *ufip;
	char *inst_num;
	char *cmd;
	SM_CMD_STATUS status;		
	unsigned int error;			
	unsigned int option;		
	
	int errtype;
	int cimstat;
	EL_SERVERITY severity;
	char *cmd_help_msg;			
	int support_verbs;		
	unsigned char list_property;
	unsigned char level_state;	
	int 	probcause_num; 		
	char	*recmdaction;
	char 	*owningentity;
	char 	*message;
	int 	message_id;
	int 	message_arg;
	char	*targets;			
	char	*src_addr;			
	char	*dst_addr;			
}RSP_MSG_BUF_TAG;


#define SM_XSD_1TAB		"  "
#define SM_XSD_2TAB		"    "
#define SM_XSD_3TAB		"      "
#define SM_XSD_4TAB		"        "
#define SM_XSD_5TAB		"          "
#define SM_XSD_6TAB		"            "
#define SM_XSD_7TAB		"              "
#define SM_XSD_8TAB		"                "
#define SM_XSD_9TAB		"                  "


#define XSD_HEADER_STR "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
<response\n\
xmlns=\"http://schemas.dmtf.org/smash/commandresponse/1.0.0/dsp0224.xsd\"\n\
xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n\
xsi:schemaLocation=\"http://schemas.dmtf.org/smash/commandresponse/1.0.0/dsp0224.xsd smclp_command_response.xsd\">\n"

#define XSD_RESPONSE_TAIL	"</response>"

#define SM_XSD_CMD_S			"<command>\n"
#define SM_XSD_CMD_E			"</command>\n"
#define SM_XSD_INPUTLINE_S		"<inputline>"
#define SM_XSD_INPUTLINE_E		"</inputline>\n"
#define SM_XSD_CMDSTAT_S		"<cmdstat>\n"
#define SM_XSD_CMDSTAT_E		"</cmdstat>\n"
#define SM_XSD_STATUS_S			"<status>"
#define SM_XSD_STATUS_E			"</status>\n"
#define SM_XSD_JOB_S			"<job>\n"
#define SM_XSD_JOB_E			"</job>\n"
#define SM_XSD_JOBID_S			"<job_id>"
#define SM_XSD_JOBID_E			"</job_id>\n"
#define SM_XSD_INSTANCE_S		"<instance>\n"
#define SM_XSD_INSTANCE_E		"</instance>\n"
#define SM_XSD_UFIT_S			"<ufit>"
#define SM_XSD_UFIT_E			"</ufit>\n"
#define SM_XSD_UFIP_S			"<ufip>"
#define SM_XSD_UFIP_E			"</ufip>\n"
#define SM_XSD_TIMESTAMP_S		"<timestamp>"
#define SM_XSD_TIMESTAMP_E		"</timestamp>\n"
#define SM_XSD_JOBERR_S			"<joberr>\n"
#define SM_XSD_JOBERR_E			"</joberr>\n"
#define SM_XSD_ERRTYPE_S		"<errtype>"
#define SM_XSD_ERRTYPE_E		"</errtype>\n"
#define SM_XSD_ERRTYPEDESC_S	"<errtype_desc>"
#define SM_XSD_ERRTYPEDESC_E	"</errtype_desc>\n"
#define SM_XSD_CIMSTAT_S		"<cimstat>"
#define SM_XSD_CIMSTAT_E		"</cimstat>\n"
#define SM_XSD_CIMSTATDESC_S	"<cimstat_desc>"
#define SM_XSD_CIMSTATDESC_E	"</cimstat_desc>\n"
#define SM_XSD_SERVERITY_S		"<severity>"
#define SM_XSD_SERVERITY_E		"</severity>\n"
#define SM_XSD_STATUSTAG_S		"<status_tag>"
#define SM_XSD_STATUSTAG_E		"</status_tag>\n"

#define SM_XSD_TARGET_S		"<target>\n"
#define SM_XSD_TARGET_E		"</target>\n"
#define SM_XSD_PROPERTIES_S		"<properties>\n"
#define SM_XSD_PROPERTIES_E		"</properties>\n"
#define SM_XSD_PROPERTY_S		"<property>\n"
#define SM_XSD_PROPERTY_E		"</property>\n"
#define SM_XSD_NAME_S		"<name>"
#define SM_XSD_NAME_E		"</name>\n"
#define SM_XSD_VALUE_S		"<value>\n"
#define SM_XSD_VALUE_E		"</value>\n"
#define SM_XSD_VAL_S		"<val>"
#define SM_XSD_VAL_E		"</val>\n"
#define SM_XSD_VALSTRING_S		"<valstring>"
#define SM_XSD_VALSTRING_E		"</valstring>\n"
#define SM_XSD_EXAMINE_S		"<examine>\n"
#define SM_XSD_EXAMINE_E		"</examine>\n"
#define SM_XSD_TEXT_S			"<text>"
#define SM_XSD_TEXT_E			"</text>\n"
#define SM_XSD_VERBS_S			"<verbs>\n"
#define SM_XSD_VERBS_E			"</verbs>\n"


#define SM_XSD_CD_CMD_S				"<cd>\n"
#define SM_XSD_CD_CMD_E				"</cd>\n"
#define SM_XSD_CREATE_CMD_S			"<create>"
#define SM_XSD_CREATE_CMD_E			"</create>\n"
#define SM_XSD_DELETE_CMD_S			"<delete>\n"
#define SM_XSD_DELETE_CMD_E			"</delete>\n"
#define SM_XSD_DUMP_CMD_S			"<dump>\n"
#define SM_XSD_DUMP_CMD_E			"</dump>\n"
#define SM_XSD_EXIT_CMD_S			"<exit>\n"
#define SM_XSD_EXIT_CMD_E			"</exit>\n"
#define SM_XSD_HELP_CMD_S			"<help>\n"
#define SM_XSD_HELP_CMD_E			"</help>\n"
#define SM_XSD_LOAD_CMD_S			"<load>\n"
#define SM_XSD_LOAD_CMD_E			"</load>\n"
#define SM_XSD_RESET_CMD_S			"<reset>\n"
#define SM_XSD_RESET_CMD_E			"</reset>\n"
#define SM_XSD_SET_CMD_S			"<set>\n"
#define SM_XSD_SET_CMD_E			"</set>\n"
#define SM_XSD_SHOW_CMD_S			"<show>\n"
#define SM_XSD_SHOW_CMD_E			"</show>\n"
#define SM_XSD_START_CMD_S			"<start>\n"
#define SM_XSD_START_CMD_E			"</start>\n"
#define SM_XSD_STOP_CMD_S			"<stop>\n"
#define SM_XSD_STOP_CMD_E			"</stop>\n"
#define SM_XSD_VERSION_CMD_S		"<version>\n"
#define SM_XSD_VERSION_CMD_E		"</version>\n"
#define SM_XSD_OEMVERB_CMD_S		"<oemverb>\n"
#define SM_XSD_OEMVERB_CMD_E		"</oemverb>\n"





#if 0
typedef struct
{
	const char* cmd;
	buildin_cmd_handle handle;
} CMD_ENTRY;


const CMD_ENTRY buildin_cmd_table[] = 
{
	{"exit", 	do_exit},
	{"cd", 		do_cd},
	{"version", do_version},
	{"show", 	do_show},
	{"help", 	do_help},
	{"delete", 	do_delete},
	{"start", 	do_start},
	{"stop", 	do_stop},
	{"reset", 	do_reset},
	{0, 0}
};
#endif


int do_parse_job(char *msg_in,RSP_MSG_BUF_TAG *rsp_msg_buf);
int do_show_level_job(char *msg_in,RSP_MSG_BUF_TAG *rsp_msg_buf);
int do_parse_keyword_job(char *msg_in,RSP_MSG_BUF_TAG *rsp_msg_buf);
int do_show_level_kd_job(char *msg_in,RSP_MSG_BUF_TAG *rsp_msg_buf);
#endif
