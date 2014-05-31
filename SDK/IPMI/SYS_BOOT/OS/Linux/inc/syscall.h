
#ifndef SYSCALL_H
#define SYSCALL_H

#include <sys/shm.h>
#include <gparam.h>
#include <ps.h>



#ifdef _SYSCALL_C_


int shm_get_shmid(key_t);
int Register_IPC_Key(key_t, int);
STATUS GlobalVar_Shm_AT(St_GlobalVarTag **);
STATUS GlobalVar_Shm_AT_SMASH(St_GlobalVarTag **);
STATUS GlobalVar_Shm_DT(St_GlobalVarTag *);
STATUS PS_Shm_AT(St_PrivateStorageTag **);
STATUS PS_Shm_DT(St_PrivateStorageTag *);
STATUS GlobalVarInit(void);
STATUS PSShmInit(void);
#else 
extern int shm_get_shmid(key_t);
extern int Register_IPC_Key(key_t, int);
extern STATUS GlobalVar_Shm_AT(St_GlobalVarTag **);
extern STATUS GlobalVar_Shm_AT_SMASH(St_GlobalVarTag **);
extern STATUS GlobalVar_Shm_DT(St_GlobalVarTag *);
extern STATUS PS_Shm_AT(St_PrivateStorageTag **);
extern STATUS PS_Shm_DT(St_PrivateStorageTag *);
extern STATUS GlobalVarInit(void);
extern STATUS PSShmInit(void);
#endif
#endif


