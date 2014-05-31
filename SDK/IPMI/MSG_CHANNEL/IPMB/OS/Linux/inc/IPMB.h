#ifndef _IPMB_H_
#define _IPMB_H_

#define IOCTL_SETUP_IPMB_ADDRESS                                           0x1554
#define IOCTL_GET_IPMB_ISR_STATUS                                          0x1555

#define IPMB_RETRY_TIME_PERIOD                      6
#define IPMB_DATA_LENGTH                            64

typedef struct IPMBIndex
{
  BYTE b_PhysicalChNo;
  BYTE b_LogicalChNo;
  BYTE b_Slaveaddr;
}IPMBIndexTag;

#ifdef _IPMB_C_

STATUS IPMB_Init(BYTE);
STATUS IPMB_Recv(BYTE ,BYTE ,BYTE ,void *,void *);
STATUS IPMB_Transmit(BYTE ,void * ,void *);
STATUS IPMBTrnEncode(BYTE *, void *);
STATUS IPMBRecDecode(BYTE, void *, void *);
void IPMBGetSeqNo(void);
STATUS IPMBTask(IPMBIndexTag *);
STATUS IPMBWriteMessage(BYTE , BYTE *, BYTE );
int IPMBReadMessage(BYTE , BYTE , BYTE *);

#else

STATUS IPMBTask(IPMBIndexTag *);
STATUS IPMB_Recv(BYTE ,BYTE ,BYTE ,void *,void *);
STATUS IPMB_Transmit(BYTE ,void * ,void *);

#endif  

#endif  



