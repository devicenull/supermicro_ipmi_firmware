
#ifndef _FLASH_H_
#define _FLASH_H_

#define UBOOT   			0
#define	WBL					1

#define WBL_SIGNATURE_WORD   0xA0FFFF9F
#define UBOOT_IH_MAGIC		 0x27051956	

#define TOTAL_FLASH_TYPE   3
#define	SPIFLASH_MAJOR				249
#define IOCTL_SPIFLASH_INIT         0x1720
#define IOCTL_SPIFLASH_ERASEAREA    0x1721
#define IOCTL_SPIFLASH_LOADADDR	    0x1722
#define IOCTL_SPIFLASH_IMGLENGTH    0x1723
#define IOCTL_SPIFLASH_IMGCHECKSUM  0x1724


#define ST_M25P64V6P_SECTOR_SIZE	0x40000
#define ST_M25P64V6P_PAGE_SIZE	0x100
#define ST_M25P64V6P_KMALLOC_SIZE	0x10100

#define FLASH_ERASE_BLOCK_SIZE 		0x40000
#if defined(CONFIG_PLATFORM_IP1600) || defined(CONFIG_PLATFORM_IP1500)
#define FLASH_BLOCK_SIZE 			0x40000
#else
#define FLASH_BLOCK_SIZE 			0x10000
#endif

#define SEEK_SET 0
#define SEEK_CUR 1

#define FLASH_FAILURE    0
#define FLASH_SUCCESS    1

#ifdef CONFIG_CPU_WINBOND_HERMON
#define BL_DHCP_ENABLE		0x1		  
#define BL_DHCP_DISABLE		0x0		  
#endif

typedef enum
{
	SPIFLASH_DEVNUM = 0,
	NANDFLASH_DEVNUM = 1,
	NORFLASH_DEVNUM = 2
}FLASH_TYPE;

#ifdef CONFIG_CPU_WINBOND_HERMON
typedef struct _t_bootloaer_info  
{
	unsigned int length;
	unsigned int type;
	char mac0[6];
	char ip0[6];
	char mac1[6];
	char ip1[6];
	unsigned int cache;
	unsigned int dhcp;
	unsigned int net_mac;
	unsigned int phy;
	unsigned int buf_base;
	unsigned int buf_size;
	int baudrate;
	int rmii;
	unsigned int serial_no;
} tbl_info;
#endif

struct erase_info_user {
	unsigned int start;
	unsigned int length;
};

typedef struct erase_info_user erase_info_t;

struct mtd_oob_buf {
	unsigned int start;
	unsigned int length;
	unsigned char *ptr;
};

struct mtd_info_user {
	unsigned char type;
	unsigned int flags;
	unsigned int size;	 
	unsigned int erasesize;
	unsigned int writesize;
	unsigned int oobsize;   

	unsigned int ecctype;
	unsigned int eccsize;
};

struct region_info_user {
	unsigned int offset;		
	unsigned int erasesize;		
	unsigned int numblocks;		
	unsigned int regionindex;
};

struct otp_info {
	unsigned int start;
	unsigned int length;
	unsigned int locked;
};


struct nand_oobinfo {
	unsigned int useecc;
	unsigned int eccbytes;
	unsigned int oobfree[8][2];
	unsigned int eccpos[32];
};

struct nand_oobfree {
	unsigned int offset;
	unsigned int length;
};

#define MTD_MAX_OOBFREE_ENTRIES	8

struct nand_ecclayout {
	unsigned int eccbytes;
	unsigned int eccpos[64];
	unsigned int oobavail;
	struct nand_oobfree oobfree[MTD_MAX_OOBFREE_ENTRIES];
};


struct mtd_ecc_stats {
	unsigned int corrected;
	unsigned int failed;
	unsigned int badblocks;
	unsigned int bbtblocks;
};


typedef struct mtd_info_user mtd_info_t;

#define MEMGETINFO		_IOR('M', 1, struct mtd_info_user)
#define MEMERASE		_IOW('M', 2, struct erase_info_user)
#define MEMWRITEOOB		_IOWR('M', 3, struct mtd_oob_buf)
#define MEMREADOOB		_IOWR('M', 4, struct mtd_oob_buf)
#define MEMLOCK			_IOW('M', 5, struct erase_info_user)
#define MEMUNLOCK		_IOW('M', 6, struct erase_info_user)
#define MEMGETREGIONCOUNT	_IOR('M', 7, int)
#define MEMGETREGIONINFO	_IOWR('M', 8, struct region_info_user)
#define MEMSETOOBSEL		_IOW('M', 9, struct nand_oobinfo)
#define MEMGETOOBSEL		_IOR('M', 10, struct nand_oobinfo)
#define MEMGETBADBLOCK		_IOW('M', 11, loff_t)
#define MEMSETBADBLOCK		_IOW('M', 12, loff_t)
#define OTPSELECT		_IOR('M', 13, int)
#define OTPGETREGIONCOUNT	_IOW('M', 14, int)
#define OTPGETREGIONINFO	_IOW('M', 15, struct otp_info)
#define OTPLOCK			_IOR('M', 16, struct otp_info)
#define ECCGETLAYOUT		_IOR('M', 17, struct nand_ecclayout)
#define ECCGETSTATS		_IOR('M', 18, struct mtd_ecc_stats)
#define MTDFILEMODE		_IO('M', 19)

