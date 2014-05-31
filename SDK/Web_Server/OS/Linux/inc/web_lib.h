#ifndef _WEB_LIB_H_
#define _WEB_LIB_H_

enum jnlp_const_tbl
{
	JNLP_KVM_MODE = 0,
	JNLP_SOL_MODE,
	JNLP_UNKNOWN_MODE
};

#define TEMPLATE_JNLP_KVM "../page/test.jnlp"
#define TEMPLATE_JNLP_SOL "../page/sol.jnlp"
#define READ_BUF_LENGTH 512
#define userADMIN "ADMIN"

#ifdef _WEB_LIB_C_
int readline_n(int fd, BYTE *p_b_out_str, int length);
int IPHEXToString(BYTE *p_value,BYTE *p_str);
void ConvertSpecialChars (BYTE *src,BYTE *dest);
char  *FindFilePointer(char *cginame);
int GetLength(BYTE *data,int MAX_LEN);
int MacAddrHEXToString(BYTE *addr,BYTE *output);
int GenerateJNLPContent(char *jnlp_filename, char *hash_key, char jnlp_type);
int GetUserPrivForJNLP(char *userPriv, char *hash_key);
int GetUserNameForJNLP(char *userName, char *hash_key);
char* GetPasswordForJNLP(char *userName);
WORD ByteToWordConvert(BYTE b_0, BYTE b_1);
#else
extern char  *FindFilePointer(char *cginame);
extern int GetLength(BYTE *data,int MAX_LEN);
extern int MacAddrHEXToString(BYTE *addr,BYTE *output);
extern int GenerateJNLPContent(char *jnlp_filename, char *hash_key, char jnlp_mode);
#endif
#endif
