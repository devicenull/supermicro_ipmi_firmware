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
#include <linux/cdev.h>


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
#define GUSB_MAJOR                          (64)

#define MIN(a,b)                            ((a) < (b) ? (a) : (b)) 
#define EP1_RECV_BUFFER_SIZE                (10)

#define WPCM450_UDC_VERSION_STR             "v1.0"
#define WPCM450_UDC_VERSION_NUM             0x0100

#define WPCM450_UDC_LONG_NAME               "WPCM450 USB Gadget Driver"
#define WPCM450_UDC_SHORT_NAME              "wpcm450_usb"


#define WPCM450_UDC_NOTIFY_MAXPACKET        5 /* 1 << 5 == 32 msec */
#define WPCM450_UDC_LOG2_NOTIFY_INTERVAL    8 


#define WPCM450_UDC_VENDOR_ID               0x0557   /*0x0B20*/
#define WPCM450_UDC_PRODUCT_ID              0x2221   /*0x1*/
#define WPCM450_UDC_NUM_CONFIGS             1
#define WPCM450_UDC_NO_CONFIG_ID            0
#define WPCM450_UDC_BULK_CONFIG_ID          1
#define WPCM450_UDC_ACM_CONFIG_ID           2
#define WPCM450_UDC_INT_CONFIG_ID           1


/* USB descriptors */

#define WPCM450_UDC_SERIAL_STR_ID           0
#define WPCM450_UDC_MANUFACTURER_STR_ID     1
#define WPCM450_UDC_PRODUCT_STR_ID          2

#define WPCM450_UDC_INT_CONFIG_STR_ID       0
#define WPCM450_UDC_CONTROL_STR_ID          4
#define WPCM450_UDC_DATA_STR_ID             5



#define WPCM450_UDC_MAX_NUM_INTERFACES      2
#define WPCM450_UDC_INT_INTERFACE_ID        0
#define WPCM450_UDC_CONTROL_INTERFACE_ID    0
#define WPCM450_UDC_DATA_INTERFACE_ID       1
#define WPCM450_UDC_NUM_ENDPOINTS           2
#define WPCM450_UDC_MAX_DESC_LEN            1500



#define WPCM450_MOUSE_INTERFACE_NUM         0
#define WPCM450_KEYBOARD_INTERFACE_NUM      1




#define  CONTROL_MAX_PACKET_SIZE            (64)
#define  INTERRUPT_MAX_PACKET_SIZE          (8)


#define  MOUSE_EP                           (2)
#define  KEYBOARD_EP                        (1)


#define CONFIG_DESC_SIZE                    (34)

#define MAX_NUM_GADGETS_PER_DEV             (3)

#define MAX_BUFFER_LENGTH  (sizeof(struct usb_config_descriptor)+\
            MAX_NUM_GADGETS_PER_DEV*sizeof(struct usb_interface_descriptor)+\
            MAX_NUM_GADGETS_PER_DEV*sizeof(struct usb_endpoint_descriptor)+\
            MAX_NUM_GADGETS_PER_DEV*sizeof(struct hid_class_descriptor))
            
/*********************************************************************************/
/* Types Definitions                                                             */
/*********************************************************************************/

typedef enum {

  WPCM450_USB_1_1_DEV_CTRL,
  WPCM450_USB_2_0_DEV_CTRL,
  WPCM450_MAX_DEV_PORT

}wpcm450_usb_dev_ctrl_t;


typedef struct {
  struct cdev cdev;
  void*  handle;
} wpcm450_dev_t;

typedef struct _keyboard_data {
   __u8  a;
   __u8  b;
   __u8  c;
   __u8  d;
} keyboard_data_t;

typedef struct wpcm450_ep {
  struct usb_ep  ep;
}wpcm450_ep_t;

typedef struct wpcm450_udc {
  struct usb_gadget         gadget;
  struct wpcm450_ep         ep[WPCM450_UDC_NUM_ENDPOINTS];
  struct usb_gadget_driver *driver;
  void*  handler;
}wpcm450_udc_t;


typedef struct hid_class_descriptor {
    __u8  bLength;
    __u8  bDescriptorType;
    __u16 bcdHID;
    __u8  bCountryCode;
    __u8  bNumDescriptors;
    __u8  bReportDescriptorType;
    __u16 wDescriptorLength;
}__attribute__ ((packed));

typedef struct usb_ctrlrequest* setup_struct_ptr_t;

/*****************************************************************************/
/* Variables                                                                 */
/*****************************************************************************/

static struct usb_device_descriptor wpcm450_usb11_device_desc = {

  .bLength            = sizeof(struct usb_device_descriptor),
  .bDescriptorType    = USB_DT_DEVICE,
  .bcdUSB             = __constant_cpu_to_le16(0x0110),
  .bDeviceClass       = 0,
  .bDeviceSubClass    = 0,
  .bDeviceProtocol    = 0,
  .bMaxPacketSize0    = CONTROL_MAX_PACKET_SIZE,
  .idVendor           = __constant_cpu_to_le16(WPCM450_UDC_VENDOR_ID),
  .idProduct          = __constant_cpu_to_le16(WPCM450_UDC_PRODUCT_ID),
  .bcdDevice          = __constant_cpu_to_le16(0x1),
  .iManufacturer      = WPCM450_UDC_MANUFACTURER_STR_ID,
  .iProduct           = WPCM450_UDC_PRODUCT_STR_ID,
  .iSerialNumber      = WPCM450_UDC_SERIAL_STR_ID,
  .bNumConfigurations = WPCM450_UDC_NUM_CONFIGS,

};


static struct usb_config_descriptor wpcm450_udc_config_desc = {
  
  .bLength             = sizeof(struct usb_config_descriptor),
  .bDescriptorType     = USB_DT_CONFIG,
  .wTotalLength        = 0, /* must be updated in run time */
  .bNumInterfaces      = WPCM450_UDC_MAX_NUM_INTERFACES,
  .bConfigurationValue = WPCM450_UDC_INT_CONFIG_ID,
  .iConfiguration      = WPCM450_UDC_INT_CONFIG_STR_ID,
  .bmAttributes        = USB_CONFIG_ATT_ONE | USB_CONFIG_ATT_WAKEUP | USB_CONFIG_ATT_SELFPOWER,
  .bMaxPower           = 0x32,
};


static struct usb_interface_descriptor wpcm450_mouse_intf_desc = {

  .bLength            = sizeof (struct usb_interface_descriptor),
  .bDescriptorType    = USB_DT_INTERFACE,
  .bInterfaceNumber   = WPCM450_MOUSE_INTERFACE_NUM,
  .bAlternateSetting  = 0,
  .bNumEndpoints      = 1,
  .bInterfaceClass    = USB_CLASS_HID,
  .bInterfaceSubClass = 1,
  .bInterfaceProtocol = 2,
  .iInterface         = 0,

};

static struct usb_interface_descriptor wpcm450_keyboard_intf_desc = {
  
  .bLength            = sizeof (struct usb_interface_descriptor),
  .bDescriptorType    = USB_DT_INTERFACE,
  .bInterfaceNumber   = WPCM450_KEYBOARD_INTERFACE_NUM,
  .bAlternateSetting  = 0,
  .bNumEndpoints      = 1,
  .bInterfaceClass    = USB_CLASS_HID,
  .bInterfaceSubClass = 1,
  .bInterfaceProtocol = 1,
  .iInterface         = 0,

};


