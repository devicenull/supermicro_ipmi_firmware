/*-------------------------------------------------------------------------
  File : wpcm450_kbd.c

  Perpose :

  Hystory :
      
-------------------------------------------------------------------------*/


/*********************************************************************************/
/* Includes                                                                      */
/*********************************************************************************/

#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ioport.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/smp_lock.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/list.h>
#include <linux/interrupt.h>
#include <linux/utsname.h>
#include <linux/wait.h>
#include <linux/proc_fs.h>
#include <linux/device.h>
#include <linux/string.h>
#include <linux/fcntl.h>	/* O_ACCMODE */
#include <linux/hdreg.h>	/* HDIO_GETGEO */
#include <linux/genhd.h>
#include <linux/blkdev.h>
#include <linux/buffer_head.h>	/* invalidate_bdev */
#include <linux/bio.h>

#include <linux/usb_ch9.h>
#include <linux/usb_cdc.h>
#include <linux/usb_gadget.h>
#include <linux/byteorder/little_endian.h>


#include <asm/cacheflush.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
#include <asm/semaphore.h>
#include <asm/hardware.h>


#include <asm/arch/map.h>
#include <asm/arch/regs-irq.h>
#include <asm/arch/regs-udc.h>


#include "../gadget_chips.h"
#include "wpcm450_devapi.h"
#include "wpcm450_usbprv_dev.h"

/*********************************************************************************/
/* Macros &  Defines                                                             */
/*********************************************************************************/
#define USB_MSTORAGE_MAJOR                  (65)
#define USB_MSTORAGE_MINORS                 (1)

#define USB_MSTORAGE_MIN(a,b)               ((a) < (b) ? (a) : (b)) 
#define USB_MSTORAGE_EP1_RECV_BUFFER_SIZE   (10)

#define WPCM450_UDC_MSTORAGE_VERSION_STR             "v1.0"
#define WPCM450_UDC_MSTORAGE_VERSION_NUM             0x0100

#define WPCM450_UDC_MSTORAGE_LONG_NAME               "WPCM450 USB Mass Storage Driver"
#define WPCM450_UDC_MSTORAGE_SHORT_NAME              "wpcm450_disk"


#define WPCM450_UDC_MSTORAGE_NOTIFY_MAXPACKET        5 /* 1 << 5 == 32 msec */
#define WPCM450_UDC_MSTORAGE_LOG2_NOTIFY_INTERVAL    8 


#define WPCM450_UDC_MSTORAGE_VENDOR_ID               0x0B20
#define WPCM450_UDC_MSTORAGE_PRODUCT_ID              0x1
#define WPCM450_UDC_MSTORAGE_NUM_CONFIGS             1
#define WPCM450_UDC_MSTORAGE_NO_CONFIG_ID            0
#define WPCM450_UDC_MSTORAGE_BULK_CONFIG_ID          1
#define WPCM450_UDC_MSTORAGE_ACM_CONFIG_ID           2
#define WPCM450_UDC_MSTORAGE_INT_CONFIG_ID           1


/* USB descriptors */

#define WPCM450_UDC_MSTORAGE_SERIAL_STR_ID           0
#define WPCM450_UDC_MSTORAGE_MANUFACTURER_STR_ID     1
#define WPCM450_UDC_MSTORAGE_PRODUCT_STR_ID          2

#define WPCM450_UDC_MSTORAGE_INT_CONFIG_STR_ID       0
#define WPCM450_UDC_MSTORAGE_CONTROL_STR_ID          4
#define WPCM450_UDC_MSTORAGE_DATA_STR_ID             5



#define WPCM450_UDC_MSTORAGE_MAX_NUM_INTERFACES      2
#define WPCM450_UDC_MSTORAGE_INT_INTERFACE_ID        0
#define WPCM450_UDC_MSTORAGE_CONTROL_INTERFACE_ID    0
#define WPCM450_UDC_MSTORAGE_DATA_INTERFACE_ID       1
#define WPCM450_UDC_MSTORAGE_NUM_ENDPOINTS           2
#define WPCM450_UDC_MSTORAGE_MAX_DESC_LEN            1500


#define WPCM450_MSTORAGE_INTERFACE_NUM      0


#define  WPCM450_UDC_MSTORAGE_CONTROL_MAX_PACKET_SIZE            (64)
#define  WPCM450_UDC_MSTORAGE_INTERRUPT_MAX_PACKET_SIZE          (8)
#define  MSTORAGE_FS_MAX_PACKET_SIZE        (64)
#define  MSTORAGE_HS_MAX_PACKET_SIZE        (512)
#define  MSTORAGE_EP                        (1)

#define WPCM450_UDC_MSTORAGE_CONFIG_DESC_SIZE                    (34)

#define WPCM450_UDC_MSTORAGE_MAX_NUM_GADGETS_PER_DEV             (1)

#define WPCM450_UDC_MSTORAGE_MAX_BUFFER_LENGTH  (sizeof(struct usb_config_descriptor)+\
            WPCM450_UDC_MSTORAGE_MAX_NUM_GADGETS_PER_DEV*sizeof(struct usb_interface_descriptor)+\
            WPCM450_UDC_MSTORAGE_MAX_NUM_GADGETS_PER_DEV*sizeof(struct usb_endpoint_descriptor) + \
            WPCM450_UDC_MSTORAGE_MAX_NUM_GADGETS_PER_DEV*sizeof(struct usb_endpoint_descriptor))

#define  USB_DCBWSIGNATURE                  (0x43425355)
#define  USB_DCSWSIGNATURE                  (0x53425355)
#define  USB_CBW_DIRECTION_BIT              (0x80)

#define  TOTAL_LOGICAL_ADDRESS_BLOCKS       (40000)
#define  LENGTH_OF_EACH_LAB                 (512)

#define USB_uint_16_low(x)                  ((x) & 0xFF)
#define USB_uint_16_high(x)                 (((x) >> 8) & 0xFF)

#define WPCM450_UDC_MSTORAGE_BUFFERSIZE      (2048)

/*
 * We can tweak our hardware sector size, but the kernel talks to us
 * in terms of small sectors, always.
 */
#define KERNEL_SECTOR_SIZE                  512

/*
 * After this much idle time, the driver will simulate a media change.
 */
#define INVALIDATE_DELAY                    30*HZ


/*********************************************************************************/
/* Types Definitions                                                             */
/*********************************************************************************/

/*
 * The different "request modes" we can use.
 */
typedef enum {
  RM_SIMPLE  = 0,	/* The extra-simple request function */
  RM_FULL    = 1,	/* The full-blown version */
  RM_NOQUEUE = 2,	/* Use make_request */
}request_mode_t;


typedef struct wpcm450_mstorage_dev {
  int size;                       /* Device size in sectors */
  __u8 *data;                     /* The data array */
  short users;                    /* How many users */
  short media_change;             /* Flag a media change? */
  spinlock_t lock;                /* For mutual exclusion */
  struct request_queue *queue;    /* The device request queue */
  struct gendisk *gd;             /* The gendisk structure */
  struct timer_list timer;        /* For simulated media changes */
  void*  handler;
}wpcm450_mstorage_dev_t;



typedef struct usb_ctrlrequest* setup_struct_ptr_t;


/* USB Command Block Wrapper */
//typedef USB_Uncached struct cbw_struct {
typedef struct cbw_struct {
   __u32  DCBWSIGNATURE;
   __u32  DCBWTAG;
   __u32  DCBWDATALENGTH;
   __u8    BMCBWFLAGS;
   /* 4 MSBs bits reserved */
   __u8    BCBWCBLUN;
   /* 3 MSB reserved */
   __u8    BCBWCBLENGTH;
   __u8    CBWCB[16];
} CBW_STRUCT;

typedef CBW_STRUCT* CBW_STRUCT_PTR;

/* USB Command Status Wrapper */
typedef struct csw_struct {
   __u32  DCSWSIGNATURE;
   __u32  DCSWTAG;
   __u32  DCSWDATARESIDUE;
   __u8    BCSWSTATUS;
} CSW_STRUCT;

typedef CSW_STRUCT* CSW_STRUCT_PTR;

/* USB Mass storage Inquiry Command */
typedef struct mass_storage_inquiry {
   __u8    OPCODE;
   __u8    LUN;
   __u8    PAGE_CODE;
   __u8    RESERVED1;
   __u8    ALLOCATION_LENGTH;
   __u8    RESERVED2[7];
} MASS_STORAGE_INQUIRY_STRUCT;

typedef MASS_STORAGE_INQUIRY_STRUCT* MASS_STORAGE_INQUIRY_PTR;

/* USB Mass storage READ CAPACITY Data */
typedef struct mass_storage_read_capacity {
   __u8    LAST_LOGICAL_BLOCK_ADDRESS[4];
   __u8    BLOCK_LENGTH_IN_BYTES[4];
} MASS_STORAGE_READ_CAPACITY_STRUCT;

typedef MASS_STORAGE_READ_CAPACITY_STRUCT* MASS_STORAGE_READ_CAPACITY_STRUCT_PTR;

/* USB Mass storage Device information */
typedef struct mass_storage_device_info {
   __u8    PERIPHERAL_DEVICE_TYPE;    /* Bits 0-4. All other bits reserved */
   __u8    RMB;                       /* Bit 7. All other bits reserved */
   __u8    ANSI_ECMA_ISO_VERSION;     /* ANSI: bits 0-2, ECMA: bits 3-5, 
                                       ** ISO: bits 6-7 
                                       */
   __u8    RESPONSE_DATA_FORMAT;      /* bits 0-3. All other bits reserved */
   __u8    ADDITIONAL_LENGTH;         /* For UFI device: always set to 0x1F */
   __u8    RESERVED1[3];
   __u8    VENDOR_INFORMATION[8];
   __u8    PRODUCT_ID[16];
   __u8    PRODUCT_REVISION_LEVEL[4];
} MASS_STORAGE_DEVICE_INFO_STRUCT;

typedef MASS_STORAGE_DEVICE_INFO_STRUCT* MASS_STORAGE_DEVICE_INFO_PTR;


/*****************************************************************************/
/* Variables                                                                 */
/*****************************************************************************/


static struct usb_device_descriptor wpcm450_usb20_device_desc = {

  .bLength            = sizeof(struct usb_device_descriptor),
  .bDescriptorType    = USB_DT_DEVICE,
  .bcdUSB             = __constant_cpu_to_le16(0x0200),
  .bDeviceClass       = 0,
  .bDeviceSubClass    = 0,
  .bDeviceProtocol    = 0,
  .bMaxPacketSize0    = WPCM450_UDC_MSTORAGE_CONTROL_MAX_PACKET_SIZE,
  .idVendor           = __constant_cpu_to_le16(WPCM450_UDC_MSTORAGE_VENDOR_ID),
  .idProduct          = __constant_cpu_to_le16(WPCM450_UDC_MSTORAGE_PRODUCT_ID),
  .bcdDevice          = __constant_cpu_to_le16(0x1),
  .iManufacturer      = WPCM450_UDC_MSTORAGE_MANUFACTURER_STR_ID,
  .iProduct           = WPCM450_UDC_MSTORAGE_PRODUCT_STR_ID,
  .iSerialNumber      = WPCM450_UDC_MSTORAGE_SERIAL_STR_ID,
  .bNumConfigurations = WPCM450_UDC_MSTORAGE_NUM_CONFIGS,

};


static struct usb_config_descriptor wpcm450_mstorage_config_desc = {
  
  .bLength             = sizeof(struct usb_config_descriptor),
  .bDescriptorType     = USB_DT_CONFIG,
  .wTotalLength        = 0, /* must be updated in run time */
  .bNumInterfaces      = 1,
  .bConfigurationValue = WPCM450_UDC_MSTORAGE_INT_CONFIG_ID,
  .iConfiguration      = WPCM450_UDC_MSTORAGE_INT_CONFIG_STR_ID,
  .bmAttributes        = USB_CONFIG_ATT_ONE | USB_CONFIG_ATT_SELFPOWER,
  .bMaxPower           = 0x1,
};

