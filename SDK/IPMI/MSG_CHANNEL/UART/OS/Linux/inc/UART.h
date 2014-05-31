

#ifndef _UART_H_
#define	_UART_H_

#define BAUD_RATE_9600                    0x06
#define BAUD_RATE_19200                   0x07
#define BAUD_RATE_38400                   0x08
#define BAUD_RATE_57600                   0x09
#define BAUD_RATE_115200                  0x0A

#define BASIC_START_CHAR				0xA0
#define BASIC_STOP_CHAR					0xA5
#define BASIC_PACKET_HANDSHAKE_CHAR  	0xA6
#define BASIC_DATA_ESCAPE_CHAR			0xAA
#define BASIC_ESCAPE_CHAR				0x1B

#define BASIC_A0						0xB0
#define BASIC_A5						0xB5
#define BASIC_AA						0xBA
#define BASIC_A6						0xB6
#define BASIC_1B						0x3B

#define BASIC_UART	    1
#define TERMINAL_UART	4

extern unsigned long at_b_UART_Pool_Size;

typedef enum
{
	UART_FREE,
	UART_ALLOCATED
} e_UARTChnlSTATUS;

typedef enum
{
	UART_RX_BUF_FREE,
	UART_RX_BUF_BUSY
} e_UARTRxBufSTATUS;

typedef struct UARTChnl
{
    unsigned char b_ChnlNo;        
    unsigned char b_phychnlno;
    unsigned char *b_p_UartRxBuf;
    e_UARTRxBufSTATUS b_UartRxBufStatus;
    unsigned short int w_RxCounter;
    unsigned char *b_p_UartTxBuf;
    unsigned char b_Set_In_Progress;
    e_UARTChnlSTATUS b_status;
    void *QPtr;
} s_UARTChnlTag;

#ifdef _UART_C_
STATUS UART_ModuleInsert(BYTE b_LogicalChNo);
STATUS UART_Transmit(
                            BYTE b_LogChanl,
                     		void *p_InitData, 
                     		void *MsgBufPtr
                          );
                          
STATUS UART_Recv(
                           	BYTE b_LogChanl,
                 			BYTE b_UARTChnlInfoIndex,
                 			void *MsgBufPtr
                         );
                         
BYTE UART_Init(
                      BYTE b_LogicalChNo
                    );
#else
extern STATUS UART_Transmit(
                            BYTE b_LogChanl,
                     		void *p_InitData, 
                     		void *MsgBufPtr
                          );
                          
extern STATUS UART_Recv(
                           	BYTE b_LogChanl,
                 			BYTE b_UARTChnlInfoIndex,
                 			void *MsgBufPtr
                         );
                         
extern BYTE UART_Init(
                      BYTE b_LogicalChNo
                    );
                    
#endif 
#endif 