static struct usb_endpoint_descriptor wpcm450_mouse_ep_desc = {
  
  .bLength            = USB_DT_ENDPOINT_SIZE,
  .bDescriptorType    = USB_DT_ENDPOINT,
  .bEndpointAddress   = USB_DIR_IN + MOUSE_EP/* interrup endpoint */,
  .bmAttributes       = USB_ENDPOINT_XFER_INT,
  .wMaxPacketSize     = __constant_cpu_to_le16(INTERRUPT_MAX_PACKET_SIZE),
  .bInterval          = 0xA,
};

static struct usb_endpoint_descriptor wpcm450_keyboard_ep_desc = {
  
  .bLength            = USB_DT_ENDPOINT_SIZE,
  .bDescriptorType    = USB_DT_ENDPOINT,
  .bEndpointAddress   = USB_DIR_IN + KEYBOARD_EP/* interrup endpoint */,
  .bmAttributes       = USB_ENDPOINT_XFER_INT,
  .wMaxPacketSize     = __constant_cpu_to_le16(INTERRUPT_MAX_PACKET_SIZE),
  .bInterval          = 0xA,
};

static __u8 config_desc_buffer[MAX_BUFFER_LENGTH+1];

#define DEVICE_QUALIFIER_DESCRIPTOR_SIZE 10

static struct usb_qualifier_descriptor wpcm450_usb11_qualifier_desc = {

  .bLength = sizeof(struct usb_qualifier_descriptor),  
  .bDescriptorType = 6,                                
  .bcdUSB = __constant_cpu_to_le16(0x0110),
  .bDeviceClass = 0,                                 
  .bDeviceSubClass = 0,                                 
  .bDeviceProtocol = 0,                               
  .bMaxPacketSize0 = CONTROL_MAX_PACKET_SIZE,
  .bNumConfigurations = 0x01,
  .bRESERVED = 0,

};


typedef enum {
  
    USAGE_PAGE     = 0x05,
    USAGE          = 0x09,
    COLLECTION     = 0xA1,
    USAGE_MIN      = 0x19,
    USAGE_MAX      = 0x29,
    LOGICAL_MIN    = 0x15,
    LOGICAL_MAX    = 0x25,
    REPORT_COUNT   = 0x95,
    REPORT_SIZE    = 0x75,
    INPUT          = 0x81,
    OUTPUT         = 0x91,
    END_COLLECTION = 0xC0,
    LAST_ITEM      = 0xFF
    
} HID_report_items_t;


/************************************************
 * Mouse Report descriptor includes 3 bytes:
 * 1st byte: 7,6 bits - two buttoms
 * 2nd byte: X-position relative
 * 3rd byte: Y-position relative
 ************************************************/
static const __u8 hid_report_mouse[] =
{
  USAGE_PAGE,    0x01, /* Usage Page (Generic Desktop) */
  USAGE,         0x02, /* Usage (Mouse) */
  COLLECTION,    0x01, /* Collection (Application) */
  USAGE,         0x01, /*   Usage (Pointer) */
  COLLECTION,    0x00, /*     Collection (Physical) */
  USAGE_PAGE,    0x09, /*     Usage Page (Buttons) */
  USAGE_MIN,     0x01, /*     Usage Minimum (1) */
  USAGE_MAX,     0x02, /*     Usage Maximum (2) */
  LOGICAL_MIN,   0x00, /*     Logical Minimum (0) */
  LOGICAL_MAX,   0x01, /*     Logical Maximum (1) */
  REPORT_COUNT,  0x05, /*     Report Count (2) */
  REPORT_SIZE,   0x01, /*     Report Size (1) */
  INPUT,         0x02, /*     Input (Data, Variable, Absolute) */
  REPORT_COUNT,  0x01, /*     Report Count (1) */
  REPORT_SIZE,   0x03, /*     Report Size (6) */
  INPUT,         0x01, /*     Input (Constant, Variable, Absolute) */
  USAGE_PAGE,    0x01, /*     Usage Page (Generic Desktop) */
  USAGE,         0x30, /*     Usage (X) */
  USAGE,         0x31, /*     Usage (Y) */
  USAGE,         0x38, /*     Usage (Y) */
  LOGICAL_MIN,   0x81, /*     Logical Minimum (-127) */
  LOGICAL_MAX,   0x7F, /*     Logical Maximum (127) */
  REPORT_SIZE,   0x08, /*     Report Size (8) */
  REPORT_COUNT,  0x03, /*     Report Count (2) */
  INPUT,         0x06, /*     Input (Data, Variable, Relative) */
  END_COLLECTION,
  END_COLLECTION
};

#define MOUSE_REPORT_LEN 3

/* TVRC Keypade Report descriptor */
static const __u8 hid_report_keypad[] =
{
  USAGE_PAGE,    0x01, /* Usage Page (Generic Desktop) */
  USAGE,         0x06, /* Usage (Keypad) */
  REPORT_COUNT,  0x00, /* Report Count (0) */
  COLLECTION,    0x01, /* Collection (Application) */
  USAGE_PAGE,    0x07, /*   Usage Page (Keyboard) */

  USAGE,         0x00, /*   Usage Minimum (Reserved (no event indicated)) */
  USAGE_MIN,     0xe0, /*   Usage Minimum (Keyboard LeftControl) */
  USAGE_MAX,     0xe7, /*   Usage Maximum (Keyboard Right GUI) */
  LOGICAL_MIN,   0x00, /*   Logical Minimum (0) */
  LOGICAL_MAX,   0x01, /*   Logical Maximum (1) */
  REPORT_SIZE,   0x01, /*   Report Size (1) */
  REPORT_COUNT,  0x08, /*   Report Count (8) */
  INPUT,         0x02, /*   Input (Data, Variable, Absolute) */

  USAGE,         0x00, /*   Usage Minimum (Reserved (no event indicated)) */
  USAGE_MIN,     0x01, /*   Usage Minimum (Keyboard ErrorRollOver) */
  USAGE_MAX,     0x65, /*   Usage Maximum (Keyboard UpArrow) */
  LOGICAL_MIN,   0x00, /*   Logical Minimum (no event) */
  LOGICAL_MAX,   0x65, /*   Logical Maximum (Keyboard UpArrow) */
  REPORT_SIZE,   0x08, /*   Report Size (8) */
  REPORT_COUNT,  0x02, /*   Report Count (2) */
  INPUT,         0x00, /*   Input (Variable, Array, Absolute) */
  END_COLLECTION
};

#define KEYPAD_REPORT_LEN 3

