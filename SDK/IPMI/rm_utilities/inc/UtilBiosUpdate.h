#ifndef	_UTIL_BIOS_UPDATE_H_
#define	_UTIL_BIOS_UPDATE_H_

#define BIOS_WSIZE	0x10000
#define BIOSMTDDev	"/dev/mtd7"
#define RMSBMODULE "/sbin/rmmod spiflash_bios.ko"
#define BIOS_FILE   		"/tmp/bios_rom"

void slavetask_bios_update(DWORD dw_event, void *ptr);
BYTE UtilGetBiosInfo(BYTE *biosInfo);
int spiGetBoardId(int fd, BYTE *boardId);
int spiGetBiosRevDate(int fd, BYTE *biosRevDate);



#ifndef	_UTIL_BIOS_UPDATE_C_
extern int bios_activate(const unsigned char *src ,const unsigned char *ip,const unsigned char *user);
extern int check_bioslicense();
extern int bioskey_deactivate();
#endif

extern int create_bios_license();
extern void HMAC_SHA1(unsigned char *k, int lk, unsigned char *d, int ld, unsigned char *out, int t);
#endif	// End of #ifndef _UTIL_BIOS_UPDATE_H_

