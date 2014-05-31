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
** $Date: 2008/02/11 12:36:33Z $
** $Revision: 1.4 $
*******************************************************************************
*** Description:      
***  This file contains the main USB device API functions that will be 
***  used by most applications.
***                                                               
**************************************************************************
**END*********************************************************/

#include <asm/cacheflush.h>
#include <asm/arch/regs-irq.h>
#include <asm/arch/regs-udc.h>
#include "wpcm450_devapi.h"
#include "wpcm450_usbprv_dev.h"
#include "wpcm450_usbprv.h"
#include <linux/spinlock.h>

__u8 g_usb_init_done = FALSE;
spinlock_t usb_spin_lock;


void USB_XD_QADD(xd_struct_ptr_t head, xd_struct_ptr_t tail, xd_struct_ptr_t XD)
{
   ScratchStructPtr_t tmp_ptr;
   
   if (head == (xd_struct_ptr_t)NULL)
   {       
      (head) = (XD);            
   }
   else
   {    
      tmp_ptr = (ScratchStructPtr_t)((__u32)(tail)->wScratchPtr );
      tmp_ptr->pPrivate = (XD);   
   } /* Endif */                 
   (tail) = (XD);               
   (XD)->wScratchPtr->pPrivate = NULL;
}   

void USB_XD_QGET(xd_struct_ptr_t head, xd_struct_ptr_t tail, xd_struct_ptr_t *XD)
{
   (*XD) = (head);               
   if (head) {                  
      (head) = (xd_struct_ptr_t)((head)->wScratchPtr->pPrivate);  
      if ((head) == NULL) {      
         (tail) = NULL;          
      } /* Endif */              
   } /* Endif */
}

void EHCI_DTD_QADD(xd_struct_ptr_t head, xd_struct_ptr_t tail, xd_struct_ptr_t dTD)
{
   if ((head) == NULL) {         
      (head) = (dTD);            
   } else {                      
      (tail)->wScratchPtr->pPrivate = (void *) (dTD);   \
   } /* Endif */                
   (tail) = (dTD);               
   (dTD)->wScratchPtr->pPrivate = NULL;
}


void EHCI_DTD_QGET(xd_struct_ptr_t head, xd_struct_ptr_t tail, xd_struct_ptr_t *dTD)
{
   (*dTD) = (head);               
   if (head) {                  
      (head) = (head)->wScratchPtr->pPrivate;  
      if ((head) == NULL) {      
         (tail) = NULL;         
      } /* Endif */             
   } /* Endif */
}



/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_device_free_XD
*  Returned Value : void
*  Comments       :
*        Enqueues a XD onto the free XD ring.
*
*END*-----------------------------------------------------------------*/

void _usb_device_free_XD
   (
      /* [IN] the dTD to enqueue */
      void*  xd_ptr
   )
{ 
   ScratchStructPtr_t         tmp_ptr;
   USB_DEV_STATE_STRUCT_PTR   usb_dev_ptr;
   
   /* Body */
   tmp_ptr = ((xd_struct_ptr_t)xd_ptr)->wScratchPtr;
   tmp_ptr = (ScratchStructPtr_t)((__u32)tmp_ptr);
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)(tmp_ptr->pPrivate);

   /*
   ** This function can be called from any context, and it needs mutual
   ** exclusion with itself.
   */

   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_free_XD");
   #endif

   //USB_lock();

   /*
   ** Add the XD to the free XD queue (linked via PRIVATE) and
   ** increment the tail to the next descriptor
   */
#if 0 
   USB_XD_QADD(
       usb_dev_ptr->XD_HEAD,
       usb_dev_ptr->XD_TAIL, 
      (xd_struct_ptr_t)((__u32)xd_ptr));
#else
  {
   ScratchStructPtr_t tmp_ptr;
   
   if (usb_dev_ptr->XD_HEAD == (xd_struct_ptr_t)NULL)
   {       
      (usb_dev_ptr->XD_HEAD) = (xd_struct_ptr_t)(xd_ptr);            
   }
   else
   {    
      tmp_ptr = (ScratchStructPtr_t)((__u32)(usb_dev_ptr->XD_TAIL)->wScratchPtr);
      tmp_ptr->pPrivate = (xd_struct_ptr_t)(xd_ptr);   
   } /* Endif */                 
   (usb_dev_ptr->XD_TAIL) = (xd_struct_ptr_t)(xd_ptr);   
   tmp_ptr = ((xd_struct_ptr_t)(xd_ptr))->wScratchPtr;
   tmp_ptr = (ScratchStructPtr_t)((__u32)tmp_ptr );
   tmp_ptr->pPrivate = NULL;
  }   