/* PS/2 Keypade Report descriptor */
static const __u8 hid_report_keyboard[] =
{
  USAGE_PAGE,    0x01, /* Usage Page (Generic Desktop) */
  USAGE,         0x06, /* Usage (Keyboard) */
  COLLECTION,    0x01,
  USAGE_PAGE,    0x07,
  USAGE_MIN,     0xE0,
  USAGE_MAX,     0xE7,
  LOGICAL_MIN,   0x00, 
  LOGICAL_MAX,   0x01, 
  REPORT_SIZE,   0x01, 
  REPORT_COUNT,  0x08, 
  INPUT,         0x02, 
  REPORT_COUNT,  0x01, 
  REPORT_SIZE,   0x08, 
  INPUT,         0x01,
  REPORT_COUNT,  0x05, 
  REPORT_SIZE,   0x01,
  USAGE_PAGE,    0x08,
  USAGE_MIN,     0x01,
  USAGE_MAX,     0x05,
  OUTPUT,        0x02,
  REPORT_COUNT,  0x01, 
  REPORT_SIZE,   0x03,
  OUTPUT,        0x01,
  USAGE_PAGE,    0x07,
  REPORT_COUNT,  0x06, 
  REPORT_SIZE,   0x08,
  LOGICAL_MIN,   0x00,
  0x26,          
  LAST_ITEM,     0x00,
  USAGE_MIN,     0x00,
  0x2A,
  LAST_ITEM,     0x00,
  INPUT,         0x00,
  END_COLLECTION
};

#define KEYBOARD_REPORT_LEN  7


static struct hid_class_descriptor wpcm450_hid_mouse_desc = {

  .bLength               = sizeof(struct hid_class_descriptor), 
  .bDescriptorType       = 0x21,
  .bcdHID                = __constant_cpu_to_le16(0x0100),
  .bCountryCode          = 0x00,
  .bNumDescriptors       = 0x01,
  .bReportDescriptorType = 0x22,
  .wDescriptorLength     = __constant_cpu_to_le16(sizeof(hid_report_mouse)), 
  
};

static struct hid_class_descriptor wpcm450_hid_keyboard_desc = {
  
  .bLength               = sizeof(struct hid_class_descriptor), 
  .bDescriptorType       = 0x21,
  .bcdHID                = __constant_cpu_to_le16(0x0100),
  .bCountryCode          = 0x00,
  .bNumDescriptors       = 0x01,
  .bReportDescriptorType = 0x22,
  .wDescriptorLength     = __constant_cpu_to_le16(sizeof(hid_report_keyboard)),
};



/* number of strings in the table not including 0 or n. */
const __u8 USB_STR_NUM = 6;

const __u16 USB_STR_0[ 2] = {0x300 + sizeof(USB_STR_0),0x0409};
const __u16 USB_STR_1[25] = {0x300 + sizeof(USB_STR_1),
      'W','i','n','b','o','n','d',' ','E','l','e','c','t','r','o','n','i',\
      'c','s',' ','C','o','r','p'};
const __u16 USB_STR_2[27] = {0x300 + sizeof(USB_STR_2),
      'H','e','r','m','o','n',' ','U','S','B',' ','h','i','d','m','o','u',\
      's','e',' ','D','e','v','i','c','e'};
const __u16 USB_STR_3[ 5] = {0x300 + sizeof(USB_STR_3),
      'D','e','m','o'};
const __u16 USB_STR_4[ 4] = {0x300 + sizeof(USB_STR_4),
      '#','0','2'};
const __u16 USB_STR_5[ 4] = {0x300 + sizeof(USB_STR_5),
      '_','A','1'};
const __u16 USB_STR_6[ 8] = {0x300 + sizeof(USB_STR_6),
      'W','i','n','b','o','n','d'};
const __u16 USB_STR_n[17] = {0x300 + sizeof(USB_STR_n),
      'B','A','D',' ','S','T','R','I','N','G',' ','I','n','d','e','x'};



#define USB_STRING_ARRAY_SIZE  8
const __u8* USB_STRING_DESC[USB_STRING_ARRAY_SIZE] =
{
   (__u8*)USB_STR_0,
   (__u8*)USB_STR_1,
   (__u8*)USB_STR_2,
   (__u8*)USB_STR_3,
   (__u8*)USB_STR_4,
   (__u8*)USB_STR_5,
   (__u8*)USB_STR_6,
   (__u8*)USB_STR_n
};


static volatile __u8 data[8];
static int wpcm450_major =   GUSB_MAJOR;
static int wpcm450_minor =   0;
static int wpcm450_nr_devs = 2;
static wpcm450_dev_t *wpcm450_devices;
int g_init_cnt = 0;

/**********************************************************************
Mouse data (this structure is used to send mouse movement information)
**********************************************************************/
#define _USB_WRITE_MUTEX_USED_
#ifdef _USB_WRITE_MUTEX_USED_
struct semaphore keyboard_data_mutex;
struct semaphore mouse_data_mutex;
#endif
static wpcm450_udc_t wpcm450_udc_dev;
static struct usb_ctrlrequest local_setup_packet;
static volatile __u8 usb_status[2];
static volatile __u8 endpoint, if_status;
static volatile __u8 data_to_send;
static __u16 sof_count;
static __u16 setup_packet[sizeof(struct usb_ctrlrequest)];
__u8 usb_if_alt[4] = { 0, 0, 0, 0};
__u8  hid_test_rep_data[EP1_RECV_BUFFER_SIZE];

volatile boolean TEST_ENABLED = FALSE;


/*****************************************************************************/
/* Functions                                                                 */
/*****************************************************************************/
static int wpcm450_udc_bind(struct usb_gadget *gadget);
static void wpcm450_udc_unbind(struct usb_gadget *gadget);
static int wpcm450_udc_setup(
                   struct usb_gadget *gadget,
                   const struct usb_ctrlrequest *ctrl);
static void wpcm450_udc_disconnect(struct usb_gadget *gadget);
static void wpcm450_service_ep0(
                    _usb_device_handle   handle,    /* [IN] Handle of the USB device                    */
                    boolean              setup,     /* [IN] Is it a setup packet?                       */
                    __u8                 direction, /* [IN] Direction of the transfer.  Is it transmit? */
                    __u8*                buffer,    /* [IN] Pointer to the data buffer                  */
                    __u32                length,    /* [IN] Length of the transfer                      */
                    __u8                 error      /* [IN] Error, if any                               */
                    );

static void wpcm450_service_ep1 (
                    _usb_device_handle   handle,    /* [IN] Handle of the USB device                    */
                    boolean              setup,     /* [IN] Is it a setup packet?                       */
                    __u8                 direction, /* [IN] Direction of the transfer.  Is it transmit? */
                    __u8*                buffer,    /* [IN] Pointer to the data buffer                  */
                    __u32                length,    /* [IN] Length of the transfer                      */
                    __u8                 error      /* [IN] Error, if any                               */
                    );
   
static void wpcm450_service_ep2 (
                    _usb_device_handle   handle,    /* [IN] Handle of the USB device                    */
                    boolean              setup,     /* [IN] Is it a setup packet?                       */
                    __u8                 direction, /* [IN] Direction of the transfer.  Is it transmit? */
                    __u8*                buffer,    /* [IN] Pointer to the data buffer                  */
                    __u32                length,    /* [IN] Length of the transfer                      */
                    __u8                 error      /* [IN] Error, if any                               */
                    );

void wpcm450_reset_ep0 (
             _usb_device_handle   handle,    /* [IN] Handle of the USB device                    */
             boolean              setup,     /* [IN] Is it a setup packet?                       */
             __u8                 direction, /* [IN] Direction of the transfer.  Is it transmit? */
             __u8*                buffer,    /* [IN] Pointer to the data buffer                  */
             __u32                length,    /* [IN] Length of the transfer                      */
             __u8                 error      /* [IN] Error, if any                               */
             );

