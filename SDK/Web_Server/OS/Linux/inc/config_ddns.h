#define DDNS_BUF_MAX_SIZE 64
#define XML_CONTENT_MAX_SIZE 256
#define XML_TOTAL_SIZE XML_CONTENT_MAX_SIZE + 100
#define NEW_LINE_SYMBOL "\n"
#define DDNS_CONFIG_PATH "/usr/local/etc/ddns/DDNS_CONFIG"
#define HOSTFILE_PATH "/etc/bmc_hostname"

#define FIELD_NAME_OPT "opt_ddns"
#define FIELD_NAME_DNS "dns_server"
#define FIELD_MAME_NTP "ntp_server"
#define FIELD_NAME_HN "hostname"
#define FIELD_NAME_AUTH_EN "enable_auth"
#define FIELD_NAME_KEY "keyfile"
#define FIELD_NAME_PRIVATE "privatefile"

#define CONFIG_NAME_OPT "enable=\""
#define CONFIG_NAME_DNS "dns_server=\""
#define CONFIG_MAME_NTP "ntp_server=\""
#define CONFIG_NAME_HN "hostname=\""
#define CONFIG_NAME_AUTH_EN "enable_auth=\""
#define CONFIG_NAME_KEY "keyname=\""
#define CONFIG_END "\"\n"
#define CONFIG_END_NULL ""

#define FILEPATH_KEY "/usr/local/etc/ddns/ddns.key"
#define FILEPATH_PRIVATE "/usr/local/etc/ddns/ddns.private"
#define DDNS_KEY_NAME "ddns.key"
#define END_OF_DATA_SYMBOL "---"

#define TEXTFIELD_PROC_ID 0
#define KEY_PROC_ID 1
#define PRIVATE_PROC_ID 2
#define HOSTNAME_PROC_ID 3

#define MAX_FIELD_NUM 7
#define FIELD_NAME_LEN 12
#define WRITE_MODE 0666
#define MAX_FILE_SIZE 8192 
