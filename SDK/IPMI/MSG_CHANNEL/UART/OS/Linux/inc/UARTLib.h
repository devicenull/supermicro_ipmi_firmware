
#ifndef _UART_LIB_H_
#define _UART_LIB_H_

#define UART_NONFLOWCTRL		0
#define UART_HWFLOWCTRL			1
#define UART_SWFLOWCTRL			2

#ifdef _UART_LIB_C_

#define	UART_MAJOR				248
#define	UART_MINOR_OFFSET		64


#define UART_IOC_MAGIC		'k'
#define	UART_IOCSBAUD		_IO(UART_IOC_MAGIC, 11)
#define	UART_IOCSEN			_IO(UART_IOC_MAGIC, 12)
#define	UART_IOCSUN			_IO(UART_IOC_MAGIC, 13)
#define	UART_IOCSTRILV		_IO(UART_IOC_MAGIC, 14)
#define	UART_IOCSNRTS		_IO(UART_IOC_MAGIC, 15)
#define	UART_IOCSFLOCTL		_IO(UART_IOC_MAGIC, 16)
#define	UART_IOCSPMOD		_IO(UART_IOC_MAGIC, 10)
#define UART_IOCSBREAKKEY _IO(UART_IOC_MAGIC, 17)

#else
#define UARTHW_Init PltUART_Init
extern STATUS PltUART_Init(BYTE b_PhyChannel);
extern STATUS PltUART_Read(BYTE b_PhyChannel, BYTE *p_b_ptr, DWORD *p_b_length);
extern STATUS PltUART_Write(BYTE b_PhyChannel, BYTE *p_b_ptr, BYTE *p_b_length);
extern STATUS PltUART_SetBaudRate(BYTE b_PhyChannel, DWORD dw_BaudRate);
extern STATUS PltUART_Enable(BYTE b_PhyChannel);
extern STATUS PltUART_Disable(BYTE b_PhyChannel);
extern STATUS PltUART_SetFlowControl(BYTE b_PhyChannel, BYTE b_FlowCtrlType);
extern STATUS PltUART_SetSerialPortMode(BYTE b_PhyChannel, DWORD dw_SerialMode);

#endif   

#endif   