static struct usb_interface_descriptor wpcm450_mstorage_intf_desc = {
  
  .bLength            = sizeof (struct usb_interface_descriptor),
  .bDescriptorType    = USB_DT_INTERFACE,
  .bInterfaceNumber   = WPCM450_MSTORAGE_INTERFACE_NUM,
  .bAlternateSetting  = 0,
  .bNumEndpoints      = 2,
  .bInterfaceClass    = USB_CLASS_MASS_STORAGE,
  .bInterfaceSubClass = 0x06,
  .bInterfaceProtocol = 0x50,
  .iInterface         = 0x00,

};


static struct usb_endpoint_descriptor wpcm450_mstorage_ep_in_desc = {
  
  .bLength            = USB_DT_ENDPOINT_SIZE,
  .bDescriptorType    = USB_DT_ENDPOINT,
  .bEndpointAddress   = USB_DIR_IN + MSTORAGE_EP/* interrup endpoint */,
  .bmAttributes       = USB_ENDPOINT_XFER_BULK,
  .wMaxPacketSize     = __constant_cpu_to_le16(MSTORAGE_FS_MAX_PACKET_SIZE),
  .bInterval          = 0x0,
};

static struct usb_endpoint_descriptor wpcm450_mstorage_ep_out_desc = {
  
  .bLength            = USB_DT_ENDPOINT_SIZE,
  .bDescriptorType    = USB_DT_ENDPOINT,
  .bEndpointAddress   = USB_DIR_OUT + MSTORAGE_EP/* interrup endpoint */,
  .bmAttributes       = USB_ENDPOINT_XFER_BULK,
  .wMaxPacketSize     = __constant_cpu_to_le16(MSTORAGE_FS_MAX_PACKET_SIZE),
  .bInterval          = 0x0,
};


static __u8 config_desc_buffer[WPCM450_UDC_MSTORAGE_MAX_BUFFER_LENGTH+1];

#define DEVICE_QUALIFIER_DESCRIPTOR_SIZE 10


static struct usb_qualifier_descriptor wpcm450_usb20_qualifier_desc = {

  .bLength            = sizeof(struct usb_qualifier_descriptor),  
  .bDescriptorType    = 6,                                
  .bcdUSB             = __constant_cpu_to_le16(0x0200),
  .bDeviceClass       = 0,                                 
  .bDeviceSubClass    = 0,                                 
  .bDeviceProtocol    = 0,                               
  .bMaxPacketSize0    = WPCM450_UDC_MSTORAGE_CONTROL_MAX_PACKET_SIZE,
  .bNumConfigurations = 0x01,
  .bRESERVED          = 0,

};

#define OTHER_SPEED_CONFIG_DESC_SIZE  WPCM450_UDC_MSTORAGE_CONFIG_DESC_SIZE

__u8  other_speed_config[WPCM450_UDC_MSTORAGE_CONFIG_DESC_SIZE] =
{
   9,                         /* bLength Length of this descriptor */
   7,                         /* bDescType This is a Other speed config descr */
   OTHER_SPEED_CONFIG_DESC_SIZE & 0xff, 
   OTHER_SPEED_CONFIG_DESC_SIZE >> 8,
   1,
   1,
   4,
   /* Attributes.  Self-powered */
   0xc0,
   /* Current draw from bus */
   1,
   /* Interface 0 Descriptor - always 9 bytes */
   9,
   /* "Interface" type of descriptor */
   4,
   /* Number of this interface */
   WPCM450_MSTORAGE_INTERFACE_NUM,
   /* Alternate Setting */
   0,
   /* Number of endpoints on this interface */
   2,
   /* Interface Class */
   0x08,
   /* Interface Subclass: SCSI transparent command set */
   0x06,
   /* Interface Protocol: Bulk only protocol */
   0x50,
   /* Interface Description String Index */
   0,
   /* Endpoint 1 (Bulk In Endpoint), Interface 0 Descriptor - always 7 bytes*/
   7,
   /* "Endpoint" type of descriptor */
   5,
   /*
   ** Endpoint address.  The low nibble contains the endpoint number and the
   ** high bit indicates TX(1) or RX(0).
   */
   0x81,
   /* Attributes.  0=Control 1=Isochronous 2=Bulk 3=Interrupt */
   0x02,
   /* Max Packet Size for this endpoint */
   USB_uint_16_low(MSTORAGE_HS_MAX_PACKET_SIZE), 
   USB_uint_16_high(MSTORAGE_HS_MAX_PACKET_SIZE),
   /* Polling Interval (ms) */
   0,
   /* Endpoint 2 (Bulk Out Endpoint), Interface 0 Descriptor - always 7 bytes*/
   7,
   /* "Endpoint" type of descriptor */
   5,
   /*
   ** Endpoint address.  The low nibble contains the endpoint number and the
   ** high bit indicates TX(1) or RX(0).
   */
   0x01,
   /* Attributes.  0=Control 1=Isochronous 2=Bulk 3=Interrupt */
   0x02,
   /* Max Packet Size for this endpoint */
   USB_uint_16_low(MSTORAGE_HS_MAX_PACKET_SIZE), 
   USB_uint_16_high(MSTORAGE_HS_MAX_PACKET_SIZE),
   /* Polling Interval (ms) */
   0
};



/* number of strings in the table not including 0 or n. */
const __u8 WPCM450_UDC_MSTORAGE_USB_STR_NUM = 6;

const __u16 WPCM450_UDC_MSTORAGE_USB_STR_0[ 2] = {0x300 + sizeof(WPCM450_UDC_MSTORAGE_USB_STR_0),0x0409};
const __u16 WPCM450_UDC_MSTORAGE_USB_STR_1[25] = {0x300 + sizeof(WPCM450_UDC_MSTORAGE_USB_STR_1),
      'W','i','n','b','o','n','d',' ','E','l','e','c','t','r','o','n','i',\
      'c','s',' ','C','o','r','p'};
const __u16 WPCM450_UDC_MSTORAGE_USB_STR_2[31] = {0x300 + sizeof(WPCM450_UDC_MSTORAGE_USB_STR_2),
      'H','e','r','m','o','n',' ','U','S','B',' ','M','a','s','s',' ','S',\
      't','o','r','a','g','e',' ','D','e','v','i','c','e'};
const __u16 WPCM450_UDC_MSTORAGE_USB_STR_3[ 5] = {0x300 + sizeof(WPCM450_UDC_MSTORAGE_USB_STR_3),
      'D','e','m','o'};
const __u16 WPCM450_UDC_MSTORAGE_USB_STR_4[ 4] = {0x300 + sizeof(WPCM450_UDC_MSTORAGE_USB_STR_4),
      '#','0','2'};
const __u16 WPCM450_UDC_MSTORAGE_USB_STR_5[ 4] = {0x300 + sizeof(WPCM450_UDC_MSTORAGE_USB_STR_5),
      '_','A','1'};
const __u16 WPCM450_UDC_MSTORAGE_USB_STR_6[ 8] = {0x300 + sizeof(WPCM450_UDC_MSTORAGE_USB_STR_6),
      'W','i','n','b','o','n','d'};
const __u16 WPCM450_UDC_MSTORAGE_USB_STR_n[17] = {0x300 + sizeof(WPCM450_UDC_MSTORAGE_USB_STR_n),
      'B','A','D',' ','S','T','R','I','N','G',' ','I','n','d','e','x'};



#define WPCM450_UDC_MSTORAGE_USB_STRING_ARRAY_SIZE  8
const __u8* WPCM450_UDC_MSTORAGE_USB_STRING_DESC[WPCM450_UDC_MSTORAGE_USB_STRING_ARRAY_SIZE] =
{
   (__u8*)WPCM450_UDC_MSTORAGE_USB_STR_0,
   (__u8*)WPCM450_UDC_MSTORAGE_USB_STR_1,
   (__u8*)WPCM450_UDC_MSTORAGE_USB_STR_2,
   (__u8*)WPCM450_UDC_MSTORAGE_USB_STR_3,
   (__u8*)WPCM450_UDC_MSTORAGE_USB_STR_4,
   (__u8*)WPCM450_UDC_MSTORAGE_USB_STR_5,
   (__u8*)WPCM450_UDC_MSTORAGE_USB_STR_6,
   (__u8*)WPCM450_UDC_MSTORAGE_USB_STR_n
};


static __u8 empty_data[8] = {0};
static __u8 data[8];
static int wpcm450_mstorage_major =   USB_MSTORAGE_MAJOR;
static int wpcm450_mstorage_minor =   0;
static int wpcm450_nr_devs = 2;
static wpcm450_mstorage_dev_t *wpcm450_mstorage_devs;




/*****************************************************************
MASS STORAGE SPECIFIC GLOBALS
*****************************************************************/

volatile boolean        CBW_PROCESSED = FALSE;
volatile boolean        ZERO_TERMINATE = FALSE;
CSW_STRUCT     csw;

const MASS_STORAGE_DEVICE_INFO_STRUCT device_information_data = {
   0, 0x80, 0, 0x01, 0x1F, 0, 0, 0, 
   /* Vendor information: "WINBOND     " */
   {0x57, 0x49, 0x4e, 0x42, 0x4f, 0x4e, 0x44, 0x20,}, 
   /* Product information: "HERMON            " */
   {0x48, 0x45, 0x52, 0x4d, 0x4f, 0x4e, 0x20, 0x20,
   0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20}, 
   /* Product Revision level: "Demo" */
   {0x44, 0x65, 0x6D, 0x6F}
}; 

const MASS_STORAGE_READ_CAPACITY_STRUCT read_capacity = {
   /* Data for the capacity */
   {
      0x00, 0x00, USB_uint_16_high(TOTAL_LOGICAL_ADDRESS_BLOCKS-14), 
      USB_uint_16_low(TOTAL_LOGICAL_ADDRESS_BLOCKS-14)
   }, 
   {
      0x00, 0x00, USB_uint_16_high(LENGTH_OF_EACH_LAB), 
      USB_uint_16_low(LENGTH_OF_EACH_LAB)
   }
};

__u8* MASS_STORAGE_DISK_UNALIGNED;
__u8* MASS_STORAGE_DISK;

__u8 BOOT_SECTOR_AREA[512] = {
   /* Block 0 is the boot sector. Following is the data in the boot sector */
   /* 80x86 "short: jump instruction, indicating that the disk is formatted */
    0xEB, 
    /* 8-bit displacement */
    0x3C, 
    /* NOP OPCode */
    0x90, 
    /* 8-bytes for OEM identification: "ARC 4.3 " */
    0x41, 0x52, 0x43, 0x20, 0x34, 0x2E, 0x33, 0x20, 
    /* bytes/sector: 512 bytes (0x0200) */
    0x00, 0x02, 
    /* Sectors/allocation unit */
    0x01,
    /* Reserved sectors: 0x0001 */
    0x01, 0x00, 
    /* Number of File Allocation Tables (FATs): 2 */
    0x02,
    /* Number of root directory entries */
    0x70, 0x00, 
    /* Total sectors in logical volume */
    USB_uint_16_low(TOTAL_LOGICAL_ADDRESS_BLOCKS), 
    USB_uint_16_high(TOTAL_LOGICAL_ADDRESS_BLOCKS),
    /* Media descriptor byte: 0xF8: Fixed disk */
    0xFD,
    /* Sectors/FAT: 3 (Each FAT starts at a new sector) */
    0x03, 0x00, 
    /* Sectors/track: 9 */
    0x09, 0x00, 
    /* Number of heads */
    0x02, 0x00, 
    /* Number of hidden sectors: 0 */
    0x00, 0x00, 0x00, 0x00, 
    /* Total sectors in logical volume */
    0x00, 0x00, 0x00, 0x00, 
    /* Physical drive number */
    0x00, 
    /* Reserved */
    0x00, 
    /* Extended boot signature record: 0x29 */
    0x29,
    /* 32-bit binary volume ID */
    0x01, 0x02, 0x03, 0x04, 
    /* Volume label */
    0x56, 0x41, 0x75, 0x74, 0x6F, 0x6D, 0x61, 0x74, 0x69, 0x6F, 0x6E, 
    /* Reserved */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /* Bootstrap */
    0x33, 0xC0, 0x8E, 0xD0, 0xBC, 0x00, 0x7C, 0xFC, 0xE8, 0x45, 0x00, 
    /* String: \r\nNon-System disk\r\nPress any key to reboot\r\n" */
    0x0D, 0x0A, 0x4E, 0x6F, 0x6E, 0x2D, 0x53, 0x79, 0x73, 0x74, 0x65,
    0x6D, 0x20, 0x64, 0x69, 0x73, 0x6B, 0x0D, 0x0A, 0x50, 0x72, 0x65, 
    0x73, 0x73, 0x20, 0x61, 0x6E, 0x79, 0x20, 0x6B, 0x65, 0x79, 0x20, 
    0x74, 0x6F, 0x20, 0x72, 0x65, 0x62, 0x6F, 0x6F, 0x74, 0x0D, 0x0A, 
    0x5E, 0xEB, 0x02, 0xCD, 0x10, 0xB4, 0x0E, 0xBB, 0x07, 0x00, 0x2E, 
    0xAC, 0x84, 0xC0, 0x75, 0xF3, 0x98, 0xCD, 0x16, 0xCD, 0x19, 0xEB, 
    0xB1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    /* Partition descriptors */
    0x00, 0x01, 0x01, 0x00, 0x01, 0x04, 0x02, 0xC8, 0x11, 0x00, 0x00, 
    0x00, 0x02, 0xD0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xAA
};

