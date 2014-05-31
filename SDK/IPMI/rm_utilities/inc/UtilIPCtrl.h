#ifndef	_UTILIPCTRL_H_
#define	_UTILIPCTRL_H_

typedef struct{
    char ipaddr[64]; 
    char policy[8];
}rule_struct;

#define MAX_IPCTRL_RULE_NO  10
#ifdef NW_FIREWALL_FEATURE
enum{
    FIREWALL_V4TYPE = 0,
    FIREWALL_V6TYPE
};

#define RULE_TABLE_SAVE_PLACE	"/nv/ipctrl/rultbl.sav"	
#define RULE_TABLE_BACKUP_PLACE "/nv/ipctrl/ruletbl.bak"
#endif

#ifdef _UTILIPCTRL_C_


#else

extern int get_ipctrl_status();
extern int get_ipctrl_current_ruleno(int ip_type);
extern int set_ipctrl_entry(int op, int ruleno,char *ip, char *netmask, int policy);
extern int get_ip_netmask(unsigned char *str,unsigned char *ip,unsigned char *netmask);
extern rule_struct  *get_ipctrl_entry(BYTE ip_type ,int ruleno);
#endif 

#endif 

