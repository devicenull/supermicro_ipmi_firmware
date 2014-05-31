#ifndef	_UTIL_BIOS_UPDATE_H_
#define	_UTIL_BIOS_UPDATE_H_

#define BIOS_WSIZE	0x10000
#define BIOSMTDDev	"/dev/mtd7"
#define INSSBMODULE "/sbin/insmod /bin/module/spiflash_bios.ko SPISelect=3" 
#define RMSBMODULE "/sbin/rmmod spiflash_bios.ko"

void UtilBiosUpdate(DWORD dw_event, void *ptr);
BYTE UtilGetBiosInfo(BYTE *biosInfo);
int spiGetBoardId(int fd, BYTE *boardId);
int spiGetBiosRevDate(int fd, BYTE *biosRevDate);

#endif	// End of #ifndef _UTIL_BIOS_UPDATE_H_

