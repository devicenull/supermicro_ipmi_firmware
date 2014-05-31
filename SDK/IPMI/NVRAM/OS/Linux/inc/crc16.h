
#ifndef _CRC16_H_
#define _CRC16_H_


#ifdef _CRC16_C_
unsigned short int CRC16Gen(
						unsigned char *pStartPtr,
						unsigned long int size
					 );
#else
extern unsigned short int CRC16Gen(
						unsigned char *pStartPtr,
						unsigned long int size
					 );
#endif
#endif