#endif

   usb_dev_ptr->XD_ENTRIES++;

   //USB_unlock();
   
   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_free_XD, SUCCESSFUL");
   #endif

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_device_init
*  Returned Value : USB_OK or error code
*  Comments       :
*        Initializes the USB device specific data structures and calls 
*  the low-level device controller chip initialization routine.
*
*END*-----------------------------------------------------------------*/
__u8 _usb_device_init
   (
      /* [IN] the USB device controller to initialize */
      __u8                    devnum,

      /* [OUT] the USB_USB_dev_initialize state structure */
      _usb_device_handle *  handle,
            
      /* [IN] number of endpoints to initialize */
      __u8                    endpoints 
   )
{ /* Body */

   USB_DEV_STATE_STRUCT_PTR         usb_dev_ptr;
   xd_struct_ptr_t                    xd_ptr;
   __u8                           temp, i, error;
   ScratchStructPtr_t               temp_scratch_ptr;
   __u32                          total_memory;
   __u8*                       stack_mem_ptr;
   
  
   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_init");
   #endif
   
   #ifndef REDUCED_ERROR_CHECKING                   
      
   if ((devnum !=  WPCM450_USB11_PORT) && (devnum !=  WPCM450_USB20_PORT))
   {
     #ifdef _DEVICE_DEBUG_
     printk("_usb_device_init, error invalid device number");
     #endif
     return USBERR_INVALID_DEVICE_NUM;
   } /* Endif */
   
   #endif


   spin_lock_init(&usb_spin_lock);
   
   /**************************************************************
   All memory allocations should be consolidated in one.
   **************************************************************/
                      
                 
   /* Allocate memory for the state structure */
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)USB_memalloc(sizeof(USB_DEV_STATE_STRUCT));

           
   if (usb_dev_ptr == NULL) 
   {
     #ifdef _DEVICE_DEBUG_
     printk("_usb_device_init, error NULL device handle");
     #endif
     return USBERR_ALLOC_STATE;
   } /* Endif */

   
   /* Zero out the internal USB state structure */
   USB_memzero(usb_dev_ptr,sizeof(USB_DEV_STATE_STRUCT));
   
   /**************************************************************
   **************************************************************/
      

   
   /* Multiple devices will have different base addresses and 
   ** interrupt vectors (For future)
   */   
   if (devnum == WPCM450_USB11_PORT)
   {
     usb_dev_ptr->DEV_VEC =  IRQ_USBD1;
   }

   if (devnum == WPCM450_USB20_PORT)
   {
     usb_dev_ptr->DEV_VEC =  IRQ_USBD2;
   }
   
   usb_dev_ptr->USB_STATE = USB_STATE_UNKNOWN;
   usb_dev_ptr->DEV_NUM = devnum;
   usb_dev_ptr->MAX_ENDPOINTS = endpoints;

               
   /* Allocate MAX_XDS_FOR_TR_CALLS */
   xd_ptr = (xd_struct_ptr_t) USB_memalloc(sizeof(xd_struct_t) * MAX_XDS_FOR_TR_CALLS);
   
   if (xd_ptr == NULL) 
   {
     #ifdef _DEVICE_DEBUG_
     printk("_usb_device_init, malloc error");
     #endif
     return USBERR_ALLOC_TR;
   }

   USB_memzero(xd_ptr, sizeof(xd_struct_t) * MAX_XDS_FOR_TR_CALLS);



   /* Allocate memory for internal scratch structure */   
   usb_dev_ptr->XD_SCRATCH_STRUCT_BASE = (ScratchStructPtr_t)USB_memalloc(sizeof(ScratchStruct_t)*MAX_XDS_FOR_TR_CALLS);
   
   if (usb_dev_ptr->XD_SCRATCH_STRUCT_BASE == NULL) 
   {
     #ifdef _DEVICE_DEBUG_
        printk("_usb_device_init, malloc error");
     #endif
         return USBERR_ALLOC;
   } /* Endif */
  
      
   
   usb_dev_ptr->XD_BASE = (xd_struct_ptr_t)((__u32)xd_ptr & NOMMU);
   

   temp_scratch_ptr = usb_dev_ptr->XD_SCRATCH_STRUCT_BASE;
   
 
   usb_dev_ptr->XD_HEAD = NULL;
   usb_dev_ptr->XD_TAIL = NULL;
   usb_dev_ptr->XD_ENTRIES = 0;
  
  
   /* Enqueue all the XDs */   
   for (i=0;i<MAX_XDS_FOR_TR_CALLS;i++) 
   {
      xd_ptr->wScratchPtr = (ScratchStructPtr_t)((__u32)temp_scratch_ptr);
      
      
      temp_scratch_ptr->pFreeCallback = _usb_device_free_XD;
      temp_scratch_ptr->pPrivate = (void*)usb_dev_ptr;

      _usb_device_free_XD((void*)xd_ptr);
      
      xd_ptr++;
      temp_scratch_ptr++;
   } /* Endfor */

     
   usb_dev_ptr->TEMP_XD_PTR = (xd_struct_ptr_t)USB_memalloc(sizeof(xd_struct_t));

   USB_memzero(usb_dev_ptr->TEMP_XD_PTR, sizeof(xd_struct_t));
      
   
   /* Initialize the USB controller chip */
   

   error = _usb_dci_init(devnum, usb_dev_ptr);


   if (error) 
   {
      #ifdef _DEVICE_DEBUG_
         printk("_usb_device_init, init failed");
      #endif
      return USBERR_INIT_FAILED;
   } /* Endif */

  
   *handle = usb_dev_ptr;

   g_usb_init_done = TRUE;
   
   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_init, SUCCESSFUL");
   #endif

   
   return USB_OK;

   
} /* EndBody */