const __u8 FAT16_SPECIAL_BYTES[3] = {
   /* FAT ID: Same as Media descriptor */
   0xFD, 0xFF, 0xFF
};



/**********************************************************************
Mouse data (this structure is used to send mouse movement information)
**********************************************************************/

static struct usb_ctrlrequest local_setup_packet;
static __u8 usb_status[2];
static __u8 endpoint;
static __u8 if_status;
static __u8 data_to_send;
static __u16 sof_count;
static __u16 setup_packet[sizeof(struct usb_ctrlrequest)];
__u8 mstorage_usb_if_alt[4] = { 0, 0, 0, 0};

static volatile __u8  speed = 0;

static int hardsect_size = 512;
static int nsectors = 1024; /* How big the drive is */
static int ndevices = 1;
static int request_mode = RM_SIMPLE;

/*****************************************************************************/
/* Functions                                                                 */
/*****************************************************************************/
static int wpcm450_udc_mstorage_bind(struct usb_gadget *gadget);
static void wpcm450_udc_mstorage_unbind(struct usb_gadget *gadget);
static int wpcm450_udc_mstorage_setup(
                   struct usb_gadget *gadget,
                   const struct usb_ctrlrequest *ctrl);
static void wpcm450_udc_mstorage_disconnect(struct usb_gadget *gadget);
static void wpcm450_udc_mstorage_service_ep0(
                    _usb_device_handle   handle,    /* [IN] Handle of the USB device                    */
                    boolean              setup,     /* [IN] Is it a setup packet?                       */
                    __u8                 direction, /* [IN] Direction of the transfer.  Is it transmit? */
                    __u8*                buffer,    /* [IN] Pointer to the data buffer                  */
                    __u32                length,    /* [IN] Length of the transfer                      */
                    __u8                 error      /* [IN] Error, if any                               */
                    );

static void wpcm450_udc_mstorage_reset_ep0(
                    _usb_device_handle   handle,    /* [IN] Handle of the USB device                    */
                    boolean              setup,     /* [IN] Is it a setup packet?                       */
                    __u8                 direction, /* [IN] Direction of the transfer.  Is it transmit? */
                    __u8*                buffer,    /* [IN] Pointer to the data buffer                  */
                    __u32                length,    /* [IN] Length of the transfer                      */
                    __u8                 error      /* [IN] Error, if any                               */
                    );

static void wpcm450_udc_mstorage_service_ep1 (
                    _usb_device_handle   handle,    /* [IN] Handle of the USB device                    */
                    boolean              setup,     /* [IN] Is it a setup packet?                       */
                    __u8                 direction, /* [IN] Direction of the transfer.  Is it transmit? */
                    __u8*                buffer,    /* [IN] Pointer to the data buffer                  */
                    __u32                length,    /* [IN] Length of the transfer                      */
                    __u8                 error      /* [IN] Error, if any                               */
                    );
   
void wpcm450_udc_mstorage_service_speed(
                    _usb_device_handle   handle,    /* [IN] Handle of the USB device                    */
                    boolean              setup,     /* [IN] Is it a setup packet?                       */
                    __u8                 direction, /* [IN] Direction of the transfer.  Is it transmit? */
                    __u8*                buffer,    /* [IN] Pointer to the data buffer                  */
                    __u32                length,    /* [IN] Length of the transfer                      */
                    __u8                 error      /* [IN] Error, if any                               */
                    );


/* Gadget Driver */


/*
 * wpcm450_udc_mstorage_bind
 *
 * Called on module load.  Allocates and initializes the device
 * structure and a control request.
 */
static int  wpcm450_udc_mstorage_bind(struct usb_gadget *gadget)
{
  int ret;
  struct usb_ep *ep;
  wpcm450_mstorage_dev_t *dev;
  int gcnum;
  _usb_device_handle handle = NULL;


  ret = _usb_device_init(WPCM450_USB20_PORT, &handle, 2);

  if(ret != 0)
  {
    printk(KERN_INFO "wpcm450_udc_mstorage_bind: %s %s bound\n",
    WPCM450_UDC_MSTORAGE_LONG_NAME, WPCM450_UDC_MSTORAGE_VERSION_STR);
  }

  wpcm450_mstorage_devs->handler = handle;
  
  ret = _usb_device_register_service(handle, USB_SERVICE_EP0, wpcm450_udc_mstorage_service_ep0);
   
  if (ret != USB_OK) {
      printk(KERN_ERR "\nUSB Service Registration failed. Error: %x", ret);
      goto autoconf_fail;
  } /* Endif */

  printk(KERN_INFO "\nUSB Service Registration EP0 OK\n");
   
  ret = _usb_device_register_service(handle, USB_SERVICE_BUS_RESET,  wpcm450_udc_mstorage_reset_ep0);
   
  if (ret != USB_OK) {
      printk(KERN_ERR "\nUSB Service Registration failed. Error: %x", ret);
      
      goto autoconf_fail;
  } /* Endif */

  printk(KERN_INFO "\nUSB Service Registration BUS RESET OK\n");
   
  ret = _usb_device_register_service(handle, USB_SERVICE_EP1, wpcm450_udc_mstorage_service_ep1);
   
  if (ret != USB_OK) {
      printk(KERN_ERR "\nUSB Service Registration failed. Error: %x", ret);
      goto autoconf_fail;
  } /* Endif */

    printk(KERN_INFO "\nUSB Service Registration EP1 OK\n");


   ret = _usb_device_register_service(handle, USB_SERVICE_SPEED_DETECTION, wpcm450_udc_mstorage_service_speed);

   if (ret != USB_OK) {
      printk(KERN_ERR "\nUSB Service Registration failed. Error: %x", ret);
      goto autoconf_fail;
  } /* Endif */


 

  printk(KERN_INFO "\nUSB Service Registration SPEED_DETECTION OK\n");
#if 0 
  ret = _usb_device_register_service(handle, USB_SERVICE_SUSPEND, wpcm450_service_suspend);

  if (ret != USB_OK) {
      printk(KERN_ERR "\nUSB Service Registration failed. Error: %x", ret);
      
  } /* Endif */
#endif

#ifdef _USB_CACHE_USE_
  flush_cache_all();
#endif

  /* Initialize the endpoint 0 in both directions */
  //_usb_device_init_endpoint(handle, 0, 64, 0,  USB_CONTROL_ENDPOINT, 0);
  //_usb_device_init_endpoint(handle, 0, 64, 1,  USB_CONTROL_ENDPOINT, 0);


   _usb_device_run(handle);

   
  printk(KERN_INFO "%s %s bound\n",
    WPCM450_UDC_MSTORAGE_LONG_NAME, WPCM450_UDC_MSTORAGE_VERSION_STR);

  return 0;

autoconf_fail:
  printk(KERN_ERR "wpcm450_udc_mstorage_bind: cannot autoconfigure on %s\n", gadget->name);
  return -ENODEV;
}


/*
 * wpcm450_udc_mstorage_unbind
 *
 * Called on module unload.  Frees the control request and device
 * structure.
 */
static void  wpcm450_udc_mstorage_unbind(struct usb_gadget *gadget)
{

  wpcm450_mstorage_dev_t *dev = get_gadget_data(gadget);

  printk(KERN_INFO "wpcm450_udc_mstorage_unbind: %s %s unbound\n", WPCM450_UDC_MSTORAGE_LONG_NAME,
         WPCM450_UDC_MSTORAGE_VERSION_STR);
}



static void wpcm450_udc_mstorage_disconnect(struct usb_gadget *gadget)
{
}


/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : service_speed
* Returned Value : None
* Comments       :
*     Called upon a speed detection event.
* 
*END*--------------------------------------------------------------------*/
void wpcm450_udc_mstorage_service_speed
   (
      /* [IN] Handle of the USB device */
      _usb_device_handle   handle,
   
      /* [IN] Unused */
      boolean              setup,
   
      /* [IN] Unused */
      __u8               direction,
   
      /* [IN] Unused */
      __u8*           buffer,
   
      /* [IN] Unused */
      __u32              length,

      /* [IN] Error, if any */
      __u8               error
            
   )
{ /* EndBody */
   speed = length;
   return;
} /* EndBody */




