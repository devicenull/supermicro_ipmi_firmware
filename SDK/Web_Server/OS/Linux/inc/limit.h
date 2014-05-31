
#define HASHKEY_LEN				20
#define IP_STRING_LEN			(sizeof("fff.fff.fff.fff") + 1 )
#define MAC_STRING_LEN			(sizeof("FF:FF:FF:FF:FF:FF") + 1)
#define NETMASK_LEN				IP_STRING_LEN
#define PASSWORD_LEN			MAX_USER_PW_LEN
#define USERNAME_LEN			MAX_USER_NAME_LEN
#define MAX_USER_PRIV_LEVEL 	4 
#define BYTE_STR_LEN	 		(sizeof("255") + 1)   // for index, number 
#define WORD_STR_LEN	 		(sizeof("65535") + 1 )
#define VLANID_MAX_NUM			4095
#define VLANID_STR_LEN			(sizeof(4095)+1)
#define PORT_MAX_NUM			65536
#define PORT_STR_LEN			WORD_STR_LEN
#define DOMIAN_NAME_LEN     	128
#define MAX_LAN_INTERFACE   	3
#define MAX_FANMODE_NUM     	5
#define HOSTNAME_STR_LEN    	HOSTNAME_LEN
#define MAX_URL_LEN				512
#define MAX_CONTENT_TYPE_LEN  	256

#define length_check (x,y) (strlen (x) < y)