/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : _usb_device_register_service
* Returned Value : USB_OK or error code
* Comments       :
*     Registers a callback routine for a specified event or endpoint.
* 
*END*--------------------------------------------------------------------*/
__u8 _usb_device_register_service
   (
      /* [IN] Handle to the USB device */
      _usb_device_handle         handle,
      
      /* [IN] type of event or endpoint number to service */
      __u8                     type,
      
      /* [IN] Pointer to the service's callback function */
      void(* service)(void*, boolean, __u8, __u8*, __u32, __u8)
   )
{ /* Body */
   volatile USB_DEV_STATE_STRUCT_PTR   usb_dev_ptr;
   volatile service_struct_ptr_t       service_ptr;
   volatile service_struct_ptr_t*      search_ptr;

   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_register_service");
   #endif
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;
   /* Needs mutual exclusion */
   USB_lock();
   
   /* Search for an existing entry for type */
   for (search_ptr = &usb_dev_ptr->SERVICE_HEAD_PTR;
      *search_ptr;
      search_ptr = &(*search_ptr)->next) 
   {
      if ((*search_ptr)->type == type) {
         /* Found an existing entry */
         USB_unlock();
         #ifdef _DEVICE_DEBUG_
            printk("_usb_device_register_service, service closed");
         #endif
         return USBERR_OPEN_SERVICE;
      } /* Endif */
   } /* Endfor */
   
   /* No existing entry found - create a new one */
   service_ptr = (service_struct_ptr_t)USB_memalloc(sizeof(service_struct_t));
   
   if (!service_ptr) 
   {
      USB_unlock();
      #ifdef _DEVICE_DEBUG_
         printk("_usb_device_register_service, error allocating service");
      #endif
      return USBERR_ALLOC;
   } /* Endif */
   
   service_ptr->type = type;
   service_ptr->service = service;
   service_ptr->next = NULL;
   *search_ptr = service_ptr;
   
   USB_unlock();

   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_register_service, SUCCESSFUL");
   #endif
   return USB_OK;
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : _usb_device_unregister_service
* Returned Value : USB_OK or error code
* Comments       :
*     Unregisters a callback routine for a specified event or endpoint.
* 
*END*--------------------------------------------------------------------*/
__u8 _usb_device_unregister_service
   (
      /* [IN] Handle to the USB device */
      _usb_device_handle         handle,

      /* [IN] type of event or endpoint number to service */
      __u8                     type
   )
{ /* Body */
   USB_DEV_STATE_STRUCT_PTR   usb_dev_ptr;
   service_struct_ptr_t         service_ptr;
   service_struct_ptr_t *   search_ptr;

   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_unregister_service");
   #endif
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;
   /* Needs mutual exclusion */
   USB_lock();
   
   /* Search for an existing entry for type */
   for (search_ptr = &usb_dev_ptr->SERVICE_HEAD_PTR;
      *search_ptr;
      search_ptr = &(*search_ptr)->next) 
   {
      if ((*search_ptr)->type == type) {
         /* Found an existing entry - delete it */
         break;
      } /* Endif */
   } /* Endfor */
   
   /* No existing entry found */
   if (!*search_ptr) 
   {
      USB_unlock();
      #ifdef _DEVICE_DEBUG_
         printk("_usb_device_unregister_service, no service found");
      #endif
      return USBERR_CLOSED_SERVICE;
   } /* Endif */
   
   service_ptr = *search_ptr;
   *search_ptr = service_ptr->next;

   USB_memfree((void*)service_ptr);
   
   USB_unlock();

   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_unregister_service, SUCCESSFUL");
   #endif
   return USB_OK;

} /* EndBody */



/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : _usb_device_call_service
* Returned Value : USB_OK or error code
* Comments       :
*     Calls the appropriate service for the specified type, if one is
*     registered. Used internally only.
* 
*END*--------------------------------------------------------------------*/
__u8 _usb_device_call_service
   (
      /* [IN] Handle to the USB device */
      _usb_device_handle   handle,

      /* [OUT] Type of service or endpoint */
      __u8               type,
      
      /* [OUT] Is it a Setup transfer? */
      boolean              setup,
      
      /* [OUT] Direction of transmission; is it a Transmit? */
      boolean              direction,

      /* [OUT] Pointer to the data */
      __u8*           buffer_ptr,
      
      /* [OUT] Number of bytes in transmission */
      __u32              length,

      /* [OUT] Any errors */
      __u8               errors
   )
{ /* Body */



   USB_DEV_STATE_STRUCT_PTR      usb_dev_ptr;
   service_struct_t *          service_ptr;

   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_call_service");
   #endif
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;
   /* Needs mutual exclusion */
   USB_lock();
   
   /* Search for an existing entry for type */
   for (service_ptr = usb_dev_ptr->SERVICE_HEAD_PTR;
      service_ptr;
      service_ptr = service_ptr->next) 
   {
      
      if (service_ptr->type == type) {
         service_ptr->service(handle, setup, direction, buffer_ptr, length, errors);
         USB_unlock();
         #ifdef _DEVICE_DEBUG_
            printk("_usb_device_call_service, SUCCESSFUL service called");
         #endif
         return USB_OK;
      } /* Endif */
      
   } /* Endfor */

   USB_unlock();

   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_call_service, SUCCESSFUL service closed");
   #endif
   return USBERR_CLOSED_SERVICE;

   
/**********************************************************************
When code size needs to be reduced we just hardcode the routine names
inside this routine. This allows the applications to, not call
register or unregister routines, thus saving code space and 
providing performace improvement.
**********************************************************************/
   

 return USB_OK;  
} /* EndBody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_device_init_endpoint
*  Returned Value : USB_OK or error code
*  Comments       :
*     Initializes the endpoint and the data structures associated with the 
*  endpoint
*
*END*-----------------------------------------------------------------*/
__u8 _usb_device_init_endpoint
   (
      /* [IN] the USB_USB_dev_initialize state structure */
      _usb_device_handle         handle,
            
      /* [IN] the Endpoint number */
      __u8                     ep_num,
            
      /* [IN] MAX Packet size for this endpoint */
      __u16                    max_pkt_size,
            
      /* [IN] Direction */
      __u8                     direction,
            
      /* [IN] Type of Endpoint */
      __u8                     type,
            
      /* [IN] After all data is transfered, should we terminate the transfer 
      ** with a zero length packet if the last packet size == MAX_PACKET_SIZE? 
      */
      __u8                     flag   
   )
{ /* Body */

   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_init_endpoint");
   #endif

   __u8         error = 0;
   USB_DEV_STATE_STRUCT_PTR      usb_dev_ptr;
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;

   /* Initialize the transfer descriptor */
   usb_dev_ptr->TEMP_XD_PTR->bEpNum = ep_num;
   usb_dev_ptr->TEMP_XD_PTR->bDirection = direction;
   usb_dev_ptr->TEMP_XD_PTR->wMaxpacketSize = max_pkt_size;
   usb_dev_ptr->TEMP_XD_PTR->bEpType = type;
   usb_dev_ptr->TEMP_XD_PTR->bDontZeroTerminate = flag;
   usb_dev_ptr->TEMP_XD_PTR->bMaxPktsPerUframe = 
      ((flag & USB_MAX_PKTS_PER_UFRAME) >> 1);


   error = _usb_dci_vusb20_init_endpoint(handle, usb_dev_ptr->TEMP_XD_PTR);

   
   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_init_endpoint, SUCCESSFUL");
   #endif
   return error;


} /* EndBody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_device_get_transfer_status
*  Returned Value : Status of the transfer
*  Comments       :
*        returns the status of the transaction on the specified endpoint.
*
*END*-----------------------------------------------------------------*/
__u8 _usb_device_get_transfer_status
   (
      /* [IN] the USB_USB_dev_initialize state structure */
      _usb_device_handle         handle,
            
      /* [IN] the Endpoint number */
      __u8                     ep_num,
            
      /* [IN] direction */
      __u8                     direction
   )
{ /* Body */
   __u8   status;
   USB_DEV_STATE_STRUCT_PTR      usb_dev_ptr;

   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_get_transfer_status");
   #endif
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;
   
   USB_lock();


   status = _usb_dci_vusb20_get_transfer_status(handle, ep_num, direction);


   USB_unlock();

   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_get_transfer_status, SUCCESSFUL");
   #endif
   
   /* Return the status of the last queued transfer */
   return (status);

} /* EndBody */


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_device_get_transfer_details
*  Returned Value : Status of the transfer
*  Comments       :
*        returns the status of the transaction on the specified endpoint.
*
*END*-----------------------------------------------------------------*/
void _usb_device_get_transfer_details
   (
      /* [IN] the USB_USB_dev_initialize state structure */
      _usb_device_handle         handle,
            
      /* [IN] the Endpoint number */
      __u8                     ep_num,
            
      /* [IN] direction */
      __u8                     direction,
      
      /* [OUT] transfer detail data structure maintained by driver*/
      __u32**           xd_ptr_ptr                   
   )
{ /* Body */
   __u8   status;
   USB_DEV_STATE_STRUCT_PTR      usb_dev_ptr;
   xd_struct_ptr_t                 temp;

   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_get_transfer_status");
   #endif
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;
   
//   USB_lock();

   temp = _usb_dci_vusb20_get_transfer_details(handle, ep_num, direction);
   (*xd_ptr_ptr) = (__u32*) temp;
   
//   USB_unlock();

   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_get_transfer_status, SUCCESSFUL");
   #endif
   
   return;

} /* EndBody */



/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_device_read_setup_data
*  Returned Value : USB_OK or error code
*  Comments       :
*        Reads the setup data from the hardware
*
*END*-----------------------------------------------------------------*/
void _usb_device_read_setup_data
   (
      /* [IN] the USB_USB_dev_initialize state structure */
      _usb_device_handle         handle,
            
      /* [IN] the Endpoint number */
      __u8                     ep_num,
            
      /* [IN] buffer for receiving Setup packet */
      __u8*                  buff_ptr
   )
{ /* Body */
   USB_DEV_STATE_STRUCT_PTR      usb_dev_ptr;

   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_read_setup_data");
   #endif
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;


   _usb_dci_vusb20_get_setup_data(handle, ep_num, buff_ptr);


   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_read_setup_data, SUCCESSFUL");
   #endif

} /* EndBody */



/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_device_set_address
*  Returned Value : USB_OK or error code
*  Comments       :
*        Sets the device address as assigned by the host during enumeration
*
*END*-----------------------------------------------------------------*/
void _usb_device_set_address
   (
      /* [IN] the USB_USB_dev_initialize state structure */
      _usb_device_handle         handle,
      
      /* [IN] the USB address to be set in the hardware */
      __u8                     address
   )
{ /* Body */
   USB_DEV_STATE_STRUCT_PTR      usb_dev_ptr;
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;

   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_set_address");
   #endif


   _usb_dci_vusb20_set_address(handle, address);


   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_set_address, SUCCESSFUL");
   #endif
   
} /* EndBody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_device_recv_data
*  Returned Value : USB_OK or error code
*  Comments       :
*        Receives data on a specified endpoint.
*
*END*-----------------------------------------------------------------*/
__u8 _usb_device_recv_data
   (
      /* [IN] the USB_USB_dev_initialize state structure */
      _usb_device_handle         handle,
            
      /* [IN] the Endpoint number */
      __u8                     ep_num,
            
      /* [IN] buffer to receive data */
      __u8*                  buff_ptr,
            
      /* [IN] length of the transfer */
      __u32                    size
   )
{ /* Body */

   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_recv_data");
   #endif


   __u8                             error = USB_OK;
   xd_struct_ptr_t                    xd_ptr;
   USB_DEV_STATE_STRUCT_PTR         usb_dev_ptr;
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;

   #ifdef _USB_CACHE_USE_
   /********************************************************
   If system has a data cache, it is assumed that buffer
   passed to this routine will be aligned on a cache line
   boundry. The following code will invalidate the
   buffer before passing it to hardware driver.   
   ********************************************************/
   //USB_dcache_invalidate_mlines((void*)buff_ptr,size);   

   flush_cache_all();
   #endif
   
   USB_lock();

   if (!usb_dev_ptr->XD_ENTRIES) 
   {
      USB_unlock();
      #ifdef _DEVICE_DEBUG_
         printk("_usb_device_recv_data, transfer in progress");
      #endif
      return USB_STATUS_TRANSFER_IN_PROGRESS;
   } /* Endif */

   /* Get a transfer descriptor for the specified endpoint 
   ** and direction 
   */
   USB_XD_QGET(usb_dev_ptr->XD_HEAD, usb_dev_ptr->XD_TAIL, &xd_ptr);
   
   usb_dev_ptr->XD_ENTRIES--;

   /* Initialize the new transfer descriptor */      
   xd_ptr->bEpNum = ep_num;
   xd_ptr->bDirection = USB_RECV;
   xd_ptr->wToatalLength = size;
   xd_ptr->wSoFar = 0;
   xd_ptr->wStartaddress = buff_ptr;
   
   xd_ptr->bStatus = USB_STATUS_TRANSFER_ACCEPTED;


   error = _usb_dci_vusb20_recv_data(handle, xd_ptr);

     
   USB_unlock();
   
   if (error) 
   {
      #ifdef _DEVICE_DEBUG_
         printk("_usb_device_recv_data, receive failed");
      #endif
      return USBERR_RX_FAILED;
   } /* Endif */
   
   return error;

} /* EndBody */


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_device_send_data
*  Returned Value : USB_OK or error code
*  Comments       :
*        Sends data on a specified endpoint.
*
*END*-----------------------------------------------------------------*/
__u8 _usb_device_send_data
   (
      /* [IN] the USB_USB_dev_initialize state structure */
      _usb_device_handle         handle,
            
      /* [IN] the Endpoint number */
      __u8                     ep_num,
            
      /* [IN] buffer to send */
      __u8*                  buff_ptr,
            
      /* [IN] length of the transfer */
      __u32                    size
   )
{ /* Body */

   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_send_data");
   #endif

   __u8                           error = 0;
   volatile xd_struct_ptr_t                    xd_ptr;
   volatile USB_DEV_STATE_STRUCT_PTR         usb_dev_ptr;
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;
   
   #ifdef _USB_CACHE_USE_
   /********************************************************
   If system has a data cache, it is assumed that buffer
   passed to this routine will be aligned on a cache line
   boundry. The following code will flush the
   buffer before passing it to hardware driver.   
   ********************************************************/
   //USB_dcache_flush_mlines((void*)buff_ptr,size);   

   flush_cache_all();
   
   #endif

   
   
   USB_lock();

     
   if(usb_dev_ptr == NULL)
   {
     printk("_usb_device_send_data, error\n");
     USB_unlock();
     return USBERR_TX_FAILED;
   }

   
   if (!usb_dev_ptr->XD_ENTRIES) 
   {
      USB_unlock();
      #ifdef _DEVICE_DEBUG_
         printk("_usb_device_send_data, transfer in progress");
      #endif
      return USBERR_TX_FAILED;
   } /* Endif */

   /* Get a transfer descriptor */
   USB_XD_QGET(usb_dev_ptr->XD_HEAD, usb_dev_ptr->XD_TAIL, &xd_ptr);

   usb_dev_ptr->XD_ENTRIES--;


   if(usb_dev_ptr == NULL)
   {
     printk("_usb_device_send_data, error\n");
     USB_unlock();
     return USB_STATUS_ERROR;
   }
   
   /* Initialize the new transfer descriptor */      
   xd_ptr->bEpNum = ep_num;
   xd_ptr->bDirection = USB_SEND;
   xd_ptr->wToatalLength = size;
   xd_ptr->wSoFar = 0;
   xd_ptr->wStartaddress = buff_ptr;
   
   xd_ptr->bStatus = USB_STATUS_TRANSFER_ACCEPTED;
   
   error = _usb_dci_vusb20_send_data(handle, xd_ptr);

   USB_unlock();
   
   if (error) 
   {
      #ifdef _DEVICE_DEBUG_
         printk("_usb_device_send_data, transfer failed");
      #endif
      return USBERR_TX_FAILED;
   } /* Endif */

   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_send_data, SUCCESSFUL");
   #endif
   return error;


} /* EndBody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_device_cancel_transfer
*  Returned Value : USB_OK or error code
*  Comments       :
*        returns the status of the transaction on the specified endpoint.
*
*END*-----------------------------------------------------------------*/
__u8 _usb_device_cancel_transfer
   (
      /* [IN] the USB_USB_dev_initialize state structure */
      _usb_device_handle         handle,
            
      /* [IN] the Endpoint number */
      __u8                     ep_num,
            
      /* [IN] direction */
      __u8                     direction
   )
{ /* Body */
   __u8                        error = USB_OK;
   USB_DEV_STATE_STRUCT_PTR      usb_dev_ptr;

   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_cancel_transfer");
   #endif
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;
   
   USB_lock();

   /* Cancel transfer on the specified endpoint for the specified 
   ** direction 
   */

   error = _usb_dci_vusb20_cancel_transfer(handle, ep_num, direction);


   USB_unlock();

   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_cancel_transfer, SUCCESSFUL");
   #endif
   return error;

} /* EndBody */


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_device_deinit_endpoint
*  Returned Value : USB_OK or error code
*  Comments       :
*  Disables the endpoint and the data structures associated with the 
*  endpoint
*
*END*-----------------------------------------------------------------*/
__u8 _usb_device_deinit_endpoint
   (
      /* [IN] the USB_USB_dev_initialize state structure */
      _usb_device_handle         handle,
            
      /* [IN] the Endpoint number */
      __u8                    ep_num,
            
      /* [IN] Direction */
      __u8                    direction
   )
{ /* Body */
   __u8         error = 0;
   USB_DEV_STATE_STRUCT_PTR      usb_dev_ptr;

   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_deinit_endpoint");
   #endif

   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;
   USB_lock();

   error = _usb_dci_vusb20_deinit_endpoint(handle, ep_num, direction);
  
   USB_unlock();

   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_deinit_endpoint,SUCCESSFUL");
   #endif
   
   return error;
} /* EndBody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_device_shutdown
*  Returned Value : USB_OK or error code
*  Comments       :
*        Shutdown an initialized USB device
*
*END*-----------------------------------------------------------------*/
void _usb_device_shutdown
   (
      /* [IN] the USB_USB_dev_initialize state structure */
      _usb_device_handle         handle
   )
{ /* Body */
   USB_DEV_STATE_STRUCT_PTR         usb_dev_ptr;

   #ifdef _DEVICE_DEBUG_
      printk( "_usb_device_shutdown");
   #endif
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;

   _usb_dci_vusb20_shutdown(usb_dev_ptr);

   
   /* Free all the Callback function structure memory */
   USB_memfree((void*)usb_dev_ptr->SERVICE_HEAD_PTR);

   /* Free all internal transfer descriptors */
   USB_memfree((void*)usb_dev_ptr->XD_BASE);
   
   /* Free all XD scratch memory */
   USB_memfree((void*)usb_dev_ptr->XD_SCRATCH_STRUCT_BASE);

   /* Free the temp ep init XD */
   USB_memfree((void*)usb_dev_ptr->TEMP_XD_PTR);

   /* Free the USB state structure */
   USB_memfree((void*)usb_dev_ptr);

   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_shutdown,SUCCESSFUL");
   #endif
   
} /* EndBody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_device_unstall_endpoint
*  Returned Value : USB_OK or error code
*  Comments       :
*     Unstalls the endpoint in specified direction
*
*END*-----------------------------------------------------------------*/
void _usb_device_unstall_endpoint
   (
      /* [IN] the USB_USB_dev_initialize state structure */
      _usb_device_handle         handle,
            
      /* [IN] the Endpoint number */
      __u8                     ep_num,
            
      /* [IN] direction */
      __u8                     direction
   )
{ /* Body */
   /* __u8         error = 0; */
   USB_DEV_STATE_STRUCT_PTR      usb_dev_ptr;
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;

   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_unstall_endpoint");
   #endif
   
   USB_lock();
   
   _usb_dci_vusb20_unstall_endpoint(handle, ep_num, direction);

   USB_unlock();

   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_unstall_endpoint, SUCCESSFULL");
   #endif
   
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : _usb_device_get_status
* Returned Value : USB_OK or error code
* Comments       :
*     Provides API to access the USB internal state.
* 
*END*--------------------------------------------------------------------*/
__u8 _usb_device_get_status
   (
      /* [IN] Handle to the USB device */
      _usb_device_handle   handle,
      
      /* [IN] What to get the status of */
      __u8               component,
      
      /* [OUT] The requested status */
      __u16*          status
   )
{ /* Body */
   USB_DEV_STATE_STRUCT_PTR usb_dev_ptr;

   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;

   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_get_status, SUCCESSFULL");
   #endif
   
   USB_lock();

   switch (component) {

      case USB_STATUS_DEVICE_STATE:
         *status = usb_dev_ptr->USB_STATE;
         break;

      case USB_STATUS_DEVICE:
         *status = usb_dev_ptr->USB_DEVICE_STATE;
         break;
         #ifndef SMALL_CODE_SIZE
      case USB_STATUS_INTERFACE:
         break;
         #endif         
      case USB_STATUS_ADDRESS:
         *status = usb_dev_ptr->DEVICE_ADDRESS;
         break;
         
      case USB_STATUS_CURRENT_CONFIG:
         *status = usb_dev_ptr->USB_CURR_CONFIG;
         break;

      case USB_STATUS_SOF_COUNT:
         *status = usb_dev_ptr->USB_SOF_COUNT;
         break;
         
      default:
         if (component & USB_STATUS_ENDPOINT) {


            *status = _usb_dci_vusb20_get_endpoint_status(handle,
               component & USB_STATUS_ENDPOINT_NUMBER_MASK);

         } else {
            USB_unlock();
            #ifdef _DEVICE_DEBUG_
               printk("_usb_device_get_status, bad status");
            #endif
            return USBERR_BAD_STATUS;
         } /* Endif */
         break;

   } /* Endswitch */

   USB_unlock();

   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_get_status, SUCCESSFUL");
   #endif
   
   return USB_OK;   
} /* EndBody */  
 
/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : _usb_device_set_status
* Returned Value : USB_OK or error code
* Comments       :
*     Provides API to set internal state
* 
*END*--------------------------------------------------------------------*/
__u8 _usb_device_set_status
   (
      /* [IN] Handle to the usb device */
      _usb_device_handle   handle,
      
      /* [IN] What to set the status of */
      __u8               component,
      
      /* [IN] What to set the status to */
      __u16              setting
   )
{ /* Body */
   USB_DEV_STATE_STRUCT_PTR usb_dev_ptr;

   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_set_status");
   #endif
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;
   USB_lock();
   switch (component) {

      case USB_STATUS_DEVICE_STATE:
         usb_dev_ptr->USB_STATE = setting;
         break;

      case USB_STATUS_DEVICE:
         usb_dev_ptr->USB_DEVICE_STATE = setting;
         break;

      case USB_STATUS_INTERFACE:
         break;

      case USB_STATUS_CURRENT_CONFIG:
         usb_dev_ptr->USB_CURR_CONFIG = setting;
         break;

      case USB_STATUS_SOF_COUNT:
         usb_dev_ptr->USB_SOF_COUNT = setting;
         break;
         

      case USB_FORCE_FULL_SPEED:
         _usb_dci_vusb20_set_speed_full((void*)usb_dev_ptr, setting);
         break;
      case USB_PHY_LOW_POWER_SUSPEND:
         _usb_dci_vusb20_suspend_phy((void*)usb_dev_ptr, setting);
         break;


      case USB_STATUS_ADDRESS:
         usb_dev_ptr->DEVICE_ADDRESS = setting;


         _usb_dci_vusb20_set_address((void*)usb_dev_ptr, setting);

         break;

      
      case USB_STATUS_TEST_MODE:
         _usb_dci_vusb20_set_test_mode(handle, 
            setting);
         break;
         
      default:
         if (component & USB_STATUS_ENDPOINT) {

            _usb_dci_vusb20_set_endpoint_status(handle,
               component & USB_STATUS_ENDPOINT_NUMBER_MASK,
               setting);

         } else {
            USB_unlock();
            #ifdef _DEVICE_DEBUG_
               printk("_usb_device_set_status, bad status");
            #endif
            return USBERR_BAD_STATUS;
         } /* Endif */
         break;


   } /* Endswitch */

   USB_unlock();

   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_set_status, SUCCESSFUL");
   #endif
   
   return USB_OK;   
} /* EndBody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_device_stall_endpoint
*  Returned Value : USB_OK or error code
*  Comments       :
*     Stalls the endpoint.
*
*END*-----------------------------------------------------------------*/
void _usb_device_stall_endpoint
   (
      /* [IN] the USB_USB_dev_initialize state structure */
      _usb_device_handle         handle,
            
      /* [IN] the Endpoint number */
      __u8                     ep_num,
            
      /* [IN] direction */
      __u8                     direction
   )
{ /* Body */
   /* __u8         error = 0; */
   USB_DEV_STATE_STRUCT_PTR      usb_dev_ptr;

   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_stall_endpoint");
   #endif
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;

   _usb_dci_vusb20_stall_endpoint(handle, ep_num, direction);


   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_stall_endpoint, SUCCESSFUL");
   #endif
   
} /* EndBody */


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_device_process_resume
*  Returned Value : USB_OK or error code
*  Comments       :
*        Process Resume event
*
*END*-----------------------------------------------------------------*/
void _usb_device_assert_resume
   (
      /* [IN] the USB_USB_dev_initialize state structure */
      _usb_device_handle         handle
   )
{ /* Body */
   USB_DEV_STATE_STRUCT_PTR      usb_dev_ptr;

   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_assert_resume");
   #endif
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;

   _usb_dci_vusb20_assert_resume(handle);


   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_assert_resume, SUCCESSFUL");
   #endif

} /* EndBody */



/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_device_run
*  Returned Value : USB_OK or error code
*  Comments       :
*        
*
*END*-----------------------------------------------------------------*/
void _usb_device_run
   (
      /* [IN] the USB_USB_dev_initialize state structure */
      _usb_device_handle         handle
      
   )
{ /* Body */
   USB_DEV_STATE_STRUCT_PTR      usb_dev_ptr;
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;

   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_run");
   #endif


   _usb_dci_vusb20_chip_run(handle);


   #ifdef _DEVICE_DEBUG_
      printk("_usb_device_run, SUCCESSFUL");
   #endif
   
} /* EndBody */


void USB_lock(void)
{
  if(g_usb_init_done == TRUE)
  {
    
    spin_lock(&usb_spin_lock);
    //printk("usb_lock\n");
  }
}

void USB_unlock(void)
{
  if(g_usb_init_done == TRUE)
  {
    
    spin_unlock(&usb_spin_lock);
    //printk("usb_unlock\n");
  }
}

/* EOF */