/* gadget driver struct */
static struct usb_gadget_driver wpcm450_udc_gadget_driver = {
  .speed      = USB_SPEED_FULL,
  .function   = WPCM450_UDC_LONG_NAME,
  .bind       = wpcm450_udc_bind,
  .unbind     = wpcm450_udc_unbind,
  .setup      = wpcm450_udc_setup,
  .disconnect = wpcm450_udc_disconnect,
  .driver     = {
  .name       =   WPCM450_UDC_SHORT_NAME,
  },
};


/*****************************************************************************/
/*****************************************************************************/

EXPORT_SYMBOL(usb_gadget_register_driver);



/*****************************************************************************/
/*****************************************************************************/


/* when a driver is successfully registered, it will receive
 * control requests including set_configuration(), which enables
 * non-control requests.  then usb traffic follows until a
 * disconnect is reported.  then a host may connect again, or
 * the driver might get unbound.
 */
int usb_gadget_register_driver(struct usb_gadget_driver *driver)
{
  wpcm450_udc_t *dev = &wpcm450_udc_dev;
  int retval;
  

  if (!driver
      || !((driver->speed == USB_SPEED_LOW) || (driver->speed == USB_SPEED_FULL))
      || !driver->bind
      || !driver->unbind
      || !driver->disconnect
      || !driver->setup)
  {
    return -EINVAL;
  }
  
  if (!dev)
  {
    return -ENODEV;
  }

  
  if (dev->driver)
  {
    return -EBUSY;
  }

    
  /* first hook up the driver ... */
  dev->driver = driver;
  dev->gadget.dev.driver = &driver->driver;
  

  retval = driver->bind(&dev->gadget);


  if (retval) 
  {
    printk("bind to driver %s --> error %d\n",
           driver->driver.name, retval);
    
    dev->driver = NULL;
    dev->gadget.dev.driver = NULL;
    return retval;
  }

  /* ... then enable host detection and ep0; and we're ready
   * for set_configuration as well as eventual disconnect.
   */


  printk("registered gadget driver '%s'\n", driver->driver.name);

  return 0;
}





/* Gadget Driver */


/*
 * wpcm450_udc_bind
 *
 * Called on module load.  Allocates and initializes the device
 * structure and a control request.
 */
static int  wpcm450_udc_bind(struct usb_gadget *gadget)
{
  int ret;
  struct usb_ep *ep;
  wpcm450_udc_t *dev;
  int gcnum;
  _usb_device_handle handle = NULL;


  ret = _usb_device_init(WPCM450_USB11_PORT, &handle, 2);

  if(ret != 0)
  {
    printk(KERN_INFO "wpcm450_udc_bind: %s %s bound\n",
    WPCM450_UDC_LONG_NAME, WPCM450_UDC_VERSION_STR);
  }

  wpcm450_udc_dev.handler = handle;
  
  ret = _usb_device_register_service(handle, USB_SERVICE_EP0, wpcm450_service_ep0);
   
  if (ret != USB_OK) {
      printk(KERN_ERR "\nUSB Service Registration failed. Error: %x", ret);
      goto autoconf_fail;
  } /* Endif */

  printk(KERN_INFO "\nUSB Service Registration EP0 OK\n");
   
  ret = _usb_device_register_service(handle, USB_SERVICE_BUS_RESET,  wpcm450_reset_ep0);
   
  if (ret != USB_OK) {
      printk(KERN_ERR "\nUSB Service Registration failed. Error: %x", ret);
      
      goto autoconf_fail;
  } /* Endif */

  printk(KERN_INFO "\nUSB Service Registration BUS RESET OK\n");
   
  ret = _usb_device_register_service(handle, MOUSE_EP, wpcm450_service_ep1);
   
  if (ret != USB_OK) {
      printk(KERN_ERR "\nUSB Service Registration failed. Error: %x", ret);
      goto autoconf_fail;
  } /* Endif */

  ret = _usb_device_register_service(handle, KEYBOARD_EP, wpcm450_service_ep2);
   
  if (ret != USB_OK) {
      printk(KERN_ERR "\nUSB Service Registration failed. Error: %x", ret);
      goto autoconf_fail;
  } /* Endif */

  printk(KERN_INFO "\nUSB Service Registration EP1 OK\n");

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
    WPCM450_UDC_LONG_NAME, WPCM450_UDC_VERSION_STR);

  return 0;

autoconf_fail:
  printk(KERN_ERR "wpcm450_udc_bind: cannot autoconfigure on %s\n", gadget->name);
  return -ENODEV;
}


/*
 * wpcm450_udc_unbind
 *
 * Called on module unload.  Frees the control request and device
 * structure.
 */
static void  wpcm450_udc_unbind(struct usb_gadget *gadget)
{

  wpcm450_udc_t *dev = get_gadget_data(gadget);

  printk(KERN_INFO "wpcm450_udc_unbind: %s %s unbound\n", WPCM450_UDC_LONG_NAME,
         WPCM450_UDC_VERSION_STR);
}


/*
 * wpcm450_udc_setup
 *
 * Implements all the control endpoint functionality that's not
 * handled in hardware or the hardware driver.
 *
 * Returns the size of the data sent to the host, or a negative
 * error number.
 */
static int wpcm450_udc_setup(
                   struct usb_gadget *gadget,
                   const struct usb_ctrlrequest *ctrl)
{

  int ret = -EOPNOTSUPP;
#if 0  
  struct wpcm450_udc_dev *dev = get_gadget_data(gadget);
  struct usb_request *req = dev->dev_ctrl_req;
  u16 wIndex = le16_to_cpu(ctrl->wIndex);
  u16 wValue = le16_to_cpu(ctrl->wValue);
  u16 wLength = le16_to_cpu(ctrl->wLength);

  switch (ctrl->bRequestType & USB_TYPE_MASK) {
  case USB_TYPE_STANDARD:
    ret = wpcm450_udc_setup_standard(gadget,ctrl);
    break;

	case USB_TYPE_CLASS:
		ret = wpcm450_udc_setup_class(gadget,ctrl);
		break;

	default:
		printk(KERN_ERR "wpcm450_udc_setup: unknown request, type=%02x, request=%02x, value=%04x, index=%04x, length=%d\n",
			ctrl->bRequestType, ctrl->bRequest,
			wValue, wIndex, wLength);
		break;
	}

	/* respond with data transfer before status phase? */
	if (ret >= 0) {
		req->length = ret;
		req->zero = ret < wLength
				&& (ret % gadget->ep0->maxpacket) == 0;
		ret = usb_ep_queue(gadget->ep0, req, GFP_ATOMIC);
		if (ret < 0) {
			printk(KERN_ERR "wpcm450_udc_setup: cannot queue response, ret=%d\n",
				ret);
			req->status = 0;
			wpcm450_udc_setup_complete(gadget->ep0, req);
		}
	}
#endif

  /* device either stalls (ret < 0) or reports success */
  return ret;
}


static void wpcm450_udc_disconnect(struct usb_gadget *gadget)
{
}



/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : get_report
* Returned Value : 
* Comments       :
*     Chapter 9 Class specific request
*     See section 9.4.11 (page 195) of the USB 1.1 Specification.
* 
*END*--------------------------------------------------------------------*/

