#ifndef _RADIUS_CLIENT_H_
#define _RADIUS_CLIENT_H_

#define RADIUS_SECRET_LEN 32
typedef struct
{
	unsigned char		b_RadiusEnable;
	unsigned char		b_ServerIP[4];
	unsigned short int	w_Port;
	unsigned char		b_Secret[RADIUS_SECRET_LEN];
	unsigned char		b_reserve[32];
}St_RadiusInfoTag;

extern unsigned char RADIUSAuthClient(char* host_name, int radius_port, char* secret, char* u_name, char* u_passwd, char* permissions);
#endif 