#ifdef _FLASH_LIB_C_
BYTE FLASHDevFileName[TOTAL_FLASH_TYPE][30] = {"/dev/SPIflash","/dev/NANDflash","/dev/NORflash"} ;
extern unsigned char FLASH_MTD_DevFileName[];
extern unsigned char InsSPIModuleCMD[];
#else    
extern int FlashHW_Init();
extern void flash_init(void);
extern void flash_close_node(void);
extern void flash_open_node(void);
extern int flash_get_erase_info(unsigned int *erasesize);
extern char flash_erase_area(unsigned int address, unsigned int size);
extern char flash_program_area(unsigned int address, unsigned char *buf, unsigned int size);
extern char flash_read_area(unsigned int address, unsigned char *buf, unsigned int size);
extern unsigned char search_header(unsigned char *, unsigned int);
extern unsigned char process_image(unsigned char *, unsigned int);
extern char BurnCodes(unsigned int, unsigned int, unsigned char);
//extern void dg_printf(const char *, ... );

struct MDInfo_Board{
			unsigned int  CPU_TYPE;
			unsigned int  FlashBase;											
			unsigned int  Bootloader_OFFSET;
			unsigned int  MDInfo_OFFSET;
			unsigned int  MDInfo_LEN;
			unsigned int  ACTPRJ_OFFSET;			
			unsigned int  DEFPRJ_OFFSET;		
			unsigned int  FS_OFFSET;
			unsigned int  FS_LEN;
			unsigned int  RESERVED;
			unsigned int  RESERVED2;								
		   };

typedef struct FlashInfoTableType
{
	unsigned int  FlashType;	
	unsigned int  FlashBase;	
	unsigned int  MTD_all_part_OFFSET;
	unsigned int  Bootloader_OFFSET;
	unsigned int  KERNAL_OFFSET;
	unsigned int  RootFS_OFFSET;
	unsigned int  SecRootFS_OFFSET;
	unsigned int  NVRAM_OFFSET;
	unsigned int  KERNAL_SIZE;
	unsigned int  RootFS_SIZE;
	unsigned int  SecRootFS_SIZE;
	unsigned int  NVRAM_SIZE;
	unsigned int  RESERVED;
	unsigned int  RESERVED2;	
} FlashInfoTable;

extern FlashInfoTable FlashInfo;


typedef struct t_footer		
{
  unsigned int num;
  unsigned int base;
  unsigned int length;
  unsigned int load_address;
  unsigned int exec_address;
  char name[16];
  unsigned int image_checksum;
  unsigned int signature;
  unsigned int type;
  unsigned int checksum;
} tfooter;

typedef struct 
{
    unsigned int	dw_BurnAddr;
    unsigned int	dw_EraseSize;
    unsigned int	dw_ImageSize;
    unsigned char	*p_Image;
} St_BURN_IMAGE_INFO;

#define REVERSE(d_word)		((d_word<<24) | (d_word>>24) | ((d_word&0xff00)<<8) | ((d_word&0xff0000)>>8))
#define MAX_IMAGE_NUM   8
#define UBOOT_HEADER_BOUNDARY   0x10000
#define DO1(buf)  crc = crc_table[((int)crc ^ (*buf++)) & 0xff] ^ (crc >> 8);
#define DO2(buf)  DO1(buf); DO1(buf);
#define DO4(buf)  DO2(buf); DO2(buf);
#define DO8(buf)  DO4(buf); DO4(buf);

#define UBOOT_ENV_OFFSET		0xFC0000
#define UBOOT_ENV_SIZE			0x040000
#define ENV_HEADER_SIZE			(sizeof(unsigned long))
#define ENV_SIZE 				(UBOOT_ENV_SIZE - ENV_HEADER_SIZE)

#define IH_TYPE_INVALID		0	
#define IH_TYPE_STANDALONE	1	
#define IH_TYPE_KERNEL		2	
#define IH_TYPE_RAMDISK		3	
#define IH_TYPE_MULTI		4	
#define IH_TYPE_FIRMWARE	5	
#define IH_TYPE_SCRIPT		6	
#define IH_TYPE_FILESYSTEM	7	

#define IH_NMLEN		32		
typedef struct UB_headerType     
{
	unsigned long	ih_magic;	
	unsigned long	ih_hcrc;	
	unsigned long	ih_time;	
	unsigned long	ih_size;	
	unsigned long	ih_load;	
	unsigned long	ih_ep;		
	unsigned long	ih_dcrc;	
	unsigned char	ih_os;		
	unsigned char	ih_arch;	
	unsigned char	ih_type;	
	unsigned char	ih_comp;	
	unsigned char	ih_name[IH_NMLEN];	
} UB_header;

typedef	struct UB_environment_s {
	unsigned long	crc;		
	unsigned char	data[ENV_SIZE]; 
} UB_env_t;


#ifdef _FILESYSTEM_C_

#else 

#endif  

#endif  
#endif  



