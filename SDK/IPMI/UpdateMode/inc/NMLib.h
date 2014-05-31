#define NMCMD_MAX_RETRY	10

#ifdef _NM_LIB_C_
unsigned char me_mode_query(void);
int me_recovery_mode(void);
int me_operational_mode(void);
#else
extern unsigned char me_mode_query(void);
extern int me_recovery_mode(void);
extern int me_operational_mode(void);
#endif