void get_report(
         _usb_device_handle handle,        /* USB handle */
         boolean     setup,                /* Is it a Setup phase? */
         __u8      direction,              /* [IN] Direction of the transfer. (1 for USB IN token)*/
         struct usb_ctrlrequest *setup_ptr /* The setup packet pointer */
         )
{ 
  int i;
      
  for(i=0;i<10;i++)	
  {
    hid_test_rep_data[i] = (__u8) i;
  }

  if (setup)
  {
    _usb_device_send_data(handle, 0, (__u8*)hid_test_rep_data, MIN(setup_ptr->wLength,4));
  }
         
   _usb_device_recv_data(handle, 0, 0, 0);
         

  return;
} 

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : set_report
* Returned Value : 
* Comments       :
*     Chapter 9 Class specific request
*     See section 9.4.11 (page 195) of the USB 1.1 Specification.
* 
*END*--------------------------------------------------------------------*/

void set_report(
         _usb_device_handle handle,        /* USB handle */
         boolean     setup,                /* Is it a Setup phase? */
         __u8      direction,              /* [IN] Direction of the transfer. (1 for USB IN token)*/
         struct usb_ctrlrequest *setup_ptr /* The setup packet pointer */
         )
{ 
  if (setup)      //on a SetUP packet
  {
    _usb_device_recv_data(handle, 0, (__u8*)hid_test_rep_data, MIN(setup_ptr->wLength,4));
  }
  else if(direction == USB_RECV)   //on a OUT packet
  {
    _usb_device_recv_data(handle, 0, (__u8*)hid_test_rep_data, MIN(setup_ptr->wLength,4));
    _usb_device_send_data(handle, 0, 0, 0);
  }

  return;
} 

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : set_idle
* Returned Value : 
* Comments       :
*     Chapter 9 Class specific request
*     See section 9.4.11 (page 195) of the USB 1.1 Specification.
* 
*END*--------------------------------------------------------------------*/

void set_idle(
         _usb_device_handle handle,        /* USB handle */
         boolean     setup,                /* Is it a Setup phase? */
         __u8      direction,              /* [IN] Direction of the transfer. (1 for USB IN token)*/
         struct usb_ctrlrequest *setup_ptr /* The setup packet pointer */
         )
{ 
  /*
   * SET_IDLE is a No data phase transaction from HID class. All it needs
   * is a terminating IN token 
   */
  if (setup)      //on a SetUP packet
  {
    _usb_device_send_data(handle, 0, 0, 0);  
  }
  return;
} 


/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : set_protocol
* Returned Value : 
* Comments       :
*     Chapter 9 Class specific request
*     See section 9.4.11 (page 195) of the USB 1.1 Specification.
* 
*END*--------------------------------------------------------------------*/

