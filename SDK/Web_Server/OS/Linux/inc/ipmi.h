
#ifndef _IPMI_WEB_H_
#define _IPMI_WEB_H_


#include "msgbuf.h"
#define  XML_File_length  		 32
#define  HTML_Content_String_length  			128
#define	 Total_HTML_CONTENT_NUMBER				128
#define  XML_Request_length  	 				16

#define WEBVERSION 								0x0101         

#define POWER_STATUS  0
#define POWER_CONTROL 1
#define POWER_OFF     0
#define POWER_ON      1
#define POWER_CYCLE   2
#define POWER_RESET   3
#define POWER_SOFT_OFF	5
#define SSL_VALIDATE_DATE_LEN 20
#define isleap(y)     (((y) % 4) == 0 && (((y) % 100) != 0 || ((y) % 400) == 0))	
#define LEAPS_THRU_END_OF(y)                   ((y) / 4 - (y) / 100 + (y) / 400)

#define SECSPERMIN                                                            60
#define MINSPERHOUR                                                           60
#define HOURSPERDAY                                                           24
#define DAYSPERWEEK                                                            7
#define DAYSPERNYEAR                                                         365
#define DAYSPERLYEAR                                                         366
#define SECSPERHOUR                                   (SECSPERMIN * MINSPERHOUR)
#define SECSPERDAY                            ((long) SECSPERHOUR * HOURSPERDAY)
#define MONSPERYEAR                                                           12

#define TM_THURSDAY                                                            4
#define TM_YEAR_BASE                                                        1900
#define EPOCH_WDAY                                                   TM_THURSDAY

#define HTTP_FILE_LENGTH				64
#define HTTP_COMMAND_TABLE_TAIL			0xFF
#define HTML_CONTENT_INFO_TABLE_TAIL		0xFF
#define HTML_LANGUAGE_INFO_TABLE_TAIL		0xFF

#ifdef NTP_FEATURE
#define NTP_CONFIG_FILE "/etc/ntp.conf"
#define NTP_PID_FILE "/tmp/ntpd.pid"
#define NTP_CONFIG_OTHER_SETTING "driftfile /tmp/ntp_drift"
#define NTPDATE_PATH "/bin/ntpdate"
#define NTPD_PATH "/bin/ntpd"
#endif

#define DHCP_UID_PATH "/tmp/dhcp6c_duid"
#define DHCP_UID_SIZE 16
#define DATETIME_SEM_TMPFILE "/tmp/datetime_sem_tmp"
#define DATETIME_SEM_ID 198432
#define DATETIME_SEM_FLAG 0600|IPC_CREAT|IPC_EXCL
#define TIMEZONE_FILE "/usr/local/etc/timezone"
#define DATETIME_DST_UNIT 3600  

#define NTP_SERVICE_DISABLE 0
#define NTP_SERVICE_ENABLE 1

#define READ_BUF_MAX_SIZE 32 
#define HOSTNAME_FILE "/etc/hostname_for_dhcp"


#define IPV6_CONFIG_FILE "/etc/network/netconfig"
#define IPv6_LOCK_FILE "/tmp/web_netlock"

#define IPV6_INTERFACE_SYMBOL "INTERFACE"
#define IPV6_DHCP_SYMBOL "DHCPv6EN"
#define IPV6_DEVICE_SYMBOL "DEVICE"
#define IPV6_ADDR_SYMBOL "IPV6ADDRLIST"
#define IPV6_DEFAULT_ADDR "fe80::101:2/64"

#define IPV6_ADDR_STR_LEN 64

#define IPV6_ADDR_LOOPBACK      0x0010U
#define PROC_IFINET6_PATH "/proc/net/if_inet6"
//#define MAXIPV6LEN   (sizeof "ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255"+10)
#define NON_CACHE_CTRL "Cache-Control: no-cache\r\nPragma: no-cache\r\nExpires: -1\r\n"

#define BLOCKSIZE 64
#define FOOTER_SIZE 52
#define USER_DEFINE_INFO 25
//#define MIN(x,y) (x)>(y)?(y):(x)
void BinToHex(unsigned char b_data, unsigned char *string);
void XMLGenericPaser(void *p_DataPtr,int priv);
void XMLSystemPaser(void *p_DataPtr,int priv);
void XMLSensorPaser(void *p_DataPtr,int priv);
void XMLCompletionCodePaser(void *p_DataPtr,int priv);
void XMLPowerPaser(void *p_DataPtr,int priv);
void XMLSELPaser(void *p_DataPtr,int priv);
void XMLLogPaser(void *p_DataPtr,int priv);
void XMLFWUpgradePaser(void *p_DataPtr,int priv);
void XMLSnoopingParser(void *p_DataPtr,int priv);
void XMLSSLValidateParser(void *p_DataPtr,int priv);
void XMLSSLStatusParser(void *p_DataPtr,int priv);
void XMLMouseMode(void *p_DataPtr,int priv);

