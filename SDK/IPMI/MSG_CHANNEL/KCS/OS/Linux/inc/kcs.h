

#ifndef _KCS_H_
#define _KCS_H_

#define IOCTL_SMS_ATN													  0x1557
#define IOCTL_SETUP_KCS_ADDRESS                                           0x1554
#define IOCTL_GET_KCS_ISR_STATUS                                          0x1555
#define IOCTL_GET_POSTCODES												  0x1556
#define KCS_IDLE      0
#define KCS_READ      1
#define KCS_WRITE     2
#define KCS_ERROR     3

#define KCS_CMD_ABORT          0x60
#define KCS_CMD_GET_STATUS     0x60
#define KCS_CMD_WRITE_START    0x61
#define KCS_CMD_WRITE_END      0x62
#define KCS_CMD_READ           0x68

#define KCS_STATUS_NO_ERROR                0x0
#define KCS_STATUS_ABORTED_BY_COMMAND      0x01
#define KCS_STATUS_ILLEGAL_CONTROL_CODE    0x02
#define KCS_STATUS_LENGTH_ERROR            0x06
#define KCS_STATUS_UNSPECIFIED_ERROR       0xFF

#define CMD_DATA            0
#define CMD_COMMAND         1
#define SMS_ATN_SET         1
#define SMS_ATN_CLEAR       0

#define KCS_ZERO_DATA           0
#define KCS_PACKAGE_MAX_SIZE    96
#define KCS_SET_ATTEN_BIT       0




typedef enum 
{
    KCS_ISR_IDLE_STATE = 0,
    KCS_ISR_RXD_STATE,
    KCS_ISR_RXDE_STATE,
    KCS_ISR_TXD_STATE,
    KCS_ISR_TXDE_STATE,
    KCS_ISR_RPS1_STATE,
    KCS_ISR_RPS2_STATE,
    KCS_ISR_ERR_STATE
} KCS_ISR_STATE_tag;


#ifdef _KCS_C_


int main(int argc, char *argv[]);
STATUS KCS_Init(BYTE);
STATUS KCS_ModuleInsert(BYTE);
STATUS KCS_Recv(BYTE, BYTE, void *, void *);
STATUS KCS_Transmit(BYTE, void *, void *);
STATUS KCS_SetMisc(BYTE b_Type, void *InitData, void *SettingData);

#else

extern STATUS KCS_Recv(BYTE, BYTE, void *, void *);       
extern STATUS KCS_Transmit(BYTE, BYTE, void *, void *);

#endif  

#endif  