/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_udc_mstorage_ch9_class
* Returned Value : 
* Comments       :
*     Chapter 9 Class specific request
*     See section 9.4.11 (page 195) of the USB 1.1 Specification.
* 
*END*--------------------------------------------------------------------*/
void wpcm450_udc_mstorage_ch9_class(
         _usb_device_handle handle,        /* USB handle */
         boolean     setup,                /* Is it a Setup phase? */
         __u8      direction,              /* [IN] Direction of the transfer. (1 for USB IN token)*/
         struct usb_ctrlrequest *setup_ptr /* The setup packet pointer */
         )
{ /* Body */

   __u8* epTemp_buf;
   
   if (setup) {
      switch (setup_ptr->bRequest) {
         case 0xFF:
            /* Bulk-Only Mass Storage Reset: Ready the device for the next 
            ** CBW from the host 
            */
            if ((setup_ptr->wValue != 0) || 
                (setup_ptr->wIndex != WPCM450_MSTORAGE_INTERFACE_NUM) ||
                (setup_ptr->wLength != 0)) 
            {
                _usb_device_stall_endpoint(handle, 0, 0);
            }
            else 
            { /* Body */
               CBW_PROCESSED = FALSE;
               ZERO_TERMINATE = FALSE;
               _usb_device_cancel_transfer(handle, MSTORAGE_EP, USB_RECV);
               _usb_device_cancel_transfer(handle, MSTORAGE_EP, USB_SEND);
               
               /* unstall bulk endpoint */
               _usb_device_unstall_endpoint(handle, MSTORAGE_EP, USB_RECV);
               _usb_device_unstall_endpoint(handle, MSTORAGE_EP, USB_SEND);
               /* 
               _usb_device_set_status(handle, USB_STATUS_ENDPOINT | 1, 0);
               */
               _usb_device_recv_data(handle, 1, wpcm450_mstorage_devs->data, 31);
               /* send zero packet to control pipe */
               _usb_device_send_data(handle, 0, 0, 0);
            } /* Endbody */
            return;
         case 0xFE:
            /* For Get Max LUN use any of these responses*/
            if (setup_ptr->wLength == 0) { /* Body */
               _usb_device_stall_endpoint(handle, 0, 0);
            } else if ((setup_ptr->wValue != 0) ||
                (setup_ptr->wIndex != WPCM450_MSTORAGE_INTERFACE_NUM) ||
                (setup_ptr->wLength != 1)) { /* Body */
                _usb_device_stall_endpoint(handle, 0, 0);
             } else { /* Body */
               /* Send Max LUN = 0 to the the control pipe */
               *epTemp_buf = 0;
               _usb_device_send_data(handle, 0, epTemp_buf, 1);
                 /* status phase */
               _usb_device_recv_data(handle, 0, 0, 0);
               
             } /* Endbody */     
            return;
         default :
            _usb_device_stall_endpoint(handle, 0, 0);
            return;
      } /* EndSwitch */
   } 

} /* Endbody */

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_udc_mstorage_ch9_get_status
* Returned Value : None
* Comments       :
*     Chapter 9 GetStatus command
*     wValue=Zero
*     wIndex=Zero
*     wLength=1
*     DATA=bmERR_STAT
*     The GET_STATUS command is used to read the bmERR_STAT register.
*     
*     Return the status based on the bRrequestType bits:
*     device (0) = bit 0 = 1 = self powered
*                  bit 1 = 0 = DEVICE_REMOTE_WAKEUP which can be modified
*     with a SET_FEATURE/CLEAR_FEATURE command.
*     interface(1) = 0000.
*     endpoint(2) = bit 0 = stall.
*     static __u8* pData;
*
*     See section 9.4.5 (page 190) of the USB 1.1 Specification.
* 
*END*--------------------------------------------------------------------*/
void wpcm450_udc_mstorage_ch9_get_status  (
        _usb_device_handle handle,        /* USB handle */
        boolean setup,                    /* Is it a Setup phase? */
        struct usb_ctrlrequest *setup_ptr /* The setup packet pointer */
   )
{ /* Body */
   __u16 temp_status;
   if (setup) {
      switch (setup_ptr->bRequestType) {

         case 0x80:
            /* Device request */
            _usb_device_get_status(handle, USB_STATUS_DEVICE, &temp_status);
            /*copy the status in usb_status to ensure that it is
               flushed from cache */
            usb_status[0] = temp_status & 0xFF;
            usb_status[1] = (temp_status >> 2)& 0xFF;
            
            /* Send the requested data */
            _usb_device_send_data(handle, 0, (__u8*)&usb_status, 2);
            break;

         case 0x81:
            /* Interface request */
            if_status = mstorage_usb_if_alt[setup_ptr->wIndex & 0x00FF];
            
            /* Send the requested data */
            _usb_device_send_data(handle, 0, (void*)&if_status, sizeof(if_status));
            break;
      
         case 0x82:
            /* Endpoint request */
            endpoint = setup_ptr->wIndex & USB_STATUS_ENDPOINT_NUMBER_MASK;
            _usb_device_get_status(handle,
               USB_STATUS_ENDPOINT | endpoint, &temp_status);
               
               /*copy the status in usb_status to ensure that it is
               flushed from cache */
               usb_status[0] = temp_status & 0xFF;
               usb_status[1] = (temp_status >> 2)& 0xFF;
            /* Send the requested data */
            _usb_device_send_data(handle, 0, (__u8*)&usb_status, 
               2);      
            break;
         
         default:
            /* Unknown request */
            _usb_device_stall_endpoint(handle, 0, 0);
            return;

      } /* Endswitch */
      
      /* status phase */
      _usb_device_recv_data(handle, 0, 0, 0);
   } /* Endif */
   return;
} /* Endbody */

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_udc_mstorage_ch9_clear_feature
* Returned Value : None
* Comments       :
*     Chapter 9 ClearFeature command
*     See section 9.4.1 (page 188) of the USB 1.1 Specification.
* 
*END*--------------------------------------------------------------------*/
void wpcm450_udc_mstorage_ch9_clear_feature(
        _usb_device_handle handle,        /* USB handle */
        boolean setup,                    /* Is it a Setup phase? */
        struct usb_ctrlrequest *setup_ptr /* The setup packet pointer */
        )
{ /* Body */
   static __u8           endpoint;
   __u16                 usb_status;

   _usb_device_get_status(handle, USB_STATUS_DEVICE_STATE, &usb_status);

   if ((usb_status != USB_STATE_CONFIG) && (usb_status != USB_STATE_ADDRESS)) {
      _usb_device_stall_endpoint(handle, 0, 0);
      return;
   } /* Endif */

   if (setup) {
      switch (setup_ptr->bRequestType) {
      
         case 0:
            /* DEVICE */
            if (setup_ptr->wValue == 1) {
               /* clear remote wakeup */
               _usb_device_get_status(handle, USB_STATUS_DEVICE, &usb_status);
               usb_status &= ~USB_REMOTE_WAKEUP;
               _usb_device_set_status(handle, USB_STATUS_DEVICE, usb_status);
            } else {
               _usb_device_stall_endpoint(handle, 0, 0);
               return;
            } /* Endif */
            break;
         
         case 2:
            /* ENDPOINT */
            if (setup_ptr->wValue != 0) {
               _usb_device_stall_endpoint(handle, 0, 0);
               return;
            } /* Endif */
            endpoint = setup_ptr->wIndex & USB_STATUS_ENDPOINT_NUMBER_MASK;
            _usb_device_get_status(handle, USB_STATUS_ENDPOINT | endpoint,
               &usb_status);
            /* set stall */
            _usb_device_set_status(handle, USB_STATUS_ENDPOINT | endpoint,
               0);
            break;
            
         default:
            _usb_device_stall_endpoint(handle, 0, 0);
            return;
      } /* Endswitch */
      
      /* status phase */
      _usb_device_send_data(handle, 0, 0, 0);
   } /* Endif */
   return;
} /* Endbody */

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_udc_mstorage_ch9_set_feature
* Returned Value : None
* Comments       :
*     Chapter 9 SetFeature command
*     See section 9.4.9 (page 194) of the USB 1.1 Specification.
* 
*END*--------------------------------------------------------------------*/
void wpcm450_udc_mstorage_ch9_set_feature(
      _usb_device_handle handle,   /* USB handle */
      boolean setup,               /* Is it a Setup phase? */
      struct usb_ctrlrequest *setup_ptr /* The setup packet pointer */
      )
{ /* Body */
   __u16           usb_status;
   __u8            endpoint;

   if (setup) {
      switch (setup_ptr->bRequestType) {

         case 0:
            /* DEVICE */
            if (setup_ptr->wValue == 1) {
               /* set remote wakeup */
               _usb_device_get_status(handle, USB_STATUS_DEVICE, &usb_status);
               usb_status |= USB_REMOTE_WAKEUP;
               _usb_device_set_status(handle, USB_STATUS_DEVICE, usb_status);
            } else {
               _usb_device_stall_endpoint(handle, 0, 0);
               return;
            } /* Endif */
            break;
            
         case 2:
            /* ENDPOINT */
            if (setup_ptr->wValue != 0) {
               _usb_device_stall_endpoint(handle, 0, 0);
               return;
            } /* Endif */
            endpoint = setup_ptr->wIndex & USB_STATUS_ENDPOINT_NUMBER_MASK;
            _usb_device_get_status(handle, USB_STATUS_ENDPOINT | endpoint,
               &usb_status);
            /* set stall */
            _usb_device_set_status(handle, USB_STATUS_ENDPOINT | endpoint,
               1);
            break;

         default:
            _usb_device_stall_endpoint(handle, 0, 0);
            return;

      } /* Endswitch */
      
      /* status phase */
      _usb_device_send_data(handle, 0, 0, 0);
   } /* Endif */
   return;
} /* Endbody */

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_udc_mstorage_ch9_set_address
* Returned Value : None
* Comments       :
*     Chapter 9 SetAddress command
*     We setup a TX packet of 0 length ready for the IN token
*     Once we get the TOK_DNE interrupt for the IN token, then
*     we change the ADDR register and go to the ADDRESS state.
*     See section 9.4.6 (page 192) of the USB 1.1 Specification.
* 
*END*--------------------------------------------------------------------*/
void wpcm450_udc_mstorage_ch9_set_address(
        _usb_device_handle handle,        /* USB handle */
        boolean setup,                    /* Is it a Setup phase? */
        struct usb_ctrlrequest *setup_ptr /* The setup packet pointer */
        )
{ /* Body */
   static __u8 new_address;
   __u32 max_pkt_size;
  
   if (setup) {
      new_address = setup_ptr->wValue;
      /*******************************************************
      if hardware assitance is enabled for set_address (see
      hardware rev for details) we need to do the set_address
      before queuing the status phase.
      *******************************************************/
      #ifdef SET_ADDRESS_HARDWARE_ASSISTANCE      
         _usb_device_set_status(handle, USB_STATUS_ADDRESS, new_address);
      #endif
      /* ack */
      _usb_device_send_data(handle, 0, 0, 0);
   } else {
      #ifndef SET_ADDRESS_HARDWARE_ASSISTANCE
         _usb_device_set_status(handle, USB_STATUS_ADDRESS, new_address);
      #endif

      _usb_device_set_status(handle, USB_STATUS_DEVICE_STATE,
         USB_STATE_ADDRESS);

      /*set address is a good place to init all non control endpoints */
      if (speed == USB_SPEED_HIGH)
      {
         max_pkt_size = MSTORAGE_HS_MAX_PACKET_SIZE;
      }
      else
      {
         max_pkt_size = MSTORAGE_FS_MAX_PACKET_SIZE;
      } /* Endif */
      
      _usb_device_init_endpoint(handle, 1, max_pkt_size,
           USB_RECV, USB_BULK_ENDPOINT, USB_DEVICE_DONT_ZERO_TERMINATE);
      _usb_device_init_endpoint(handle, 1, max_pkt_size,
           USB_SEND, USB_BULK_ENDPOINT, USB_DEVICE_DONT_ZERO_TERMINATE);

      if (_usb_device_get_transfer_status(handle, 1, USB_RECV) == USB_OK)
      {
         _usb_device_recv_data(handle, 1, wpcm450_mstorage_devs->data, 31);
      } /* Endif */      
      
   } /* Endif */
   return;
} /* Endbody */

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_udc_mstorage_ch9_get_description
* Returned Value : None
* Comments       :
*     Chapter 9 GetDescription command
*     The Device Request can ask for Device/Config/string/interface/endpoint
*     descriptors (via wValue). We then post an IN response to return the
*     requested descriptor.
*     And then wait for the OUT which terminates the control transfer.
*     See section 9.4.3 (page 189) of the USB 1.1 Specification.
* 
*END*--------------------------------------------------------------------*/
void wpcm450_udc_mstorage_ch9_get_description(
       _usb_device_handle handle,        /* USB handle */
        boolean setup,                    /* Is it a Setup phase? */
        struct usb_ctrlrequest *setup_ptr /* The setup packet pointer */
        )
{ /* Body */
   
   __u32 buf_offset;
   __u32 max_pkt_size;
   
   if (setup) {
      /* Load the appropriate string depending on the descriptor requested.*/
      switch (setup_ptr->wValue & 0xFF00) {

         case 0x0100:

            _usb_device_send_data(
                 handle,
                 0, 
                 (__u8*)&wpcm450_usb20_device_desc,
                 USB_MSTORAGE_MIN(setup_ptr->wLength, sizeof(wpcm450_usb20_device_desc)));
      
            break;

         case 0x0200:
        
            buf_offset = 0;



            if(speed == USB_SPEED_HIGH) {
               wpcm450_mstorage_ep_in_desc.wMaxPacketSize = MSTORAGE_HS_MAX_PACKET_SIZE;
               wpcm450_mstorage_ep_out_desc.wMaxPacketSize = MSTORAGE_HS_MAX_PACKET_SIZE;
            } else {
               wpcm450_mstorage_ep_in_desc.wMaxPacketSize = MSTORAGE_FS_MAX_PACKET_SIZE;
               wpcm450_mstorage_ep_out_desc.wMaxPacketSize = MSTORAGE_FS_MAX_PACKET_SIZE;
            } /* Endif */

            

            wpcm450_mstorage_config_desc.wTotalLength =  sizeof(struct usb_config_descriptor) 
             +  sizeof(struct usb_interface_descriptor) + wpcm450_mstorage_ep_in_desc.bLength 
             + wpcm450_mstorage_ep_out_desc.bLength;


            
            
            memcpy(config_desc_buffer, &wpcm450_mstorage_config_desc, sizeof(struct usb_config_descriptor));
            buf_offset += sizeof(struct usb_config_descriptor);

            memcpy((config_desc_buffer+buf_offset), &wpcm450_mstorage_intf_desc, sizeof(struct usb_interface_descriptor));
            buf_offset += sizeof(struct usb_interface_descriptor);

            memcpy((config_desc_buffer+buf_offset), &wpcm450_mstorage_ep_in_desc, wpcm450_mstorage_ep_in_desc.bLength);
            buf_offset += wpcm450_mstorage_ep_in_desc.bLength;

            memcpy((config_desc_buffer+buf_offset), &wpcm450_mstorage_ep_out_desc, wpcm450_mstorage_ep_out_desc.bLength);
            buf_offset += wpcm450_mstorage_ep_out_desc.bLength;
              
            _usb_device_send_data(handle, 0, (__u8*)config_desc_buffer,
               USB_MSTORAGE_MIN(setup_ptr->wLength, buf_offset));
            
                        
            break;

           
         case 0x0300:
          
            if ((setup_ptr->wValue & 0x00FF) > WPCM450_UDC_MSTORAGE_USB_STR_NUM) {
               _usb_device_send_data(handle, 0, WPCM450_UDC_MSTORAGE_USB_STRING_DESC[WPCM450_UDC_MSTORAGE_USB_STR_NUM+1],
                  USB_MSTORAGE_MIN(setup_ptr->wLength, WPCM450_UDC_MSTORAGE_USB_STRING_DESC[WPCM450_UDC_MSTORAGE_USB_STR_NUM+1][0]));
            } else {
               _usb_device_send_data(handle, 0,
                  WPCM450_UDC_MSTORAGE_USB_STRING_DESC[setup_ptr->wValue & 0x00FF],
                  USB_MSTORAGE_MIN(setup_ptr->wLength,
                     WPCM450_UDC_MSTORAGE_USB_STRING_DESC[setup_ptr->wValue & 0x00FF][0]));
            } /* Endif */      
            break;

         case 0x600:
            _usb_device_send_data(handle, 0, (__u8*)&wpcm450_usb20_qualifier_desc, 
               USB_MSTORAGE_MIN(setup_ptr->wLength, DEVICE_QUALIFIER_DESCRIPTOR_SIZE));
            break;
           
         case 0x700:      
            _usb_device_send_data(handle, 0, (__u8*)other_speed_config, 
               USB_MSTORAGE_MIN(setup_ptr->wLength, OTHER_SPEED_CONFIG_DESC_SIZE));
            break;


         default:
            _usb_device_stall_endpoint(handle, 0, 0);
            return;
      } /* Endswitch */
      /* status phase */
      _usb_device_recv_data(handle, 0, 0, 0);
   } /* Endif */
   return;
} /* Endbody */

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_udc_mstorage_ch9_set_description
* Returned Value : None
* Comments       :
*     Chapter 9 SetDescription command
*     See section 9.4.8 (page 193) of the USB 1.1 Specification.
* 
*END*--------------------------------------------------------------------*/
void wpcm450_udc_mstorage_ch9_set_description(
       _usb_device_handle handle,        /* USB handle */
        boolean setup,                    /* Is it a Setup phase? */
        struct usb_ctrlrequest *setup_ptr /* The setup packet pointer */
        )
{ /* Body */
   _usb_device_stall_endpoint(handle, 0, 0);
   return;
} /* Endbody */

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_udc_mstorage_ch9_get_config
* Returned Value : None
* Comments       :
*     Chapter 9 GetConfig command
*     See section 9.4.2 (page 189) of the USB 1.1 Specification.
* 
*END*--------------------------------------------------------------------*/
void wpcm450_udc_mstorage_ch9_get_config(
        _usb_device_handle handle,        /* USB handle */
        boolean setup,                    /* Is it a Setup phase? */
        struct usb_ctrlrequest *setup_ptr /* The setup packet pointer */
        )
{ /* Body */
   __u16 current_config;
   /* Return the currently selected configuration */
   if (setup){ 
      _usb_device_get_status(handle, USB_STATUS_CURRENT_CONFIG,
         &current_config);
      data_to_send = current_config;      
      _usb_device_send_data(handle, 0, (__u8*) &data_to_send, sizeof(data_to_send));
      /* status phase */
      _usb_device_recv_data(handle, 0, 0, 0);
   } /* Endif */
   return;
} /* Endbody */

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_udc_mstorage_ch9_set_config
* Returned Value : None
* Comments       :
*     Chapter 9 SetConfig command
* 
*END*--------------------------------------------------------------------*/
void wpcm450_udc_mstorage_ch9_set_config(
        _usb_device_handle handle,        /* USB handle */
        boolean setup,                    /* Is it a Setup phase? */
        struct usb_ctrlrequest *setup_ptr /* The setup packet pointer */
        )
{ /* Body */
   __u16 usb_state;
   
   if (setup) {
      if ((setup_ptr->wValue & 0x00FF) > 1) {
         /* generate stall */
         _usb_device_stall_endpoint(handle, 0, 0);
         return;
      } /* Endif */
      /* 0 indicates return to unconfigured state */
      if ((setup_ptr->wValue & 0x00FF) == 0) {
         _usb_device_get_status(handle, USB_STATUS_DEVICE_STATE, &usb_state);
         if ((usb_state == USB_STATE_CONFIG) || 
            (usb_state == USB_STATE_ADDRESS)) 
         {
            /* clear the currently selected config value */
            _usb_device_set_status(handle, USB_STATUS_CURRENT_CONFIG, 0);
            _usb_device_set_status(handle, USB_STATUS_DEVICE_STATE,
               USB_STATE_ADDRESS);
            /* status phase */      
            _usb_device_send_data(handle, 0, 0, 0);
         } else {
            _usb_device_stall_endpoint(handle, 0, 0);
         } /* Endif */
         return;
      } /* Endif */

      /*
      ** If the configuration value (setup_ptr->wValue & 0x00FF) differs
      ** from the current configuration value, then endpoints must be
      ** reconfigured to match the new device configuration
      */
      _usb_device_get_status(handle, USB_STATUS_CURRENT_CONFIG,
         &usb_state);
      if (usb_state != (setup_ptr->wValue & 0x00FF)) {
         /* Reconfigure endpoints here */
         switch (setup_ptr->wValue & 0x00FF) {
      
            default:
               break;
         } /* Endswitch */
         _usb_device_set_status(handle, USB_STATUS_CURRENT_CONFIG,
            setup_ptr->wValue & 0x00FF);
         _usb_device_set_status(handle, USB_STATUS_DEVICE_STATE,
            USB_STATE_CONFIG);      
         /* status phase */      
         _usb_device_send_data(handle, 0, 0, 0);
         return;
      } /* Endif */
      _usb_device_set_status(handle, USB_STATUS_DEVICE_STATE,
         USB_STATE_CONFIG);
      /* status phase */
      _usb_device_send_data(handle, 0, 0, 0);
   } /* Endif */
   return;
} /* Endbody */

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_udc_mstorage_ch9_get_interface
* Returned Value : None
* Comments       :
*     Chapter 9 GetInterface command
*     See section 9.4.4 (page 190) of the USB 1.1 Specification.
* 
*END*--------------------------------------------------------------------*/
void wpcm450_udc_mstorage_ch9_get_interface(
        _usb_device_handle handle,        /* USB handle */
        boolean setup,                    /* Is it a Setup phase? */
        struct usb_ctrlrequest *setup_ptr /* The setup packet pointer */
        )
{ /* Body */
   __u16 usb_state;
   
   _usb_device_get_status(handle, USB_STATUS_DEVICE_STATE, &usb_state);
   if (usb_state != USB_STATE_CONFIG) {
      _usb_device_stall_endpoint(handle, 0, 0);
      return;
   } /* Endif */

   if (setup) {
      _usb_device_send_data(handle, 0, &mstorage_usb_if_alt[setup_ptr->wIndex & 0x00FF],
         USB_MSTORAGE_MIN(setup_ptr->wLength, sizeof(__u8)));
      /* status phase */      
      _usb_device_recv_data(handle, 0, 0, 0);
   } /* Endif */
   return;
} /* Endbody */

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_udc_mstorage_ch9_set_interface
* Returned Value : None
* Comments       :
*     Chapter 9 SetInterface command
*     See section 9.4.10 (page 195) of the USB 1.1 Specification.
* 
*END*--------------------------------------------------------------------*/
void wpcm450_udc_mstorage_ch9_set_interface(
        _usb_device_handle handle,        /* USB handle */
        boolean setup,                    /* Is it a Setup phase? */
        struct usb_ctrlrequest *setup_ptr /* The setup packet pointer */
        )
{ /* Body */
   if (setup) {
      if (setup_ptr->bRequestType != 0x01) {
         _usb_device_stall_endpoint(handle, 0, 0);
         return;
      } /* Endif */

      /*
      ** If the alternate value (setup_ptr->wValue & 0x00FF) differs
      ** from the current alternate value for the specified interface,
      ** then endpoints must be reconfigured to match the new alternate
      */
      if (mstorage_usb_if_alt[setup_ptr->wIndex & 0x00FF]
         != (setup_ptr->wValue & 0x00FF))
      {
         mstorage_usb_if_alt[setup_ptr->wIndex & 0x00FF] = (setup_ptr->wValue & 0x00FF);
         /* Reconfigure endpoints here. */
         
      } /* Endif */

      /* status phase */
      _usb_device_send_data(handle, 0, 0, 0);
   } /* Endif */
   return;
} /* Endbody */

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_udc_mstorage_ch9_sync_frame
* Returned Value : 
* Comments       :
*     Chapter 9 SynchFrame command
*     See section 9.4.11 (page 195) of the USB 1.1 Specification.
* 
*END*--------------------------------------------------------------------*/
void wpcm450_udc_mstorage_ch9_sync_frame(
        _usb_device_handle handle,        /* USB handle */
        boolean setup,                    /* Is it a Setup phase? */
        struct usb_ctrlrequest *setup_ptr /* The setup packet pointer */
        )
{ /* Body */
   
   if (setup) {
      if (setup_ptr->bRequestType != 0x02) {
         _usb_device_stall_endpoint(handle, 0, 0);
         return;
      } /* Endif */

      if ((setup_ptr->wIndex & 0x00FF) >= wpcm450_mstorage_intf_desc.bInterfaceNumber)
      {
         _usb_device_stall_endpoint(handle, 0, 0);
         return;
      } /* Endif */

      _usb_device_get_status(handle, USB_STATUS_SOF_COUNT, &sof_count);
      _usb_device_send_data(handle, 0, (__u8*)&sof_count,
         USB_MSTORAGE_MIN(setup_ptr->wLength, sizeof(sof_count)));
      /* status phase */      
      _usb_device_recv_data(handle, 0, 0, 0);
   } /* Endif */
   return;
} /* Endbody */