void XMLConfigPaser(void *p_DataPtr,int priv);
void XMLPEFPaser(void *p_DataPtr,int priv);
void XMLAlertInfo(void *p_DataPtr,int priv);
void XMLGetSysEventLog(void *p_DataPtr,int priv);
void XMLDateTimeInfo(void *p_DataPtr,int priv);
void XMLRemoteSession(void *p_DataPtr,int priv);
void XMLDDNSParser(void *p_DataPtr,int priv);
void XMLRADIUSParser(void *p_DataPtr,int priv);
void XMLVirtualMediaShareImage(void *p_DataPtr,int priv);
void XMLUIDStatus(void *p_DataPtr,int priv);
void XMLIPV6ConfigParser(void *p_DataPtr,int priv);
void XMLMCUtype(void *p_DataPtr,int priv);
void XMLFRUInfo(void *p_DataPtr,int priv);
void XMLUploadFWVersion(void *p_DataPtr,int priv);
void XMLLockUploadFW(void *p_DataPtr,int priv);
void XMLUploadBIOS(void *p_DataPtr,int priv);
void XMLUploadBIOSWithME(void *p_DataPtr,int priv);
void XMLBIOSUpdateCheck(void *p_DataPtr,int priv);
void XMLBIOSOption(void *p_DataPtr,int priv);
void XMLBIOSRev(void *p_DataPtr,int priv);
void XMLBIOSUnlock(void *p_DataPtr,int priv);
void XMLBIOSLicenseCheck(void *p_DataPtr,int priv);
void XMLBIOSLicenseActivate(void *p_DataPtr,int priv);
void XMLCMMInfoParser(void *p_DataPtr,int priv);
void XMLBladeInfo(void *p_DataPtr,int priv);
void XMLPSUInfo(void *p_DataPtr,int priv);
void XMLBladeSELParser(void *p_DataPtr,int priv);
void XMLBladeSensorPaser(void *p_DataPtr,int priv);
void XMLConfigTFTPD(void *p_DataPtr,int priv);
void XMLActiveDirectoryParser(void *p_DataPtr,int priv);
void XMLActiveDirectoryGroupParser(void *p_DataPtr,int priv);
void XMLGETPORTINFO(void *p_DataPtr,int priv);
void XMLGetPEFGlobalAction(void *p_DataPtr,int priv);
void XMLCSRStatus(void *p_DataPtr,int priv);
void XMLGetIPCtrlRules (void *p_DataPtr,int priv);
void XMLGetFanMode(void *p_DataPtr,int priv);
void XMLGetPowerConsumptionInfo (void *p_DataPtr,int priv);
void XMLGetiKVMStatus(void *p_DataPtr,int priv);
void XMLRejectSession(void *p_DataPtr,int priv);

typedef enum {
    Nocheck,
    check
} CheckSessionTag;

typedef struct
{
    unsigned char ab_XMLString[XML_File_length];
    void (*XML_func)(void *, int );
    CheckSessionTag check_flag; 
	int 			priv;  //minimum privilege
} St_HTTP_XML_TAG;

typedef struct
{
    unsigned char b_Value;
} St_THRESHOLD_TAG;

typedef struct
{
    unsigned char *ab_CommandString;
    unsigned char *p_FilePtr;
} St_HTTP_COMMAND_TAG;



typedef struct
{
    unsigned char ab_HTMLContentString[HTML_Content_String_length];
} St_HTMLCONTENT_STRING_TABLE;


typedef struct
{
    unsigned char ab_LanguageTypeString[HTTP_FILE_LENGTH];
} St_HTMLPAGE_MULTILANGUAGE_STRING_TAG;


//typedef struct 
//{
//    En_MouseModeTag En_MouseMode;
//}mouseCtlInfo_t;

//#define ATEN_MOUSE_IOC_MAGIC  'b'
//#define ATEN_MOUSE_IOCHOTPLUG _IOW(ATEN_MOUSE_IOC_MAGIC, 1, mouseCtlInfo_t)
//#define ATEN_MOUSE_IOCGETMODE _IOR(ATEN_MOUSE_IOC_MAGIC, 2, mouseCtlInfo_t)
//#define ATEN_MOUSE_IOCSETMODE _IOW(ATEN_MOUSE_IOC_MAGIC, 3, mouseCtlInfo_t)


extern int TotalVSNum(void);

extern int GetVSdevattribute(int);

extern void GetVSClientLanAttribute(int, char *);

extern int MaxVSNum(void);

#ifndef _HTTP_WEB_LIB_
extern char  *FindFilePointer(char *);
extern int GetIPFromENV(char *, char *);
#endif


#ifndef _HTTP_CMD_Table_
extern St_HTTP_COMMAND_TAG St_HTTP_CMD[];
#endif

#ifndef _MULTI_LANGUAGE_Table_
extern St_HTMLCONTENT_STRING_TABLE HtmlContentInfoTable[][Total_HTML_CONTENT_NUMBER];
extern St_HTMLPAGE_MULTILANGUAGE_STRING_TAG  HtmlLanguageInfoTable[];
#endif



extern unsigned char PltGetUIDStatus(void);
extern void PltEnableUID(unsigned char);
extern void PltDisableUID(unsigned char);


typedef struct
{
    int tm_sec;        
    int tm_min;        
    int tm_hour;       
    int tm_mday;       
    int tm_mon;        
    int tm_year;       
    int tm_wday;       
    int tm_yday;       
    int tm_isdst;      
} St_TimeStampDecode;

#endif
