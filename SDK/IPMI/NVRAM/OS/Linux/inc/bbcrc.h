
#ifndef _BBCRC_H_
#define _BBCRC_H_


#ifdef _BBCRC_C_
unsigned short int bbCRCCal(
								unsigned long int pSectionStartPtr,
								unsigned long int pSectionEndPtr
						   );
#else
extern unsigned short int bbCRCCal(
									unsigned long int pSectionStartPtr,
									unsigned long int pSectionEndPtr
								  );
#endif
#endif