/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_udc_mstorage_service_ep0
* Returned Value : None
* Comments       :
*     Called upon a completed endpoint 0 (USB 1.1 Chapter 9) transfer
* 
*END*--------------------------------------------------------------------*/
void wpcm450_udc_mstorage_service_ep0
   (
      _usb_device_handle   handle,    /* [IN] Handle of the USB device                    */
      boolean              setup,     /* [IN] Is it a setup packet?                       */
      __u8                 direction, /* [IN] Direction of the transfer.  Is it transmit? */
      __u8*                buffer,    /* [IN] Pointer to the data buffer                  */
      __u32                length,    /* [IN] Length of the transfer                      */
      __u8                 error      /* [IN] Error, if any                               */
    )
{ /* Body */
   boolean  class_request = FALSE;
   
   if (setup) {
      _usb_device_read_setup_data(handle, 0, (__u8*)&local_setup_packet);
   } else if (class_request) {
      class_request = FALSE;
      /* Finish your class or vendor request here */
      
      return;
   } /* Endif */
   
   switch (local_setup_packet.bRequestType & 0x60) {

      case 0x00:
         switch (local_setup_packet.bRequest) {

            case 0x0:
               wpcm450_udc_mstorage_ch9_get_status(handle, setup, &local_setup_packet);
               break;

            case 0x1:
               wpcm450_udc_mstorage_ch9_clear_feature(handle, setup, &local_setup_packet);
               break;

            case 0x3:
               wpcm450_udc_mstorage_ch9_set_feature(handle, setup, &local_setup_packet);
               break;

            case 0x5:
               wpcm450_udc_mstorage_ch9_set_address(handle, setup, &local_setup_packet);
               break;

            case 0x6:
               wpcm450_udc_mstorage_ch9_get_description(handle, setup, &local_setup_packet);
               break;

            case 0x7:
               wpcm450_udc_mstorage_ch9_set_description(handle, setup, &local_setup_packet);
               break;

            case 0x8:
               wpcm450_udc_mstorage_ch9_get_config(handle, setup, &local_setup_packet);
               break;

            case 0x9:
               wpcm450_udc_mstorage_ch9_set_config(handle, setup, &local_setup_packet);
               break;

            case 0xa:
               wpcm450_udc_mstorage_ch9_get_interface(handle, setup, &local_setup_packet);
               break;

            case 0xb:
               wpcm450_udc_mstorage_ch9_set_interface(handle, setup, &local_setup_packet);
               break;

            case 0xc:
               wpcm450_udc_mstorage_ch9_sync_frame(handle, setup, &local_setup_packet);
               break;

            default:
               _usb_device_stall_endpoint(handle, 0, 0);
               break;

         } /* Endswitch */
         
         break;

      case 0x20:
         wpcm450_udc_mstorage_ch9_class(handle, setup, direction, &local_setup_packet);

         break;

      case 0x40:
         /* vendor specific request can be handled here*/
         _usb_device_stall_endpoint(handle, 0, 0);
         break;
      
      default:
         _usb_device_stall_endpoint(handle, 0, 0);
         break;
         
   } /* Endswitch */
   
   return;
} /* Endbody */