void set_protocol(
         _usb_device_handle handle,        /* USB handle */
         boolean     setup,                /* Is it a Setup phase? */
         __u8      direction,              /* [IN] Direction of the transfer. (1 for USB IN token)*/
         struct usb_ctrlrequest *setup_ptr /* The setup packet pointer */
         )
{ 
  /*
   * SET_IDLE is a No data phase transaction from HID class. All it needs
   * is a terminating IN token 
   */
  if (setup)      //on a SetUP packet
  {
    _usb_device_send_data(handle, 0, 0, 0);  
  }
  return;
} 

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : ch9Class
* Returned Value : 
* Comments       :
*     Chapter 9 Class specific request
*     See section 9.4.11 (page 195) of the USB 1.1 Specification.
* 
*END*--------------------------------------------------------------------*/
void ch9Class(
         _usb_device_handle handle,        /* USB handle */
         boolean     setup,                /* Is it a Setup phase? */
         __u8      direction,              /* [IN] Direction of the transfer. (1 for USB IN token)*/
         struct usb_ctrlrequest *setup_ptr /* The setup packet pointer */
         )
{ /* Body */
   
   switch (setup_ptr->bRequest) 
   {
   
      case 0x01:
         get_report(handle, setup, direction, setup_ptr);
         break;
      
      case 0x09:
         set_report(handle, setup, direction, setup_ptr);
         break;

      case 0x0A:
         set_idle(handle, setup, direction, setup_ptr);
         break;

      case 0x0B:
         set_protocol(handle, setup, direction, setup_ptr);
         break;
     
      default:
         _usb_device_stall_endpoint(handle, 0, 0);
         break;

   } /* EndSwitch */

} /* Endbody */

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : ch9GetStatus
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
*     static __u8_ptr pData;
*
*     See section 9.4.5 (page 190) of the USB 1.1 Specification.
* 
*END*--------------------------------------------------------------------*/
void ch9GetStatus  (
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
            _usb_device_send_data(handle, 0, (__u8*)&usb_status, 
               2);
            break;

         case 0x81:
            /* Interface request */
            if_status = usb_if_alt[setup_ptr->wIndex & 0x00FF];
            
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
* Function Name  : ch9ClearFeature
* Returned Value : None
* Comments       :
*     Chapter 9 ClearFeature command
*     See section 9.4.1 (page 188) of the USB 1.1 Specification.
* 
*END*--------------------------------------------------------------------*/
void ch9ClearFeature(
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
* Function Name  : ch9SetFeature
* Returned Value : None
* Comments       :
*     Chapter 9 SetFeature command
*     See section 9.4.9 (page 194) of the USB 1.1 Specification.
* 
*END*--------------------------------------------------------------------*/
void ch9SetFeature(
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
* Function Name  : ch9SetAddress
* Returned Value : None
* Comments       :
*     Chapter 9 SetAddress command
*     We setup a TX packet of 0 length ready for the IN token
*     Once we get the TOK_DNE interrupt for the IN token, then
*     we change the ADDR register and go to the ADDRESS state.
*     See section 9.4.6 (page 192) of the USB 1.1 Specification.
* 
*END*--------------------------------------------------------------------*/
void ch9SetAddress(
        _usb_device_handle handle,        /* USB handle */
        boolean setup,                    /* Is it a Setup phase? */
        struct usb_ctrlrequest *setup_ptr /* The setup packet pointer */
        )
{ /* Body */
   static __u8 new_address;

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
            
      _usb_device_init_endpoint(
           handle,
           KEYBOARD_EP,
           INTERRUPT_MAX_PACKET_SIZE, 
           USB_SEND,
           USB_INTERRUPT_ENDPOINT,
           USB_SEND);

      _usb_device_init_endpoint(
           handle,
           MOUSE_EP,
           INTERRUPT_MAX_PACKET_SIZE, 
           USB_SEND,
           USB_INTERRUPT_ENDPOINT,
           USB_SEND);
      
      TEST_ENABLED = TRUE;
     
   } /* Endif */
   return;
} /* Endbody */

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : ch9GetDescription
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
void ch9GetDescription(
       _usb_device_handle handle,        /* USB handle */
        boolean setup,                    /* Is it a Setup phase? */
        struct usb_ctrlrequest *setup_ptr /* The setup packet pointer */
        )
{ /* Body */
   
   __u32 buf_offset;
   
   if (setup) {
      /* Load the appropriate string depending on the descriptor requested.*/
      switch (setup_ptr->wValue & 0xFF00) {

         case 0x0100:

            _usb_device_send_data(
                 handle,
                 0, 
                 (__u8*)&wpcm450_usb11_device_desc,
                 MIN(setup_ptr->wLength, sizeof(wpcm450_usb11_device_desc)));
      
            break;

         case 0x0200:
        
            buf_offset = 0;



            wpcm450_udc_config_desc.wTotalLength =  sizeof(struct usb_config_descriptor) 
                     + sizeof(struct usb_interface_descriptor) + wpcm450_hid_keyboard_desc.bLength + wpcm450_keyboard_ep_desc.bLength
                     + sizeof(struct usb_interface_descriptor) + wpcm450_hid_mouse_desc.bLength + wpcm450_mouse_ep_desc.bLength;
            
            memcpy(config_desc_buffer, &wpcm450_udc_config_desc, sizeof(struct usb_config_descriptor));
            buf_offset += sizeof(struct usb_config_descriptor);

            
            memcpy((config_desc_buffer+buf_offset), &wpcm450_mouse_intf_desc, sizeof(struct usb_interface_descriptor));
            buf_offset += sizeof(struct usb_interface_descriptor);
            memcpy((config_desc_buffer+buf_offset), &wpcm450_hid_mouse_desc, wpcm450_hid_mouse_desc.bLength);
            buf_offset += wpcm450_hid_mouse_desc.bLength;
            memcpy((config_desc_buffer+buf_offset), &wpcm450_mouse_ep_desc, wpcm450_mouse_ep_desc.bLength);
            buf_offset += wpcm450_mouse_ep_desc.bLength;

            memcpy((config_desc_buffer+buf_offset), &wpcm450_keyboard_intf_desc, sizeof(struct usb_interface_descriptor));
            buf_offset += sizeof(struct usb_interface_descriptor);
            memcpy((config_desc_buffer+buf_offset), &wpcm450_hid_keyboard_desc, wpcm450_hid_keyboard_desc.bLength);
            buf_offset += wpcm450_hid_keyboard_desc.bLength;
            memcpy((config_desc_buffer+buf_offset), &wpcm450_keyboard_ep_desc, wpcm450_keyboard_ep_desc.bLength);
            buf_offset += wpcm450_keyboard_ep_desc.bLength;
            
            
            
           
              
            _usb_device_send_data(handle, 0, (__u8*)config_desc_buffer,
               MIN(setup_ptr->wLength, buf_offset));
            
                        
            break;

         case 0x2200:

           switch (setup_ptr->wIndex) 
           {
           case WPCM450_MOUSE_INTERFACE_NUM :
             _usb_device_send_data(handle, 0, hid_report_mouse,  sizeof(hid_report_mouse));
             break;
           case WPCM450_KEYBOARD_INTERFACE_NUM :
             _usb_device_send_data(handle, 0, hid_report_keyboard,  sizeof(hid_report_keyboard));
             break;
           default:
             break;

           }
           
           
            break;      
   
         case 0x0300:
          
            if ((setup_ptr->wValue & 0x00FF) > USB_STR_NUM) {
               _usb_device_send_data(handle, 0, USB_STRING_DESC[USB_STR_NUM+1],
                  MIN(setup_ptr->wLength, USB_STRING_DESC[USB_STR_NUM+1][0]));
            } else {
               _usb_device_send_data(handle, 0,
                  USB_STRING_DESC[setup_ptr->wValue & 0x00FF],
                  MIN(setup_ptr->wLength,
                     USB_STRING_DESC[setup_ptr->wValue & 0x00FF][0]));
            } /* Endif */      
            break;

         case 0x600:
            _usb_device_send_data(handle, 0, (__u8*)&wpcm450_usb11_qualifier_desc, 
               MIN(setup_ptr->wLength, DEVICE_QUALIFIER_DESCRIPTOR_SIZE));
            break;
#if 0            
         case 0x700:      
            _usb_device_send_data(handle, 0, (__u8*)other_speed_config, 
               MIN(setup_ptr->wLength, OTHER_SPEED_CONFIG_DESC_SIZE));
            break;
#endif

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
* Function Name  : ch9SetDescription
* Returned Value : None
* Comments       :
*     Chapter 9 SetDescription command
*     See section 9.4.8 (page 193) of the USB 1.1 Specification.
* 
*END*--------------------------------------------------------------------*/
void ch9SetDescription(
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
* Function Name  : ch9GetConfig
* Returned Value : None
* Comments       :
*     Chapter 9 GetConfig command
*     See section 9.4.2 (page 189) of the USB 1.1 Specification.
* 
*END*--------------------------------------------------------------------*/
void ch9GetConfig(
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
* Function Name  : ch9SetConfig
* Returned Value : None
* Comments       :
*     Chapter 9 SetConfig command
* 
*END*--------------------------------------------------------------------*/
void ch9SetConfig(
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
* Function Name  : ch9GetInterface
* Returned Value : None
* Comments       :
*     Chapter 9 GetInterface command
*     See section 9.4.4 (page 190) of the USB 1.1 Specification.
* 
*END*--------------------------------------------------------------------*/
void ch9GetInterface(
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
      _usb_device_send_data(handle, 0, &usb_if_alt[setup_ptr->wIndex & 0x00FF],
         MIN(setup_ptr->wLength, sizeof(__u8)));
      /* status phase */      
      _usb_device_recv_data(handle, 0, 0, 0);
   } /* Endif */
   return;
} /* Endbody */

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : ch9SetInterface
* Returned Value : None
* Comments       :
*     Chapter 9 SetInterface command
*     See section 9.4.10 (page 195) of the USB 1.1 Specification.
* 
*END*--------------------------------------------------------------------*/
void ch9SetInterface(
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
      if (usb_if_alt[setup_ptr->wIndex & 0x00FF]
         != (setup_ptr->wValue & 0x00FF))
      {
         usb_if_alt[setup_ptr->wIndex & 0x00FF] = (setup_ptr->wValue & 0x00FF);
         /* Reconfigure endpoints here. */
         
      } /* Endif */

      /* status phase */
      _usb_device_send_data(handle, 0, 0, 0);
   } /* Endif */
   return;
} /* Endbody */

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : ch9SynchFrame
* Returned Value : 
* Comments       :
*     Chapter 9 SynchFrame command
*     See section 9.4.11 (page 195) of the USB 1.1 Specification.
* 
*END*--------------------------------------------------------------------*/
void ch9SynchFrame(
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

      if ((setup_ptr->wIndex & 0x00FF) >= WPCM450_UDC_MAX_NUM_INTERFACES)
         /*ConfigDesc[CONFIG_DESC_NUM_INTERFACES])*/
      {
         _usb_device_stall_endpoint(handle, 0, 0);
         return;
      } /* Endif */

      _usb_device_get_status(handle, USB_STATUS_SOF_COUNT, &sof_count);
      _usb_device_send_data(handle, 0, (__u8*)&sof_count,
         MIN(setup_ptr->wLength, sizeof(sof_count)));
      /* status phase */      
      _usb_device_recv_data(handle, 0, 0, 0);
   } /* Endif */
   return;
} /* Endbody */


/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_service_ep0
* Returned Value : None
* Comments       :
*     Called upon a completed endpoint 0 (USB 1.1 Chapter 9) transfer
* 
*END*--------------------------------------------------------------------*/
void wpcm450_service_ep0
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

   int index;
   for (index = 0; index < 8; index++)
	   printk ("0x%02x ", *(unsigned char *)(&local_setup_packet + index));
   printk ("\n");
   
   switch (local_setup_packet.bRequestType & 0x60) {

      case 0x00:
         switch (local_setup_packet.bRequest) {

            case 0x0:
               ch9GetStatus(handle, setup, &local_setup_packet);
               break;

            case 0x1:
               ch9ClearFeature(handle, setup, &local_setup_packet);
               break;

            case 0x3:
               ch9SetFeature(handle, setup, &local_setup_packet);
               break;

            case 0x5:
               ch9SetAddress(handle, setup, &local_setup_packet);
               break;

            case 0x6:
               ch9GetDescription(handle, setup, &local_setup_packet);
               break;

            case 0x7:
               ch9SetDescription(handle, setup, &local_setup_packet);
               break;

            case 0x8:
               ch9GetConfig(handle, setup, &local_setup_packet);
               break;

            case 0x9:
               ch9SetConfig(handle, setup, &local_setup_packet);
               break;

            case 0xa:
               ch9GetInterface(handle, setup, &local_setup_packet);
               break;

            case 0xb:
               ch9SetInterface(handle, setup, &local_setup_packet);
               break;

            case 0xc:
               ch9SynchFrame(handle, setup, &local_setup_packet);
               break;

            default:
               _usb_device_stall_endpoint(handle, 0, 0);
               break;

         } /* Endswitch */
         
         break;

      case 0x20:
         ch9Class(handle, setup, direction, &local_setup_packet);

         break;

      case 0x40:
         /* vendor specific request */
         break;
      
      default:
         _usb_device_stall_endpoint(handle, 0, 0);
         break;
         
   } /* Endswitch */
   
   return;
} /* Endbody */

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_service_ep1
* Returned Value : None
* Comments       :
*     Called upon a completed endpoint 1 (USB 1.1 Chapter 9) transfer
* 
*END*--------------------------------------------------------------------*/
void wpcm450_service_ep1
   (
      _usb_device_handle   handle,    /* [IN] Handle of the USB device                    */
      boolean              setup,     /* [IN] Is it a setup packet?                       */
      __u8                 direction, /* [IN] Direction of the transfer.  Is it transmit? */
      __u8*                buffer,    /* [IN] Pointer to the data buffer                  */
      __u32                length,    /* [IN] Length of the transfer                      */
      __u8                 error      /* [IN] Error, if any                               */
   )
{ /* Body */

 /*****************************************************/
 /* Interrupt fee mutex for allow next transaction    */
 /*****************************************************/

 udelay(10);


#ifdef _USB_WRITE_MUTEX_USED_
 up(&mouse_data_mutex);
#endif
 
 return;
} /* Endbody */



/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_service_ep2
* Returned Value : None
* Comments       :
*     Called upon a completed endpoint 2 (USB 1.1 Chapter 9) transfer
* 
*END*--------------------------------------------------------------------*/
static void wpcm450_service_ep2 (
             _usb_device_handle   handle,    /* [IN] Handle of the USB device                    */
             boolean              setup,     /* [IN] Is it a setup packet?                       */
             __u8                 direction, /* [IN] Direction of the transfer.  Is it transmit? */
             __u8*                buffer,    /* [IN] Pointer to the data buffer                  */
             __u32                length,    /* [IN] Length of the transfer                      */
             __u8                 error      /* [IN] Error, if any                               */
             )
{ /* Body */

 /*****************************************************/
 /* Interrupt fee mutex for allow next transaction    */
 /*****************************************************/

 udelay(10);
 
#ifdef _USB_WRITE_MUTEX_USED_ 
 up(&keyboard_data_mutex);
#endif

 return;
} /* Endbody */             


/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_reset_ep0
* Returned Value : None
* Comments       :
*     Called upon a bus reset event.  Initialises the control endpoint.
* 
*END*--------------------------------------------------------------------*/
void wpcm450_reset_ep0 (
             _usb_device_handle   handle,    /* [IN] Handle of the USB device                    */
             boolean              setup,     /* [IN] Is it a setup packet?                       */
             __u8                 direction, /* [IN] Direction of the transfer.  Is it transmit? */
             __u8*                buffer,    /* [IN] Pointer to the data buffer                  */
             __u32                length,    /* [IN] Length of the transfer                      */
             __u8                 error      /* [IN] Error, if any                               */
             )
{ /* Body */

  volatile USB_DEV_STATE_STRUCT_PTR usb_dev_ptr;
  
   /* on a reset always ensure all transfers are cancelled on control EP*/
   _usb_device_cancel_transfer(handle, 0, USB_RECV);
   _usb_device_cancel_transfer(handle, 0, USB_SEND);

   _usb_device_cancel_transfer(handle, MOUSE_EP, USB_RECV);
   _usb_device_cancel_transfer(handle, MOUSE_EP, USB_SEND);
      
   _usb_device_cancel_transfer(handle, KEYBOARD_EP, USB_RECV);
   _usb_device_cancel_transfer(handle, KEYBOARD_EP , USB_SEND);


   if (TEST_ENABLED) {
   /* Initialize the endpoint 0 in both directions */
   _usb_device_init_endpoint(handle, 0, wpcm450_usb11_device_desc.bMaxPacketSize0, 
      USB_RECV, USB_CONTROL_ENDPOINT, 0);
   _usb_device_init_endpoint(handle, 0, wpcm450_usb11_device_desc.bMaxPacketSize0, 
      USB_SEND, USB_CONTROL_ENDPOINT, 0);

   }

 #ifdef _USB_WRITE_MUTEX_USED_
   

  usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;
  
  if(usb_dev_ptr->USB_STATE != USB_STATE_CONFIG)  
  {
    down_trylock(&mouse_data_mutex);
    up(&mouse_data_mutex);
    down_trylock(&keyboard_data_mutex);
    up(&keyboard_data_mutex);
    //if(atomic_read(&mouse_data_mutex.count) != 1)
    //{
    //  atomic_inc(&mouse_data_mutex.count);
    //}

    //if(atomic_read(&keyboard_data_mutex.count) != 1)
    //{
    //  atomic_inc(&keyboard_data_mutex.count);
    //}
    
   }
   
#endif 
   
   TEST_ENABLED = FALSE;
   
   return;
} /* EndBody */


/******************************************************************************/
/******************************************************************************/


/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_usb_read
* Returned Value : 
* Comments       :
*     
* 
*END*--------------------------------------------------------------------*/
ssize_t wpcm450_usb_read(
                struct file *filp,
                char __user *buf,
                size_t count,
                loff_t *f_pos
                )
{
  struct scull_dev *dev = filp->private_data; 

  return 0;
}

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_usb_write
* Returned Value : 
* Comments       :
*     
* 
*END*--------------------------------------------------------------------*/
ssize_t wpcm450_usb_write(
                struct file *filp,
                const char __user *buf,
                size_t count,
                loff_t *f_pos
                )
{
  wpcm450_dev_t *dev = filp->private_data;
  int ep_num;
  volatile USB_DEV_STATE_STRUCT_PTR usb_dev_ptr;

  /*****************************************************/
  /* Try to lock mutex that will be free into interrupt*/
  /* handler                                           */
  /*****************************************************/

  usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)dev->handle;
  
  if(usb_dev_ptr->USB_STATE != USB_STATE_CONFIG)  
  {
    return (-1);
  }
  
  switch(MINOR (dev->cdev.dev))
  {
  case 0:

#ifdef _USB_WRITE_MUTEX_USED_
    if(down_trylock(&mouse_data_mutex) != 0)
    {
      return (-1);
    }
#endif
    
    ep_num = MOUSE_EP;
    
    break;
    
  case 1:

#ifdef _USB_WRITE_MUTEX_USED_
    if(down_trylock(&keyboard_data_mutex) != 0)
    {
      return (-1);
    }
#endif
    
    ep_num = KEYBOARD_EP;
    
    break;
  default:
    
    return -1;
  }


 
  copy_from_user((__u8*)((void*)&data), buf, count);

  
  
  if (_usb_device_send_data(
           dev->handle,
          ep_num, 
          (__u8*)((void*)&data),
          count) != USB_OK)
  {
    
    return (-1);
  }

  

    
  return 0;
}

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_usb_ioctl
* Returned Value : 
* Comments       :
*     
* 
*END*--------------------------------------------------------------------*/
int wpcm450_usb_ioctl(
            struct inode *inode,
            struct file *filp,
            unsigned int cmd,
            unsigned long arg
            )
{

  int err = 0, tmp;
  int retval = 0;


 #if 0   
  /*
   * extract the type and number bitfields, and don't decode
   * wrong cmds: return ENOTTY (inappropriate ioctl) before access_ok()
   */
  if (_IOC_TYPE(cmd) != SCULL_IOC_MAGIC) return -ENOTTY;
  if (_IOC_NR(cmd) > SCULL_IOC_MAXNR) return -ENOTTY;

  /*
   * the direction is a bitmask, and VERIFY_WRITE catches R/W
   * transfers. `Type' is user-oriented, while
   * access_ok is kernel-oriented, so the concept of "read" and
   * "write" is reversed
   */
  if (_IOC_DIR(cmd) & _IOC_READ) {
    err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
  } else if (_IOC_DIR(cmd) & _IOC_WRITE) {
    err =  !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
  }
  if (err) return -EFAULT;
#endif


  return 0;
}

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_usb_open
* Returned Value : 
* Comments       :
*     
* 
*END*--------------------------------------------------------------------*/

int wpcm450_usb_open(
            struct inode *inode,
            struct file *filp
            )
{
  wpcm450_dev_t *dev; /* device information */
  int result;
   
  dev = container_of(inode->i_cdev, wpcm450_dev_t, cdev);
  filp->private_data = dev;  /* for other methods */

  dev->handle = wpcm450_udc_dev.handler;
  
  return 0;          /* success */
}

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_usb_close
* Returned Value : 
* Comments       :
*     
* 
*END*--------------------------------------------------------------------*/
int wpcm450_usb_close(
            struct inode *inode,
            struct file *filp
            )
{
  return 0;
}


struct file_operations wpcm450_usb_fops = {
    .owner =   THIS_MODULE,
    .llseek =  NULL,
    .read =    wpcm450_usb_read,
    .write =   wpcm450_usb_write,
    .ioctl =   wpcm450_usb_ioctl,
    .open =    wpcm450_usb_open,
    .release = wpcm450_usb_close,
      
    
  };

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_cleanup_module
* Returned Value : 
* Comments       :
*     The cleanup function is used to handle initialization failures as well.
*     Thefore, it must be careful to work correctly even if some of the items
*     have not been initialized     
* 
*END*--------------------------------------------------------------------*/
void wpcm450_cleanup_module(void)
{
  int i;
  dev_t devno = MKDEV(wpcm450_major, wpcm450_minor);

  /* Get rid of our char dev entries */
  if (wpcm450_devices) {
    for (i = 0; i < wpcm450_nr_devs; i++) {
      cdev_del(&wpcm450_devices[i].cdev);
    }

    kfree(wpcm450_devices);
  }

  /* cleanup_module is never called if registering failed */
  unregister_chrdev_region(devno, wpcm450_nr_devs);

  /* and call the cleanup functions for friend devices */
  

}

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_setup_cdev
* Returned Value : 
* Comments       :
*     Set up the char_dev structure for this device.
* 
*END*--------------------------------------------------------------------*/
static void wpcm450_setup_cdev(
                    wpcm450_dev_t *dev,
                    int index)
{
  int err, devno = MKDEV(wpcm450_major, wpcm450_minor + index);
    
  cdev_init(&dev->cdev, &wpcm450_usb_fops);
  dev->cdev.owner = THIS_MODULE;
  dev->cdev.ops = &wpcm450_usb_fops;
  dev->handle =  wpcm450_udc_dev.handler;
   
  err = cdev_add (&dev->cdev, devno, 1);
  /* Fail gracefully if need be */
  if (err)
    printk(KERN_NOTICE "Error %d adding scull%d", err, index);
}

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : wpcm450_init_module
* Returned Value : 
* Comments       :
*    
* 
*END*--------------------------------------------------------------------*/
int wpcm450_init_module(void)
{
  int result, i;
  dev_t dev = 0;

  g_init_cnt++;

#ifdef _USB_WRITE_MUTEX_USED_  
  init_MUTEX(&keyboard_data_mutex);
  init_MUTEX(&mouse_data_mutex);
#endif
    
  /*
   * Get a range of minor numbers to work with, asking for a dynamic
   * major unless directed otherwise at load time.
   */
  if (wpcm450_major) 
  {

    dev = MKDEV(wpcm450_major, wpcm450_minor);
    result = register_chrdev_region(dev, wpcm450_nr_devs, "gkbd");
    
  } 
  else
  {

    result = alloc_chrdev_region(
                   &dev,
                   wpcm450_minor,
                   wpcm450_nr_devs,
                   "scull");
    
    wpcm450_major = MAJOR(dev);
  }

  if (result < 0)
  {
    printk(KERN_WARNING "gkbd: can't get major %d\n", wpcm450_major);
    return result;
  }

  /* 
   * allocate the devices -- we can't have them static, as the number
   * can be specified at load time
   */
  wpcm450_devices = kmalloc(wpcm450_nr_devs * sizeof(wpcm450_dev_t), GFP_KERNEL);
  if (!wpcm450_devices)
  {
    result = -ENOMEM;
    goto fail;  /* Make this more graceful */
  }
  
  memset(wpcm450_devices, 0, wpcm450_nr_devs * sizeof(wpcm450_dev_t));

  /* Initialize each device. */
  for (i = 0; i < wpcm450_nr_devs; i++) 
  {
    wpcm450_setup_cdev(&wpcm450_devices[i], i);
  }

  /* usb board init */
  result = usb_gadget_register_driver(&wpcm450_udc_gadget_driver);
  if (result) {
    printk(KERN_ERR "wpcm450_init_module: cannot register gadget driver, ret=%d\n", result);
    return result;
  }
  
  printk(KERN_INFO "gkbd: wpcm450_init_module register dev %d:%d\n", wpcm450_major, wpcm450_minor);
  
  return 0; /* succeed */

fail:
  wpcm450_cleanup_module();
  return result;
}

module_init(wpcm450_init_module);
module_exit(wpcm450_cleanup_module);


