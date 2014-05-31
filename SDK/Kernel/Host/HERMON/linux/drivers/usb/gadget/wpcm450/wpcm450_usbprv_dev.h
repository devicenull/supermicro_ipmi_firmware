#ifndef __usbprv_dev_h__
#define __usbprv_dev_h__ 1
/*******************************************************************************
** File          : $HeadURL$ 
** Author        : $Author: achechik $
** Project       : HSCTRL 
** Instances     : 
** Creation date : 
********************************************************************************
********************************************************************************
** ChipIdea Microelectronica - IPCS
** TECMAIA, Rua Eng. Frederico Ulrich, n 2650
** 4470-920 MOREIRA MAIA
** Portugal
** Tel: +351 229471010
** Fax: +351 229471011
** e_mail: chipidea.com
********************************************************************************
** ISO 9001:2000 - Certified Company
** (C) 2005 Copyright Chipidea(R)
** Chipidea(R) - Microelectronica, S.A. reserves the right to make changes to
** the information contained herein without notice. No liability shall be
** incurred as a result of its use or application.
********************************************************************************
** Modification history:
** $Date: 2008/02/11 11:47:33Z $
** $Revision: 1.2 $
*******************************************************************************
*** Description:      
***  This file contains the private defines, externs and
***  data structure definitions required by the VUSB_HS Device
***  driver.
***                                                               
**************************************************************************
**END*********************************************************/

#include "wpcm450_vusbhs.h"
//#include "wpcm450_usbprv.h"

//#define _DEVICE_DEBUG_ 1

#define  MAX_EP_TR_DESCRS                    (32)
#define  MAX_XDS_FOR_TR_CALLS                (32)
#define  MAX_USB_DEVICES                     (1)
#define  USB_MAX_ENDPOINTS                   (3)

/*Assumption here is that all control endpoints are sequential 0,1,..
if they are not you need to modify the tr_complete routine to handle that */
#define  USB_MAX_CONTROL_ENDPOINTS           (1)

#define  USB_MAX_CTRL_PAYLOAD                (64)
 
/* Macro for aligning the EP queue head to 32 byte boundary */
#define USB_MEM32_ALIGN(n)                   ((n) + (-(n) & 31))

/* Macro for aligning the EP queue head to 1024 byte boundary */
#define USB_MEM1024_ALIGN(n)                 ((n) + (-(n) & 1023))

/* Macro for aligning the EP queue head to 1024 byte boundary */
#define USB_MEM2048_ALIGN(n)                 ((n) + (-(n) & 2047))

#if 0

#define USB_XD_QADD(head,tail,XD)      \
   if ((head) == NULL) {         \
      (head) = (XD);            \
   } else {                      \
      (tail)->wScratchPtr->pPrivate = (XD);   \
   } /* Endif */                 \
   (tail) = (XD);               \
   (XD)->wScratchPtr->pPrivate = NULL
   
#define USB_XD_QGET(head,tail,XD)      \
   (XD) = (head);               \
   if (head) {                   \
      (head) = (xd_struct_ptr_t)((head)->wScratchPtr->pPrivate);  \
      if ((head) == NULL) {      \
         (tail) = NULL;          \
      } /* Endif */              \
   } /* Endif */

#define EHCI_DTD_QADD(head,tail,dTD)      \
   if ((head) == NULL) {         \
      (head) = (dTD);            \
   } else {                      \
      (tail)->wScratchPtr->pPrivate = (void *) (dTD);   \
   } /* Endif */                 \
   (tail) = (dTD);               \
   (dTD)->wScratchPtr->pPrivate = NULL
   
#define EHCI_DTD_QGET(head,tail,dTD)      \
   (dTD) = (head);               \
   if (head) {                   \
      (head) = (head)->wScratchPtr->pPrivate;  \
      if ((head) == NULL) {      \
         (tail) = NULL;          \
      } /* Endif */              \
   } /* Endif */


#endif

/*
#define EHCI_DTD_QGET(head,tail,dTD)      \
   (dTD) = (head);               \
   if (head) {                   \
      (head) = (EpTrStructPtr_t)((head)->wScratchPtr->pPrivate);  \
      if ((head) == NULL) {      \
         (tail) = NULL;          \
      }               \
   }
*/

/******************************************************************************
**
** Data structures
**
*******************************************************************************/

/* Callback function storage structure */

typedef struct service_struct {
   __u8                   type;
   void                   (* service)(void*, boolean, __u8, __u8*, __u32, __u8);
   struct service_struct *next;
} service_struct_t, * service_struct_ptr_t;


typedef struct xd_struct {
   __u8               bEpNum;              /* Endpoint number */
   __u8               bDirection;          /* Direction : Send/Receive */
   __u8               bEpType;             /* Type of the endpoint: Ctrl, Isoch, Bulk, Int */
   __u8               bStatus;             /* Current transfer status */
   __u8*              wStartaddress;       /* Address of first byte */
   __u32              wToatalLength;       /* Number of bytes to send/recv */
   __u32              wSoFar;              /* Number of bytes recv'd so far */
   __u16              wMaxpacketSize;      /* Max Packet size */
   boolean            bDontZeroTerminate;
   __u8               bMaxPktsPerUframe;
   ScratchStructPtr_t wScratchPtr;
} xd_struct_t;

typedef xd_struct_t* xd_struct_ptr_t;

