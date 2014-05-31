#ifndef _LDAP_CLIENT_H_
#define _LDAP_CLIENT_H_

#define LDAP_BASEDN_LEN 0xFF
#define LDAP_ENABLE		1
#define LDAP_IKVM		0
#define LDAP_VUSB		1
#define LDAP_HTTP		2
#define DOMAIN_NAME_LENGTH 128
#define ROLE_GROUP_NUM 5
#define GROUP_NAME_LENGTH 62
#define AD_SERVER_IP_NUM 3


typedef enum enSSL_Tag{
	LDAP_SSL = 0,
    AD_SSL,
    AD_Port,
    enSSL_total_item,
}Type_enSSL_Tag;

typedef struct
{
	unsigned char		b_LdapEnable;
	unsigned short int	w_Timeout;
	unsigned short int	w_Port;
	unsigned char		b_ServerIP[4];
	char				b_BaseDN[LDAP_BASEDN_LEN];
	char				b_BindID[LDAP_BASEDN_LEN];
	unsigned char		b_BindPWD[LDAP_BASEDN_LEN];
}St_LdapInfoTag;

typedef struct
{
	char				ab_GroupName[GROUP_NAME_LENGTH];
	char				ab_GroupDomainName[DOMAIN_NAME_LENGTH];
	unsigned char		b_GroupPrivilege;
}St_GroupInfo;

typedef struct
{
	unsigned char		b_ActiveDirectoryEnable;
	unsigned short int	w_Timeout; 
	unsigned char		b_ServerIP[AD_SERVER_IP_NUM][4]; 
	char				ab_UserDomainName[DOMAIN_NAME_LENGTH];
	St_GroupInfo		a_St_GroupInfo[ROLE_GROUP_NUM];
}St_ActiveDirectorsyInfoTag;
#ifndef  _UTILUSERCMDS_C_

extern unsigned char AuthLdapProcess(char *usr_name, char *usr_pwd);

#endif
#endif 
