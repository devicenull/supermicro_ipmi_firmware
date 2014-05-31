//Baidu project

#define RULE_TABLE_SAVE_PLACE1	"/nv/ipctrl/modtbl.sav"
#define IPCTRL_CMD 				"/sbin/iptables"
#define IPCTRL_SAVCMD 			"/sbin/iptables-save"
#define IPCTRL_RESTORECMD 		"/sbin/iptables-restore"

#define FW_CHAIN  "FORWARD"
#define IN_CHAIN  "INPUT"
#define OUT_CHAIN "OUTPUT"

extern void DisEnService(BYTE b_ON,WORD w_port) ;
