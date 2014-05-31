#ifndef _CGI_MISC_H_
#define _CGI_MISC_H_

#ifdef  _CGI_MISC_C_

#else 
extern int  readline_n(int fd, BYTE *p_b_out_str, int length);
extern int  IPHEXToString(BYTE *p_value,BYTE *p_str);
extern int  netmask_prefix_to_dot_decimal(int prefix,unsigned char *p_decimal);
extern int  cgi_adapter(int (*callback)(int ),int priv,char *buffer);
extern int  port_number_check(char *p_port);
extern void ConvertSpecialChars (BYTE *src,BYTE *dest);
extern void IPByteString2Hex(BYTE *ipstr , BYTE *ipbuf);
extern BYTE ByteString2Hex(BYTE number);
extern WORD ByteToWordConvert(BYTE b_0, BYTE b_1);
extern int HexToStr(char *src,int len,char *out);
#endif
#endif