/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_udc_mstorage_process_inquiry_command
* Returned Value : None
* Comments       :
*     Process a Mass storage class Inquiry command
* 
*END*--------------------------------------------------------------------*/
void wpcm450_udc_mstorage_process_inquiry_command
   (
      /* [IN] Handle of the USB device */
      _usb_device_handle   handle,
            
      /* [IN] Endpoint number */
      __u8               ep_num,
      
      /* [IN] Pointer to the data buffer */
      CBW_STRUCT_PTR       cbw_ptr
   )
{ /* Body */
   MASS_STORAGE_INQUIRY_PTR   inquiry_cmd_ptr = 
      (MASS_STORAGE_INQUIRY_PTR)((void*)cbw_ptr->CBWCB);

   if (cbw_ptr->DCBWDATALENGTH) {
      if (cbw_ptr->BMCBWFLAGS & USB_CBW_DIRECTION_BIT) {      
         /* Send the device information */
         _usb_device_send_data(handle, 1, (__u8*)&device_information_data, 
            36);
      } /* Endif */
   } /* Endif */
   
   /* The actual length will never exceed the DCBWDATALENGTH */            
   csw.DCSWDATARESIDUE = (cbw_ptr->DCBWDATALENGTH - 36);
   csw.BCSWSTATUS = 0;
   
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_udc_mstorage_process_unsupported_command
* Returned Value : None
* Comments       :
*     Responds appropriately to unsupported commands
* 
*END*--------------------------------------------------------------------*/
void wpcm450_udc_mstorage_process_unsupported_command
   (
      /* [IN] Handle of the USB device */
      _usb_device_handle   handle,
            
      /* [IN] Endpoint number */
      __u8               ep_num,
      
      /* [IN] Pointer to the data buffer */
      CBW_STRUCT_PTR       cbw_ptr
   )
{ /* Body */

      
   /* The actual length will never exceed the DCBWDATALENGTH */
   csw.DCSWDATARESIDUE = 0;
   csw.BCSWSTATUS = 0;

   if (cbw_ptr->BMCBWFLAGS & USB_CBW_DIRECTION_BIT) {      
      /* Send a zero-length packet */
      _usb_device_send_data(handle, ep_num, 
         (__u8*)&device_information_data, 0);
   } else {
      CBW_PROCESSED = FALSE;
      /* Send the command status information */
      _usb_device_send_data(handle, 1, (__u8*)&csw, 13);
      _usb_device_recv_data(handle, 1, wpcm450_mstorage_devs->data, 31);
   } /* Endif */
   
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_udc_mstorage_process_report_capacity
* Returned Value : None
* Comments       :
*     Reports the media capacity as a response to READ CAPACITY Command.
* 
*END*--------------------------------------------------------------------*/
void wpcm450_udc_mstorage_process_report_capacity
   (
      /* [IN] Handle of the USB device */
      _usb_device_handle   handle,
            
      /* [IN] Endpoint number */
      __u8               ep_num,
      
      /* [IN] Pointer to the data buffer */
      CBW_STRUCT_PTR       cbw_ptr
   )
{ /* Body */

   if (cbw_ptr->BMCBWFLAGS & USB_CBW_DIRECTION_BIT) {      
      /* Send a zero-length packet */
         _usb_device_send_data(handle, ep_num, (__u8*)&read_capacity, 8);
      
   } /* Endif */
   
   /* The actual length will never exceed the DCBWDATALENGTH */            
   csw.DCSWDATARESIDUE = 0;
   csw.BCSWSTATUS = 0;
   
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_udc_mstorage_process_read_command
* Returned Value : None
* Comments       :
*     Sends data as a response to READ Command.
* 
*END*--------------------------------------------------------------------*/
void wpcm450_udc_mstorage_process_read_command
   (
      /* [IN] Handle of the USB device */
      _usb_device_handle   handle,
            
      /* [IN] Endpoint number */
      __u8               ep_num,
      
      /* [IN] Pointer to the data buffer */
      CBW_STRUCT_PTR       cbw_ptr
   )
{ /* Body */
   __u32 index1 = 0, index2 = 0;
   __u32 max_pkt_size;
   
   if (cbw_ptr->BMCBWFLAGS & USB_CBW_DIRECTION_BIT) {      
      /* Send a zero-length packet */
      index1  = ((__u32)cbw_ptr->CBWCB[4] << 8);
      index1  |= cbw_ptr->CBWCB[5];
      index2 = ((__u32)cbw_ptr->CBWCB[7] << 8);
      index2 |= (__u32)cbw_ptr->CBWCB[8];
      
      index2 *= LENGTH_OF_EACH_LAB;
      
      if (cbw_ptr->DCBWDATALENGTH == 0) { /* Body */
         csw.DCSWDATARESIDUE = 0;
         csw.BCSWSTATUS = 2;
         CBW_PROCESSED = FALSE;
         /* Send the command status information */
         _usb_device_send_data(handle, ep_num, (__u8*)&csw, 13);
         _usb_device_recv_data(handle, ep_num, wpcm450_mstorage_devs->data, 31);
         return;
      } else { /* Body */
         csw.DCSWDATARESIDUE = 0;
         csw.BCSWSTATUS = 0;         
         if (index2 > cbw_ptr->DCBWDATALENGTH) { /* Body */
            index2 = cbw_ptr->DCBWDATALENGTH;
            csw.DCSWDATARESIDUE = cbw_ptr->DCBWDATALENGTH;
            csw.BCSWSTATUS = 2;
         } else if (index2 < cbw_ptr->DCBWDATALENGTH) { /* Body */
            csw.DCSWDATARESIDUE = cbw_ptr->DCBWDATALENGTH - index2;
            if (index2 > 0) { /* Body */
               if (speed == USB_SPEED_HIGH) {
                  max_pkt_size = MSTORAGE_HS_MAX_PACKET_SIZE;
               } else {
                  max_pkt_size = MSTORAGE_FS_MAX_PACKET_SIZE;
               }
               if (index2%max_pkt_size == 0) { /* Body */
                  /* Need send a zero terminate packet to host */
                  ZERO_TERMINATE = TRUE;
               } /* Endbody */
            } /* Endbody */  
         } /* Endbody */
         _usb_device_send_data(handle, ep_num, 
            MASS_STORAGE_DISK + (index1*LENGTH_OF_EACH_LAB), 
            index2);
      } /* Endbody */
   } else { /* Body */
      /* Incorrect but valid CBW */
      if (cbw_ptr->DCBWDATALENGTH > WPCM450_UDC_MSTORAGE_BUFFERSIZE)
         index2 = WPCM450_UDC_MSTORAGE_BUFFERSIZE;
      else
         index2 = cbw_ptr->DCBWDATALENGTH;
      csw.DCSWDATARESIDUE = cbw_ptr->DCBWDATALENGTH;
      csw.BCSWSTATUS = 2;
       _usb_device_recv_data(handle, ep_num, wpcm450_mstorage_devs->data, index2);   
   } /* Endbody */  
   
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_udc_mstorage_process_write_command
* Returned Value : None
* Comments       :
*     Sends data as a response to WRITE Command.
* 
*END*--------------------------------------------------------------------*/
void wpcm450_udc_mstorage_process_write_command
   (
      /* [IN] Handle of the USB device */
      _usb_device_handle   handle,
            
      /* [IN] Endpoint number */
      __u8               ep_num,
      
      /* [IN] Pointer to the data buffer */
      CBW_STRUCT_PTR       cbw_ptr
   )
{ /* Body */
   __u32 index1 = 0, index2 = 0;
   
   if (!(cbw_ptr->BMCBWFLAGS & USB_CBW_DIRECTION_BIT)) {
      index1  = ((__u32)cbw_ptr->CBWCB[4] << 8);
      index1  |= cbw_ptr->CBWCB[5];
      index2 = ((__u32)cbw_ptr->CBWCB[7] << 8);
      index2 |= (__u32)cbw_ptr->CBWCB[8];
      
      if (cbw_ptr->DCBWDATALENGTH == 0) { /* Body */
         /* Zero transfer length */
         csw.DCSWDATARESIDUE = 0;
         csw.BCSWSTATUS = 2;
         CBW_PROCESSED = FALSE;
         /* Send the command status information */
         _usb_device_send_data(handle, ep_num, (__u8*)&csw, 13);
         _usb_device_recv_data(handle, ep_num, wpcm450_mstorage_devs->data, 31);
         return;
      } else { /* Body */
         csw.DCSWDATARESIDUE = 0;
         csw.BCSWSTATUS = 0;
         index2 *= LENGTH_OF_EACH_LAB;
         
         if (index2 < cbw_ptr->DCBWDATALENGTH) { /* Body */
            /* The actual length will never exceed the DCBWDATALENGTH */
            csw.DCSWDATARESIDUE = cbw_ptr->DCBWDATALENGTH - index2;
            index2 = cbw_ptr->DCBWDATALENGTH;
         } else if (index2 > cbw_ptr->DCBWDATALENGTH) { /* Body */
            csw.DCSWDATARESIDUE = cbw_ptr->DCBWDATALENGTH;
            csw.BCSWSTATUS = 2;
            index2 = cbw_ptr->DCBWDATALENGTH;
         } /* Endbody */
            
         if (_usb_device_get_transfer_status(handle, ep_num, USB_RECV) != USB_OK) {
            _usb_device_cancel_transfer(handle, ep_num, USB_RECV);
         } /* Endif */
               
         _usb_device_recv_data(handle, ep_num, 
            MASS_STORAGE_DISK + (index1*LENGTH_OF_EACH_LAB), 
            index2);
      }
   } else { /* Body */
      /* Incorrect but valid CBW */
      csw.DCSWDATARESIDUE = cbw_ptr->DCBWDATALENGTH;
      csw.BCSWSTATUS = 2;
      _usb_device_send_data(handle, ep_num, 0, 0);
      return;
   } /* Endbody */
   
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_udc_mstorage_process_test_unit_ready
* Returned Value : None
* Comments       :
*     Responds appropriately to unit ready query
* 
*END*--------------------------------------------------------------------*/
void wpcm450_udc_mstorage_process_test_unit_ready
   (
      /* [IN] Handle of the USB device */
      _usb_device_handle   handle,
            
      /* [IN] Endpoint number */
      __u8               ep_num,
      
      /* [IN] Pointer to the data buffer */
      CBW_STRUCT_PTR       cbw_ptr
   )
{ /* Body */
   __u32 BufSize;
  
   
   if ((cbw_ptr->BMCBWFLAGS & USB_CBW_DIRECTION_BIT) ||
       (cbw_ptr->DCBWDATALENGTH == 0)) {
      /* The actual length will never exceed the DCBWDATALENGTH */
      csw.DCSWDATARESIDUE = 0;
      csw.BCSWSTATUS = 0;
   
      CBW_PROCESSED = FALSE;
      /* Send the command status information */
      _usb_device_send_data(handle, 1, (__u8*)&csw, 13);
      _usb_device_recv_data(handle, 1, wpcm450_mstorage_devs->data, 31);
   } else { /* Body */
      /* Incorrect but valid CBW */
      if (cbw_ptr->DCBWDATALENGTH > WPCM450_UDC_MSTORAGE_BUFFERSIZE)
         BufSize = WPCM450_UDC_MSTORAGE_BUFFERSIZE;
      else
         BufSize = cbw_ptr->DCBWDATALENGTH;
      csw.DCSWDATARESIDUE = cbw_ptr->DCBWDATALENGTH;
      csw.BCSWSTATUS = 1;
      _usb_device_recv_data(handle, ep_num, wpcm450_mstorage_devs->data, BufSize);
   } /* Endbody */
   
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_udc_mstorage_process_prevent_allow_medium_removal
* Returned Value : None
* Comments       :
*     Responds appropriately to unit ready query
* 
*END*--------------------------------------------------------------------*/
void wpcm450_udc_mstorage_process_prevent_allow_medium_removal
   (
      /* [IN] Handle of the USB device */
      _usb_device_handle   handle,
            
      /* [IN] Endpoint number */
      __u8               ep_num,
      
      /* [IN] Pointer to the data buffer */
      CBW_STRUCT_PTR       cbw_ptr
   )
{ /* Body */

    
   /* The actual length will never exceed the DCBWDATALENGTH */
   csw.DCSWDATARESIDUE = 0;
   csw.BCSWSTATUS = 0;
   
   CBW_PROCESSED = FALSE;
   /* Send the command status information */
   _usb_device_send_data(handle, 1, (__u8*)&csw, 13);

   _usb_device_recv_data(handle, 1, wpcm450_mstorage_devs->data, 31);
   
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_udc_mstorage_process_mass_storage_command
* Returned Value : None
* Comments       :
*     Process a Mass storage class command
* 
*END*--------------------------------------------------------------------*/
void wpcm450_udc_mstorage_process_mass_storage_command
   (
      /* [IN] Handle of the USB device */
      _usb_device_handle   handle,
            
      /* [IN] Endpoint number */
      __u8               ep_num,
      
      /* [IN] Pointer to the data buffer */
      CBW_STRUCT_PTR       cbw_ptr
   )
{ /* Body */
   switch (cbw_ptr->CBWCB[0]) {
      case 0x00: /* Request the device to report if it is ready */
         wpcm450_udc_mstorage_process_test_unit_ready(handle, ep_num, cbw_ptr);
         break;
      case 0x12: /* Inquity command. Get device information */

         wpcm450_udc_mstorage_process_inquiry_command(handle, ep_num, cbw_ptr);
         break;
      case 0x1A:
         wpcm450_udc_mstorage_process_unsupported_command(handle, ep_num, cbw_ptr);
         break;
      case 0x1E: /* Prevent or allow the removal of media from a removable 
                 ** media device 
                 */
         wpcm450_udc_mstorage_process_prevent_allow_medium_removal(handle, ep_num, cbw_ptr);
         break;
      case 0x23: /* Read Format Capacities. Report current media capacity and 
                 ** formattable capacities supported by media 
                 */
         /* We bahave like already installed medium. No need to send any data */
         wpcm450_udc_mstorage_process_unsupported_command(handle, ep_num, cbw_ptr);
         break;
      case 0x25: /* Report current media capacity */
         wpcm450_udc_mstorage_process_report_capacity(handle, ep_num, cbw_ptr);
         break;
      case 0x28: /* Read (10) Transfer binary data from media to the host */
         wpcm450_udc_mstorage_process_read_command(handle, ep_num, cbw_ptr);
         break;
      case 0x2A: /* Write (10) Transfer binary data from the host to the 
                 ** media 
                 */
         wpcm450_udc_mstorage_process_write_command(handle, ep_num, cbw_ptr);
         break;
      case 0x01: /* Position a head of the drive to zero track */
      case 0x03: /* Transfer status sense data to the host */
      case 0x04: /* Format unformatted media */
      case 0x1B: /* Request a request a removable-media device to load or 
                 ** unload its media 
                 */
      case 0x1D: /* Perform a hard reset and execute diagnostics */
      case 0x2B: /* Seek the device to a specified address */
      case 0x2E: /* Transfer binary data from the host to the media and 
                 ** verify data 
                 */
      case 0x2F: /* Verify data on the media */
      case 0x55: /* Allow the host to set parameters in a peripheral */
      case 0x5A: /* Report parameters to the host */
      case 0xA8: /* Read (12) Transfer binary data from the media to the host */
      case 0xAA: /* Write (12) Transfer binary data from the host to the 
                 ** media 
                 */
      default:
         wpcm450_udc_mstorage_process_unsupported_command(handle, ep_num, cbw_ptr);
         break;
   } /* Endswitch */

} /* EndBody */


/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_udc_mstorage_service_ep1
* Returned Value : None
* Comments       :
*     Called upon a completed endpoint 1 (USB 1.1 Chapter 9) transfer
* 
*END*--------------------------------------------------------------------*/
void wpcm450_udc_mstorage_service_ep1
   (
      _usb_device_handle   handle,    /* [IN] Handle of the USB device                    */
      boolean              setup,     /* [IN] Is it a setup packet?                       */
      __u8                 direction, /* [IN] Direction of the transfer.  Is it transmit? */
      __u8*                buffer,    /* [IN] Pointer to the data buffer                  */
      __u32                length,    /* [IN] Length of the transfer                      */
      __u8                 error      /* [IN] Error, if any                               */
   )
{ /* Body */
   CBW_STRUCT_PTR cbw_ptr; 
  
   if ((!direction) && (!CBW_PROCESSED)) {
      cbw_ptr = (CBW_STRUCT_PTR)((void*)buffer);
   } /* Endif */

   if ((!direction) && (!CBW_PROCESSED) && (length == 31) && 
      (cbw_ptr->DCBWSIGNATURE == USB_DCBWSIGNATURE)) 
   {
      /* A valid CBW was received */
      csw.DCSWSIGNATURE = USB_DCSWSIGNATURE;
      csw.DCSWTAG = cbw_ptr->DCBWTAG;
      CBW_PROCESSED = TRUE;
      /* Process the command */
      wpcm450_udc_mstorage_process_mass_storage_command(handle, 1, cbw_ptr);
   } else {
      /* If a CBW was processed then send the status information and 
      ** queue another cbw receive request, else just queue another CBW receive
      ** request if we received an invalid CBW 
      */
      if (CBW_PROCESSED) {
         if (ZERO_TERMINATE) { /* Body */
            ZERO_TERMINATE = FALSE;
            _usb_device_send_data(handle, 1, 0, 0);
         } else { /* Body */
            CBW_PROCESSED = FALSE;
            /* Send the command status information */
            _usb_device_send_data(handle, 1, (__u8*)&csw, 13);        
            _usb_device_recv_data(handle, 1, wpcm450_mstorage_devs->data, 31);
         }
      } else if (!direction) {
         _usb_device_stall_endpoint(handle, 1, 0);
         _usb_device_stall_endpoint(handle, 1, 1);

         /* Invalid CBW received. Queue another receive buffer */
         _usb_device_recv_data(handle, 1, wpcm450_mstorage_devs->data, 31);  
      } /* Endif */
   } /* Endif */
   
   return;
} /* Endbody */




/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_udc_mstorage_reset_ep0
* Returned Value : None
* Comments       :
*     Called upon a bus reset event.  Initialises the control endpoint.
* 
*END*--------------------------------------------------------------------*/
void wpcm450_udc_mstorage_reset_ep0
   (
      /* [IN] Handle of the USB device */
      _usb_device_handle   handle,
   
      /* [IN] Unused */
      boolean              setup,
   
      /* [IN] Unused */
      __u8               direction,
   
      /* [IN] Unused */
      __u8*           buffer,
   
      /* [IN] Unused */
      __u32              length,

      /* [IN] Error, if any */
      __u8               error
            
   )
{ /* Body */

   /* on a reset always ensure all transfers are cancelled on control EP*/
   _usb_device_cancel_transfer(handle, 0, USB_RECV);
   _usb_device_cancel_transfer(handle, 0, USB_SEND);

   
   /* Initialize the endpoint 0 in both directions */
   _usb_device_init_endpoint(handle, 0, wpcm450_usb20_device_desc.bMaxPacketSize0, 
      USB_RECV, USB_CONTROL_ENDPOINT, 0);
   _usb_device_init_endpoint(handle, 0, wpcm450_usb20_device_desc.bMaxPacketSize0, 
      USB_SEND, USB_CONTROL_ENDPOINT, 0);

   //if (TEST_ENABLED) {
      _usb_device_cancel_transfer(handle, 1, USB_RECV);
      _usb_device_cancel_transfer(handle, 1, USB_SEND);
   //} /* Endif */
   
   //TEST_ENABLED = FALSE;
   
   return;
} /* EndBody */

/******************************************************************************/
/******************************************************************************/


/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_mstorage_transfer
* Returned Value : None
* Comments       :
*     Handle an I/O request.
* 
*END*--------------------------------------------------------------------*/ 
static void wpcm450_mstorage_transfer(
                    wpcm450_mstorage_dev_t *dev,
                    unsigned long sector,
                    unsigned long nsect,
                    char *buffer,
                    int write
                    )
{
  unsigned long offset = sector*KERNEL_SECTOR_SIZE;
  unsigned long nbytes = nsect*KERNEL_SECTOR_SIZE;

  if ((offset + nbytes) > dev->size) {
    printk (KERN_NOTICE "Beyond-end write (%ld %ld)\n", offset, nbytes);
    return;
  }
  
  if (write)
    memcpy(dev->data + offset, buffer, nbytes);
  else
    memcpy(buffer, dev->data + offset, nbytes);
}


/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_mstorage_request
* Returned Value : None
* Comments       :
*     The simple form of the request function.
* 
*END*--------------------------------------------------------------------*/ 
static void wpcm450_mstorage_request(
                    request_queue_t *q
                    )
{
  struct request *req;

  while ((req = elv_next_request(q)) != NULL) {
    wpcm450_mstorage_dev_t *dev = req->rq_disk->private_data;
    if (! blk_fs_request(req)) {
    printk (KERN_NOTICE "Skip non-fs request\n");
    end_request(req, 0);
    continue;
    }
    //    	printk (KERN_NOTICE "Req dev %d dir %ld sec %ld, nr %d f %lx\n",
    //    			dev - wpcm450_mstorage_devs, rq_data_dir(req),
    //    			req->sector, req->current_nr_sectors,
    //    			req->flags);
    wpcm450_mstorage_transfer(
          dev,
          req->sector,
          req->current_nr_sectors,
          req->buffer,
          rq_data_dir(req));
    end_request(req, 1);
  }
}


/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_mstorage_xfer_bio
* Returned Value : None
* Comments       :
*     Transfer a single BIO.
* 
*END*--------------------------------------------------------------------*/  
static int wpcm450_mstorage_xfer_bio(
                   wpcm450_mstorage_dev_t *dev,
                   struct bio *bio
                   )
{
  int i;
  struct bio_vec *bvec;
  char *buffer;
  sector_t sector = bio->bi_sector;

  /* Do each segment independently. */
  bio_for_each_segment(bvec, bio, i)
  {
    buffer = __bio_kmap_atomic(bio, i, KM_USER0);
    
    wpcm450_mstorage_transfer(
          dev,
          sector,
          bio_cur_sectors(bio),
          buffer,
          bio_data_dir(bio) == WRITE);

    sector += bio_cur_sectors(bio);
    __bio_kunmap_atomic(bio, KM_USER0);
  }
  return 0; /* Always "succeed" */
}


/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_mstorage_xfer_request
* Returned Value : None
* Comments       :
*     Transfer a full request.
* 
*END*--------------------------------------------------------------------*/ 
static int wpcm450_mstorage_xfer_request(
                   wpcm450_mstorage_dev_t *dev,
                   struct request *req
                   )
{
  struct bio *bio;
  int nsect = 0;
    
  rq_for_each_bio(bio, req)
  {
    wpcm450_mstorage_xfer_bio(dev, bio);
    nsect += bio->bi_size/KERNEL_SECTOR_SIZE;
  }
  return nsect;
}


/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_mstorage_full_request
* Returned Value : None
* Comments       :
*     Smarter request function that "handles clustering".
* 
*END*--------------------------------------------------------------------*/  
static void wpcm450_mstorage_full_request(
                    request_queue_t *q
                    )
{
  struct request *req;
  int sectors_xferred;
  wpcm450_mstorage_dev_t *dev = q->queuedata;

  while ((req = elv_next_request(q)) != NULL)
  {
    if (! blk_fs_request(req)) 
    {
      printk (KERN_NOTICE "Skip non-fs request\n");
      end_request(req, 0);
      continue;
    }
    sectors_xferred = wpcm450_mstorage_xfer_request(dev, req);
    if (! end_that_request_first(req, 1, sectors_xferred))
    {
      blkdev_dequeue_request(req);
      end_that_request_last(req, 1);
    }
  }
}




/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_mstorage_make_request
* Returned Value : None
* Comments       :
*     The direct make request version.
* 
*END*--------------------------------------------------------------------*/ 
static int wpcm450_mstorage_make_request(
                   request_queue_t *q,
                   struct bio *bio
                   )
{
  wpcm450_mstorage_dev_t *dev = q->queuedata;
  int status;

  status = wpcm450_mstorage_xfer_bio(dev, bio);
  bio_endio(bio, bio->bi_size, status);
  return 0;
}



/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_mstorage_open
* Returned Value : None
* Comments       :
*     Open.
* 
*END*--------------------------------------------------------------------*/ 
static int wpcm450_mstorage_open(
                   struct inode *inode,
                   struct file *filp
                   )
{
  wpcm450_mstorage_dev_t *dev = inode->i_bdev->bd_disk->private_data;

  del_timer_sync(&dev->timer);
  filp->private_data = dev;
  spin_lock(&dev->lock);
  if (! dev->users) 
    check_disk_change(inode->i_bdev);
  dev->users++;
  spin_unlock(&dev->lock);
  return 0;
}

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_mstorage_release
* Returned Value : None
* Comments       :
*     Close.
* 
*END*--------------------------------------------------------------------*/ 
static int wpcm450_mstorage_release(
                   struct inode *inode,
                   struct file *filp
                   )
{
  wpcm450_mstorage_dev_t *dev = inode->i_bdev->bd_disk->private_data;

  spin_lock(&dev->lock);
  dev->users--;

  if (!dev->users)
  {
    dev->timer.expires = jiffies + INVALIDATE_DELAY;
    add_timer(&dev->timer);
  }
  spin_unlock(&dev->lock);

  return 0;
}


/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_mstorage_media_changed
* Returned Value : None
* Comments       :
*     Look for a (simulated) media change.
* 
*END*--------------------------------------------------------------------*/  
int wpcm450_mstorage_media_changed(
            struct gendisk *gd
            )
{
  wpcm450_mstorage_dev_t *dev = gd->private_data;

  return dev->media_change;
}



/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_mstorage_revalidate
* Returned Value : None
* Comments       :
*     Revalidate.  WE DO NOT TAKE THE LOCK HERE, for fear of deadlocking
*     with open.  That needs to be reevaluated.
* 
*END*--------------------------------------------------------------------*/  
int wpcm450_mstorage_revalidate(
            struct gendisk *gd
            )
{
  wpcm450_mstorage_dev_t *dev = gd->private_data;

  if (dev->media_change)
  {
    dev->media_change = 0;
    memset (dev->data, 0, dev->size);
  }
  return 0;
}


/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_mstorage_invalidate
* Returned Value : None
* Comments       :
*     The "invalidate" function runs out of the device timer; it sets
*     a flag to simulate the removal of the media.
* 
*END*--------------------------------------------------------------------*/ 
void wpcm450_mstorage_invalidate(
             unsigned long ldev
             )
{
  wpcm450_mstorage_dev_t *dev = (wpcm450_mstorage_dev_t *) ldev;

  spin_lock(&dev->lock);
  if (dev->users || !dev->data) 
    printk (KERN_WARNING "wpcm450_mstorage: timer sanity check failed\n");
  else
    dev->media_change = 1;
  spin_unlock(&dev->lock);
}


/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_mstorage_ioctl
* Returned Value : None
* Comments       :
*     The ioctl() implementation
*     
* 
*END*--------------------------------------------------------------------*/ 
int wpcm450_mstorage_ioctl (
            struct inode *inode,
            struct file *filp,
            unsigned int cmd,
            unsigned long arg
            )
{
  long size;
  struct hd_geometry geo;
  wpcm450_mstorage_dev_t *dev = filp->private_data;

  switch(cmd)
  {
  case HDIO_GETGEO:
    /*
     * Get geometry: since we are a virtual device, we have to make
     * up something plausible.  So we claim 16 sectors, four heads,
     * and calculate the corresponding number of cylinders.  We set the
     * start of data at sector four.
     */
    size = dev->size*(hardsect_size/KERNEL_SECTOR_SIZE);
    geo.cylinders = (size & ~0x3f) >> 6;
    geo.heads = 4;
    geo.sectors = 16;
    geo.start = 4;
    if (copy_to_user((void __user *) arg, &geo, sizeof(geo)))
      return -EFAULT;
    return 0;
  }

  return -ENOTTY; /* unknown command */
}



/*
 * The device operations structure.
 */
static struct block_device_operations wpcm450_mstorage_ops = {
  .owner           = THIS_MODULE,
  .open            = wpcm450_mstorage_open,
  .release         = wpcm450_mstorage_release,
  .media_changed   = wpcm450_mstorage_media_changed,
  .revalidate_disk = wpcm450_mstorage_revalidate,
  .ioctl           = wpcm450_mstorage_ioctl
};



/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_mstorage_setup_device
* Returned Value : None
* Comments       :
*     Set up our internal device.
*     
* 
*END*--------------------------------------------------------------------*/  
static void wpcm450_mstorage_setup_device(
                    wpcm450_mstorage_dev_t *dev,
                    int which
                    )
{
  /*
   * Get some memory.
   */
  memset (dev, 0, sizeof (wpcm450_mstorage_dev_t));
  dev->size = nsectors*hardsect_size;
  dev->data = vmalloc(dev->size);
  if (dev->data == NULL) 
  {
    printk (KERN_NOTICE "vmalloc failure.\n");
    return;
  }
  spin_lock_init(&dev->lock);

  /*
   * The timer which "invalidates" the device.
   */
  init_timer(&dev->timer);
  dev->timer.data = (unsigned long) dev;
  dev->timer.function = wpcm450_mstorage_invalidate;
  
  /*
   * The I/O queue, depending on whether we are using our own
   * make_request function or not.
   */
  switch (request_mode) 
  {
  case RM_NOQUEUE:
    dev->queue = blk_alloc_queue(GFP_KERNEL);
    if (dev->queue == NULL)
      goto out_vfree;
    blk_queue_make_request(dev->queue, wpcm450_mstorage_make_request);
    break;

  case RM_FULL:
    dev->queue = blk_init_queue(wpcm450_mstorage_full_request, &dev->lock);
    if (dev->queue == NULL)
      goto out_vfree;
    break;

  default:
    printk(KERN_NOTICE "Bad request mode %d, using simple\n", request_mode);
    /* fall into.. */

  case RM_SIMPLE:
    dev->queue = blk_init_queue(wpcm450_mstorage_request, &dev->lock);
    if (dev->queue == NULL)
      goto out_vfree;
    break;
  }

  blk_queue_hardsect_size(dev->queue, hardsect_size);
  dev->queue->queuedata = dev;
  /*
   * And the gendisk structure.
   */
  dev->gd = alloc_disk(USB_MSTORAGE_MINORS);
  if (! dev->gd) 
  {
    printk (KERN_NOTICE "alloc_disk failure\n");
    goto out_vfree;
  }
  dev->gd->major = wpcm450_mstorage_major;
  dev->gd->first_minor = which*USB_MSTORAGE_MINORS;
  dev->gd->fops = &wpcm450_mstorage_ops;
  dev->gd->queue = dev->queue;
  dev->gd->private_data = dev;
  snprintf (dev->gd->disk_name, 32, "wpcm450_mstorage%c", which + 'a');
  set_capacity(dev->gd, nsectors*(hardsect_size/KERNEL_SECTOR_SIZE));
  add_disk(dev->gd);
  return;

  out_vfree:
  if (dev->data)
    vfree(dev->data);
}


/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_mstorage_init
* Returned Value : None
* Comments       :
*     Module init.
*     
* 
*END*--------------------------------------------------------------------*/  
static int __init wpcm450_mstorage_init(void)
{
  int i;
  /*
   * Get registered.
   */

  if (register_blkdev(wpcm450_mstorage_major, "gusb2")) {
    printk(KERN_WARNING "wpcm450_mstorage: failed to register device\n");
    return -EBUSY;
  }
  /*
   * Allocate the device array, and initialize each one.
   */
  wpcm450_mstorage_devs = kmalloc(ndevices*sizeof (wpcm450_mstorage_dev_t), GFP_KERNEL);
  
  if (wpcm450_mstorage_devs == NULL)
    goto out_unregister;

  wpcm450_mstorage_devs->data  = kmalloc(WPCM450_UDC_MSTORAGE_BUFFERSIZE*sizeof (__u8), GFP_KERNEL);

  if (wpcm450_mstorage_devs->data == NULL)
    goto out_unregister;
  
  for (i = 0; i < ndevices; i++) 
    wpcm450_mstorage_setup_device(wpcm450_mstorage_devs + i, i);

  wpcm450_udc_mstorage_bind(NULL);
  
  return 0;

  out_unregister:
  unregister_blkdev(wpcm450_mstorage_major, "sbd");
  return -ENOMEM;
}

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_mstorage_exit
* Returned Value : None
* Comments       :
*     Module exit.
*     
* 
*END*--------------------------------------------------------------------*/ 
static void wpcm450_mstorage_exit(void)
{
  int i;

  for (i = 0; i < ndevices; i++)
  {
    wpcm450_mstorage_dev_t *dev = wpcm450_mstorage_devs + i;

    del_timer_sync(&dev->timer);
    if (dev->gd)
    {
      del_gendisk(dev->gd);
      put_disk(dev->gd);
    }

    if (dev->queue)
    {
      if (request_mode == RM_NOQUEUE)
        blk_put_queue(dev->queue);
      else
        blk_cleanup_queue(dev->queue);
    }
    if (dev->data)
      vfree(dev->data);
  }
  unregister_blkdev(wpcm450_mstorage_major, "wpcm450_mstorage");
  kfree(wpcm450_mstorage_devs);
}


module_init(wpcm450_mstorage_init);
module_exit(wpcm450_mstorage_exit);