/* The USB Device State Structure */
typedef struct {
   boolean                          BUS_RESETTING;       /* Device is being reset */
   boolean                          TRANSFER_PENDING;    /* Transfer pending ? */
   void*                            CALLBACK_STRUCT_PTR;
   service_struct_ptr_t             SERVICE_HEAD_PTR;    /* Head struct address of registered services */
   xd_struct_ptr_t                  TEMP_XD_PTR;       /* Temp xd for ep init */
   xd_struct_ptr_t                  XD_BASE;
   xd_struct_ptr_t                  XD_HEAD;             /* Head Transaction descriptors */
   xd_struct_ptr_t                  XD_TAIL;             /* Tail Transaction descriptors */
   xd_struct_ptr_t                  PENDING_XD_PTR;      /* pending transfer */
   __u32                            XD_ENTRIES;
   EpQueueHeadStructPtr_t  EP_QUEUE_HEAD_PTR;   /* Endpoint Queue head */
   __u8*                            DRIVER_MEMORY;
   EpQueueHeadStructPtr_t  EP_QUEUE_HEAD_BASE;
   EpTrStructPtr_t          DTD_BASE_PTR;        /* Device transfer descriptor pool address */
   EpTrStructPtr_t          DTD_ALIGNED_BASE_PTR; /* Aligned transfer descriptor pool address */
   EpTrStructPtr_t          DTD_HEAD;
   EpTrStructPtr_t          DTD_TAIL;
   EpTrStructPtr_t          EP_DTD_HEADS[USB_MAX_ENDPOINTS * 2];
   EpTrStructPtr_t          EP_DTD_TAILS[USB_MAX_ENDPOINTS * 2];
   ScratchStructPtr_t               XD_SCRATCH_STRUCT_BASE;
   ScratchStructPtr_t               SCRATCH_STRUCT_BASE;
   
   /* These fields are kept only for USB_shutdown() */
   
   void                             (*OLDISR_PTR)(void*);
   void*                            OLDISR_DATA;
   __u16                            USB_STATE;
   __u16                            USB_DEVICE_STATE;
   __u16                            USB_SOF_COUNT;
   __u16                            DTD_ENTRIES;
   __u16                            ERRORS;
   __u16                            USB_DEV_STATE_B4_SUSPEND;
   __u8                             DEV_NUM;             /* USB device number on the board */
   __u8                             DEV_VEC;             /* Interrupt vector number for USB */
   __u8                             SPEED;               /* Low Speed, High Speed, Full Speed */
   __u8                             MAX_ENDPOINTS;       /* Max endpoints supported by this device */
   __u8                             USB_CURR_CONFIG;                                                         
   __u8                             DEVICE_ADDRESS;
} USB_DEV_STATE_STRUCT ;

typedef USB_DEV_STATE_STRUCT* USB_DEV_STATE_STRUCT_PTR;


/******************************************************************************
**
** Prototypes
**
*******************************************************************************/

#define DEVICE_INTERRUPT_ROUTINE_KEYWORD irqreturn_t


extern __u8 _usb_dci_init(__u8, _usb_device_handle);
extern void _usb_device_free_XD(void*);
extern void _usb_dci_vusb20_free_dTD(void*);
extern __u8 _usb_dci_vusb20_add_dTD(_usb_device_handle, xd_struct_ptr_t);
extern __u8 _usb_dci_vusb20_send_data(_usb_device_handle, xd_struct_ptr_t);
extern __u8 _usb_dci_vusb20_recv_data(_usb_device_handle, xd_struct_ptr_t);
extern __u8 _usb_dci_vusb20_cancel_transfer(_usb_device_handle, __u8, __u8);
extern __u8 _usb_dci_vusb20_get_transfer_status(_usb_device_handle, __u8, __u8);
extern xd_struct_ptr_t _usb_dci_vusb20_get_transfer_details(_usb_device_handle, __u8, __u8);
extern __u8 _bsp_get_usb_vector(__u8);
extern void* _bsp_get_usb_base(__u8);
extern void* _bsp_get_usb_capability_register_base(__u8);
extern void _usb_dci_vusb20_process_tr_complete(_usb_device_handle);
extern void _usb_dci_vusb20_process_reset(_usb_device_handle);
extern void _usb_dci_vusb20_process_tr_complete(_usb_device_handle);
extern void _usb_dci_vusb20_process_suspend(_usb_device_handle);
extern void _usb_dci_vusb20_process_SOF(_usb_device_handle);
extern void _usb_dci_vusb20_process_port_change(_usb_device_handle);
extern void _usb_dci_vusb20_process_error(_usb_device_handle);
extern void _usb_dci_vusb20_shutdown(_usb_device_handle);
extern void _usb_dci_vusb20_set_speed_full(_usb_device_handle, __u8);
extern void _usb_dci_vusb20_suspend_phy(_usb_device_handle, __u8);
extern void _usb_dci_vusb20_set_address(_usb_device_handle, __u8);
extern void _usb_dci_vusb20_get_setup_data(_usb_device_handle, __u8, __u8*);
extern void _usb_dci_vusb20_assert_resume(_usb_device_handle);
extern __u8 _usb_dci_vusb20_init_endpoint(_usb_device_handle, xd_struct_ptr_t);
extern void _usb_dci_vusb20_stall_endpoint(_usb_device_handle, __u8, __u8);
extern void _usb_dci_vusb20_unstall_endpoint(_usb_device_handle, __u8, __u8);
extern __u8 _usb_dci_vusb20_deinit_endpoint(_usb_device_handle, __u8, __u8);
extern void _usb_dci_vusb20_set_endpoint_status(_usb_device_handle, __u8,  __u8);
extern void _usb_dci_vusb20_set_test_mode(_usb_device_handle, __u16);   
extern __u8  _usb_dci_vusb20_get_endpoint_status(_usb_device_handle, __u8);
extern void _usb_dci_vusb20_reset_data_toggle(_usb_device_handle, __u8, __u8);
extern void _usb_dci_vusb20_chip_initialize(_usb_device_handle);
extern void _usb_dci_vusb20_chip_run(_usb_device_handle);

#endif
