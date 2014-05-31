
#ifndef _UTILUSERCMDS_H_
#define _UTILUSERCMDS_H_

#ifdef _UTILUSERCMDS_C_

BYTE UtilGetEnabledUsrCount(BYTE b_chnlNum);
BYTE UtilGetUserID(BYTE *p_b_UserName);
void UtilGetPSMutex(void);
void UtilLoadUserInfoDefault2PS(void);
void UtilPSUserCommit(void);
void UtilPutPSMutex(void);
void UtilSetUserEnable(BYTE b_userID,BYTE b_Enabled);
BYTE UtilSetUserName(BYTE b_userID,BYTE* a_b_Name);
void UtilGetUserName(BYTE * a_b_Name, BYTE b_userID);
void UtilSetUserPassword(BYTE b_PasswordLength,BYTE b_userID,BYTE* b_Password,BYTE b_PasswdSize);
int UtilWsmanDelAccount(char *username);
int UtilWsmanAddAccount(char *username, char *passwd);
#ifdef OEM_AST2000_LOGINFO
void UtilLoadLogInfoDefault2PS(void);
#endif

#else
extern int UtilSetUserSecretPW(BYTE b_userID,BYTE *new_pw, int new_pwd_len);
extern int UtilSetUserSecretName(BYTE b_userID,BYTE *new_name, int new_len);
extern void UserInfoVolatitleInit(void);
extern BYTE UtilGetEnabledUsrCount(BYTE b_chnlNum);
extern BYTE UtilGetUserID(BYTE *p_b_UserName);
extern void UtilGetPSMutex(void);
extern void UtilLoadUserInfoDefault2PS(void);
extern void UtilPSUserCommit(void);
extern void UtilPutPSMutex(void);
extern void UtilSetUserEnable(BYTE b_userID,BYTE b_Enabled);
extern void UtilGetUserName(BYTE * a_b_Name, BYTE b_userID);
extern BYTE UtilSetUserName(BYTE b_userID,BYTE* a_b_Name);
extern void UtilSetUserPassword(BYTE b_PasswordLength,BYTE b_userID,BYTE* b_Password,BYTE b_PasswdSize);
extern int UtilWsmanDelAccount(char *username);
extern int UtilWsmanAddAccount(char *username, char *passwd);
extern unsigned char AuthRadiusProcess(char *usr_name, char *usr_pwd);
#ifdef OEM_AST2000_LOGINFO
extern void UtilLoadLogInfoDefault2PS(void);
#endif
extern int Set_enSSL_Config(Type_enSSL_Tag item, int value);
extern int Get_enSSL_Config(unsigned char item);
extern STATUS show_user_info();
extern void StrToLowercase(unsigned char *str);
#endif 
#endif 


