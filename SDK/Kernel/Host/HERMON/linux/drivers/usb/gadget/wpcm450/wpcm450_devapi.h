#ifndef __devapi_h__
#define __devapi_h__ 1
/*******************************************************************************
** File          : $HeadURL$ 
** Author        : $Author: afishman $
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
** $Date: 2008/01/28 15:36:31Z $
** $Revision: 1.1 $
*******************************************************************************
*** Description:
***  This file contains the declarations specific to the USB Device API
***
**************************************************************************
**END*********************************************************/
#include <linux/types.h>
#include "wpcm450_usb.h"
#include "wpcm450_usbprv.h"
//#include "debug.h"

#define  USB_SEND                         (1)
#define  USB_RECV                         (0)

/* Endpoint types */
#define  USB_CONTROL_ENDPOINT             (0)
#define  USB_ISOCHRONOUS_ENDPOINT         (1)
#define  USB_BULK_ENDPOINT                (2)
#define  USB_INTERRUPT_ENDPOINT           (3)

/* Informational Request/Set Types */
#define  USB_STATUS_DEVICE_STATE          (0x01)
#define  USB_STATUS_INTERFACE             (0x02)
#define  USB_STATUS_ADDRESS               (0x03)
#define  USB_STATUS_CURRENT_CONFIG        (0x04)
#define  USB_STATUS_SOF_COUNT             (0x05)
#define  USB_STATUS_DEVICE                (0x06)
#define  USB_STATUS_TEST_MODE             (0x07)
#define  USB_FORCE_FULL_SPEED             (0x08)
#define  USB_PHY_LOW_POWER_SUSPEND        (0x09)
#define  USB_STATUS_ENDPOINT              (0x10)
#define  USB_STATUS_ENDPOINT_NUMBER_MASK  (0x8F)

#define  USB_TEST_MODE_TEST_PACKET        (0x0400)

/* Available service types */
/* Services 0 through 15 are reserved for endpoints */
#define  USB_SERVICE_EP0                  (0x00)
#define  USB_SERVICE_EP1                  (0x01)
#define  USB_SERVICE_EP2                  (0x02)
#define  USB_SERVICE_EP3                  (0x03)
#define  USB_SERVICE_BUS_RESET            (0x10)
#define  USB_SERVICE_SUSPEND              (0x11)
#define  USB_SERVICE_SOF                  (0x12)
#define  USB_SERVICE_RESUME               (0x13)
#define  USB_SERVICE_SLEEP                (0x14)
#define  USB_SERVICE_SPEED_DETECTION      (0x15)
#define  USB_SERVICE_ERROR                (0x16)
#define  USB_SERVICE_STALL                (0x17)

typedef void* _usb_device_handle;

typedef __u8 boolean;



extern __u8 _usb_device_init(__u8, _usb_device_handle*, __u8);
extern __u8 _usb_device_recv_data(_usb_device_handle, __u8, __u8*,  __u32);
extern __u8 _usb_device_send_data(_usb_device_handle, __u8, __u8*, __u32);
extern __u8 _usb_device_get_transfer_status(_usb_device_handle, __u8, __u8);
extern void _usb_device_get_transfer_details(_usb_device_handle, __u8, __u8, __u32**);
extern __u8 _usb_device_cancel_transfer(_usb_device_handle, __u8, __u8);
extern __u8 _usb_device_get_status(_usb_device_handle, __u8, __u16*);
extern __u8 _usb_device_set_status(_usb_device_handle, __u8, __u16);
extern __u8 _usb_device_register_service(_usb_device_handle, __u8, void(* service)(void*, boolean, __u8, __u8*, __u32, __u8));
extern __u8 _usb_device_unregister_service(_usb_device_handle, __u8);
extern __u8 _usb_device_call_service(_usb_device_handle, __u8, boolean, boolean, __u8*, __u32, __u8);
extern void _usb_device_shutdown(_usb_device_handle);
extern void _usb_device_set_address(_usb_device_handle, __u8);
extern void _usb_device_read_setup_data(_usb_device_handle, __u8, __u8*);
extern void _usb_device_assert_resume(_usb_device_handle);
extern __u8 _usb_device_init_endpoint(_usb_device_handle, __u8, __u16,  __u8, __u8, __u8);
extern void _usb_device_stall_endpoint(_usb_device_handle,  __u8, __u8);
extern void _usb_device_unstall_endpoint(_usb_device_handle,  __u8, __u8);
extern __u8 _usb_device_deinit_endpoint(_usb_device_handle, __u8, __u8);


#endif
/* EOF */

