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
** $Date: 2008/02/26 08:15:42Z $
** $Revision: 1.6 $
*******************************************************************************
*** Description:      
***  This file contains the main VUSB_HS Device Controller interface 
***  functions.
***                                                               
**************************************************************************
**END*********************************************************/

#include <asm/cacheflush.h>
#include <asm/io.h>
#include <asm/arch/regs-udc.h>
#include <asm/arch/regs-irq.h>
#include <asm/arch/regs-clock.h>
#include <asm/arch/regs-gcr.h>
#include <linux/delay.h>
#include "wpcm450_devapi.h"
#include "wpcm450_usbprv.h"
#include "wpcm450_usbprv_dev.h"
#include <asm/irq.h>

#define TRIP_WIRE
#define  USB_TEST_MODE_TEST_PACKET_LENGTH   (53)



/* Test packet for Test Mode : TEST_PACKET. USB 2.0 Specification section 7.1.20 */
__u8 test_packet[USB_TEST_MODE_TEST_PACKET_LENGTH] = 
{
   /* Synch */
   /* DATA 0 PID */
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 
   0xAA, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 
   0xEE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xBF, 0xDF, 
   0xEF, 0xF7, 0xFB, 0xFD, 0xFC, 0x7E, 0xBF, 0xDF, 
   0xEF, 0xF7, 0xFB, 0xFD, 0x7E
};


int usb_dbg_cnt = 0;

static irqreturn_t _usb_dci_vusb20_isr (int irq, void *dev, struct pt_regs * r);

static int _usb_irq_init(
                __u8   vector_number,
                irqreturn_t (*isr_ptr)(int, void *, struct pt_regs *),
                void* handle
                );



#ifdef __MISSING_RESUME_INTERRUPT_WORKAROUND__
__u32 port_change_counter=0;
__u32 port_change_log[100];
__u32 sof_after_suspend =0;
#endif /* __MISSING_RESUME_INTERRUPT_WORKAROUND__ */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_dci_init
*  Returned Value : USB_OK or error code
*  Comments       :
*        Initializes the USB device controller.
*
*END*-----------------------------------------------------------------*/
__u8 _usb_dci_init(
          __u8               devnum, /* [IN] the USB device controller to initialize */
          _usb_device_handle handle /* [OUT] the USB_dev_initialize state structure */
   )
{ /* Body */
   volatile USB_DEV_STATE_STRUCT_PTR  usb_dev_ptr;
   __u32 temp;
   __u32 total_memory=0;
   volatile __u8* driver_memory;
   

   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_init\n");
   #endif
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;

       
      
   /****************************************************************   
         Consolidated memory allocation    
    ****************************************************************/   

   /* Get the maximum number of endpoints supported by this USB controller */
   usb_dev_ptr->MAX_ENDPOINTS = (__raw_readl(WPCM450_UDC_DCCPARAMS_REG(devnum)) & VUSB20_MAX_ENDPTS_SUPPORTED);
   
    
   temp = (usb_dev_ptr->MAX_ENDPOINTS * 2);
   
   /****************************************************************   
     Consolidated memory allocation    
    ****************************************************************/   
   total_memory = ((temp * sizeof(EpQueueHeadStruct_t)) + 2048) +
                     ((MAX_EP_TR_DESCRS * sizeof(EpTrStruct_t)) + 32) +
                     (sizeof(ScratchStruct_t)*MAX_EP_TR_DESCRS);

   driver_memory = (__u8*) USB_memalloc(total_memory);
   //driver_memory = (__u8*)WPCM450_VA_USB1_MEM;
   
   if (driver_memory == NULL)
   {
     #ifdef _DEVICE_DEBUG_
     printk("_usb_dci_init, malloc failed");
     #endif
     
     return USBERR_ALLOC;
   }

  
   /****************************************************************   
      Zero out the memory allocated
    ****************************************************************/   
   memset((void *) driver_memory, 0, total_memory );


   #ifdef _USB_CACHE_USE_   
   /****************************************************************   
      Flush the zeroed memory if cache is enabled
   ****************************************************************/   
   //USB_dcache_flush_mlines((void*)driver_memory,total_memory);
   flush_cache_all();
   #endif

   

 
   /****************************************************************   
     Keep a void* to driver memory alloctaion
   ****************************************************************/   
   usb_dev_ptr->DRIVER_MEMORY = driver_memory;
                    
   /****************************************************************   
      Assign QH base
   ****************************************************************/   
   usb_dev_ptr->EP_QUEUE_HEAD_BASE = (EpQueueHeadStructPtr_t) driver_memory;


   driver_memory += ((temp * sizeof(EpQueueHeadStruct_t)) + 2048);

   /* Align the endpoint queue head to 2K boundary */   
   usb_dev_ptr->EP_QUEUE_HEAD_PTR = (EpQueueHeadStructPtr_t)
       USB_MEM2048_ALIGN((__u32)usb_dev_ptr->EP_QUEUE_HEAD_BASE);



   /****************************************************************   
      Assign DTD base
   ****************************************************************/   
  
   usb_dev_ptr->DTD_BASE_PTR = (EpTrStructPtr_t) ((__u32)driver_memory & NOMMU);
   
   driver_memory += ((MAX_EP_TR_DESCRS * sizeof(EpTrStruct_t)) + 32);


   /* Align the dTD base to 32 byte boundary */   
   usb_dev_ptr->DTD_ALIGNED_BASE_PTR = (EpTrStructPtr_t)
       USB_MEM32_ALIGN((__u32)usb_dev_ptr->DTD_BASE_PTR);


   /****************************************************************   
      Assign SCRATCH Structure base
   ****************************************************************/
  
   /* Allocate memory for internal scratch structure */   
   usb_dev_ptr->SCRATCH_STRUCT_BASE = (ScratchStructPtr_t)driver_memory;

  /*********************************************************************
   If there is no need to install ISR, don't install it. Some USB products   
   would just like to hardcode the function pointers for ISR calls.
   *********************************************************************/

 
   usb_dev_ptr->USB_STATE = USB_STATE_UNKNOWN;

   /* Initialize the VUSB_HS controller */   
   _usb_dci_vusb20_chip_initialize((void*)usb_dev_ptr);
   
    
   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_init, SUCCESSFUL");
   #endif

   return USB_OK;   
} /* EndBody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_dci_vusb20_chip_initialize
*  Returned Value : USB_OK or error code
*  Comments       :
*        Initializes the USB device controller.
*
*END*-----------------------------------------------------------------*/
void _usb_dci_vusb20_chip_initialize
   (
      /* [IN] the USB_dev_initialize state structure */
      _usb_device_handle         handle
   )
{ /* Body */
   volatile USB_DEV_STATE_STRUCT_PTR  usb_dev_ptr;
   volatile EpQueueHeadStructPtr_t ep_queue_head_ptr;
   volatile EpTrStructPtr_t dTD_ptr;
   __u32 i;
   __u32 port_control;
   ScratchStructPtr_t temp_scratch_ptr;
   __u32 tmp_reg_val;

   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_chip_initialize\n");
   #endif
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;

   
   /* Stop the controller */
   tmp_reg_val = __raw_readl(WPCM450_UDC_USBCMD_REG(usb_dev_ptr->DEV_NUM));
   tmp_reg_val &= ~EHCI_CMD_RUN_STOP;
   __raw_writel(tmp_reg_val, WPCM450_UDC_USBCMD_REG(usb_dev_ptr->DEV_NUM));

   
   tmp_reg_val = __raw_readl(WPCM450_CLKSEL);
   tmp_reg_val &= ((~(0x3<<6)) | (0x1<<6));
   __raw_writel(tmp_reg_val, WPCM450_CLKSEL);
   
   tmp_reg_val = __raw_readl(WPCM450_GCR_MFSEL1);
   tmp_reg_val |= (0x1<<23); 
   __raw_writel(tmp_reg_val,WPCM450_GCR_MFSEL1);

   tmp_reg_val = __raw_readl(WPCM450_GCR_INTCR);
   tmp_reg_val &= (~(0x0<<13));
   __raw_writel(tmp_reg_val, WPCM450_GCR_INTCR);
   

   /* Reset the controller to get default values */
   __raw_writel(EHCI_CMD_CTRL_RESET, WPCM450_UDC_USBCMD_REG(usb_dev_ptr->DEV_NUM));

   while (__raw_readl(WPCM450_UDC_USBCMD_REG(usb_dev_ptr->DEV_NUM)) & EHCI_CMD_CTRL_RESET) 
   {
    
      /* Wait for the controller reset to complete */
   } /* EndWhile */

   #ifdef TRIP_WIRE
   /* Program the controller to be the USB device controller */ 
   __raw_writel((VUSBHS_MODE_CTRL_MODE_DEV | VUSBHS_MODE_SETUP_LOCK_DISABLE), WPCM450_UDC_USBMODE_REG(usb_dev_ptr->DEV_NUM));
   #else
   /* Program the controller to be the USB device controller */ 
   __raw_writel(VUSBHS_MODE_CTRL_MODE_DEV , WPCM450_UDC_USBMODE_REG(usb_dev_ptr->DEV_NUM));
   #endif

   /* Initialize the internal dTD head and tail to NULL */   
   usb_dev_ptr->DTD_HEAD = NULL;
   usb_dev_ptr->DTD_TAIL = NULL;

   /* Make sure the 16 MSBs of this register are 0s */
   __raw_writel(0, WPCM450_UDC_ENPDTSETUPSTAT_REG(usb_dev_ptr->DEV_NUM));
   
   ep_queue_head_ptr = usb_dev_ptr->EP_QUEUE_HEAD_PTR;

   /* Initialize all device queue heads */
   for (i=0;i<(usb_dev_ptr->MAX_ENDPOINTS * 2);i++) {
      /* Interrupt on Setup packet */
      (ep_queue_head_ptr + i)->dwMaxPktLength = (((__u32)USB_MAX_CTRL_PAYLOAD << VUSB_EP_QUEUE_HEAD_MAX_PKT_LEN_POS) | VUSB_EP_QUEUE_HEAD_IOS);
      (ep_queue_head_ptr + i)->dwNextDtdPtr = VUSB_EP_QUEUE_HEAD_NEXT_TERMINATE;
   } /* Endfor */



   /* Configure the Endpoint List Address */
   __raw_writel((((__u32)ep_queue_head_ptr) & NOMMU), WPCM450_UDC_ENDPOINTLISTADDR_REG(usb_dev_ptr->DEV_NUM));
   
  
#if 1  /* ???? Need review */

   if (__raw_readl(WPCM450_UDC_HCS_PARAMS_REG(usb_dev_ptr->DEV_NUM)) & VUSB20_HCS_PARAMS_PORT_POWER_CONTROL_FLAG) 
   {
      port_control = __raw_readl(WPCM450_UDC_PORTSC1_REG(usb_dev_ptr->DEV_NUM));
      port_control &= (~EHCI_PORTSCX_W1C_BITS | ~EHCI_PORTSCX_PORT_POWER);
      __raw_writel(port_control,WPCM450_UDC_PORTSC1_REG(usb_dev_ptr->DEV_NUM));
   } /* Endif */
#endif


   dTD_ptr = (EpTrStructPtr_t)((__u32)usb_dev_ptr->DTD_ALIGNED_BASE_PTR | _NOMMU);

   temp_scratch_ptr = usb_dev_ptr->SCRATCH_STRUCT_BASE;

   /* Enqueue all the dTDs */   
   for (i=0;i<MAX_EP_TR_DESCRS;i++) 
   {
      temp_scratch_ptr->pFreeCallback = _usb_dci_vusb20_free_dTD;
      temp_scratch_ptr->pPrivate = (void*)usb_dev_ptr;

      dTD_ptr->wScratchPtr = (ScratchStructPtr_t)((__u32)temp_scratch_ptr);
      
      /* Set the dTD to be invalid */
      dTD_ptr->dwNextTrElemPtr = VUSBHS_TD_NEXT_TERMINATE;
      /* Set the Reserved fields to 0 */
      dTD_ptr->dwSizeIocSts &= ~VUSBHS_TD_RESERVED_FIELDS;
      
      _usb_dci_vusb20_free_dTD((void*)dTD_ptr);
      
      dTD_ptr++;
      temp_scratch_ptr++;
   } /* Endfor */




/* Initialize the endpoint 0 properties */
/* Edited by WECI*/
/*	   dev_ptr->REGISTERS.OPERATIONAL_DEVICE_REGISTERS.ENDPTCTRLX[0] = */
/*	      (EHCI_EPCTRL_TX_DATA_TOGGLE_RST | EHCI_EPCTRL_RX_DATA_TOGGLE_RST);	*/
/*	   dev_ptr->REGISTERS.OPERATIONAL_DEVICE_REGISTERS.ENDPTCTRLX[0] &= 	*/
/*      ~(EHCI_EPCTRL_TX_EP_STALL | EHCI_EPCTRL_RX_EP_STALL); */




   /* Enable interrupts don't enable if one is disabled by a compiler switch*/
   __raw_writel(
         
            (EHCI_INTR_INT_EN | 
          #ifndef ERROR_INTERRUPT_DISABLE
            EHCI_INTR_ERR_INT_EN | 
          #endif
          #ifndef PORT_CHANGE_INTERRUPT_DISABLE
            EHCI_INTR_PORT_CHANGE_DETECT_EN | 
          #endif  
          #ifndef SOF_INTERRUPT_DISABLE  
            EHCI_INTR_SOF_UFRAME_EN | 
          #endif
          #ifndef SUSPEND_INTERRUPT_DISABLE  
            EHCI_INTR_DEVICE_SUSPEND |
          #endif  
            EHCI_INTR_RESET_EN),  

          WPCM450_UDC_USBINTR_REG(usb_dev_ptr->DEV_NUM));


    #ifndef SMALL_CODE_SIZE
      usb_dev_ptr->USB_STATE = USB_STATE_UNKNOWN;
    #endif



   /* Set the Run bit in the command register */
  
   tmp_reg_val = __raw_readl(WPCM450_GCR_MFSEL1);
   tmp_reg_val |= (0x1<<23); 
   __raw_writel(tmp_reg_val,WPCM450_GCR_MFSEL1);

   tmp_reg_val = __raw_readl(WPCM450_GCR_INTCR);
   tmp_reg_val |= (0x3<<13);
   __raw_writel(tmp_reg_val, WPCM450_GCR_INTCR);

  
   
   __raw_writel(EHCI_CMD_RUN_STOP , WPCM450_UDC_USBCMD_REG(usb_dev_ptr->DEV_NUM));


 
   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_chip_initialize, SUCCESSFUL");
   #endif
   
} /* EndBody */



/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_dci_vusb20_chip_run
*  Returned Value : USB_OK or error code
*  Comments       :
*        Run the USB device controller.
*
*END*-----------------------------------------------------------------*/
void _usb_dci_vusb20_chip_run  
      (
      /* [IN] the USB_dev_initialize state structure */
      _usb_device_handle         handle
      )
{

   USB_DEV_STATE_STRUCT_PTR                     usb_dev_ptr;
   EpTrStructPtr_t                      dTD_ptr;
   int i;

   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;

   request_irq(usb_dev_ptr->DEV_VEC , &_usb_dci_vusb20_isr, SA_INTERRUPT, "wpcm450_udc", usb_dev_ptr);
   
  __raw_writel(0x47, WPCM450_AIC_SCR20); 
  __raw_writel(0x47, WPCM450_AIC_SCR21);
  
      
   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_chip_run, SUCCESSFUL");
   #endif
}


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_dci_vusb20_free_dTD
*  Returned Value : void
*  Comments       :
*        Enqueues a dTD onto the free DTD ring.
*
*END*-----------------------------------------------------------------*/

void _usb_dci_vusb20_free_dTD
   (
      /* [IN] the dTD to enqueue */
      void*  dTD_ptr
   )
{ /* Body */
   ScratchStructPtr_t         scratch_ptr;
   USB_DEV_STATE_STRUCT_PTR   usb_dev_ptr;

   scratch_ptr = (ScratchStructPtr_t)((__u32)((EpTrStructPtr_t)dTD_ptr)->wScratchPtr);

   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR) (scratch_ptr->pPrivate);

   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_free_dTD");
   #endif

   /*
   ** This function can be called from any context, and it needs mutual
   ** exclusion with itself.
   */
   USB_lock();

   /*
   ** Add the dTD to the free dTD queue (linked via pPrivate) and
   ** increment the tail to the next descriptor
   */
  #if 0 
   EHCI_DTD_QADD(usb_dev_ptr->DTD_HEAD, usb_dev_ptr->DTD_TAIL, 
      (EpTrStructPtr_t)dTD_ptr);
  #else

  {
   ScratchStructPtr_t tmp_ptr;
   
   if (usb_dev_ptr->DTD_HEAD == (xd_struct_ptr_t)NULL)
   {       
      (usb_dev_ptr->DTD_HEAD) = (EpTrStructPtr_t)dTD_ptr;            
   }
   else
   {    
      tmp_ptr = (ScratchStructPtr_t)((__u32)(usb_dev_ptr->DTD_TAIL)->wScratchPtr );
      tmp_ptr->pPrivate = (EpTrStructPtr_t)dTD_ptr;   
   } /* Endif */                 
   (usb_dev_ptr->DTD_TAIL) = (EpTrStructPtr_t)dTD_ptr; 
   tmp_ptr = (ScratchStructPtr_t)(((EpTrStructPtr_t)dTD_ptr )->wScratchPtr);
   tmp_ptr = (ScratchStructPtr_t)((__u32)tmp_ptr ) ;
   tmp_ptr->pPrivate = NULL;
  }   

  #endif
   usb_dev_ptr->DTD_ENTRIES++;

   USB_unlock();

   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_free_dTD, SUCCESSFUL");
   #endif
   

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_dci_vusb20_add_dTD
*  Returned Value : USB_OK or error code
*  Comments       :
*        Adds a device transfer desriptor(s) to the queue.
*
*END*-----------------------------------------------------------------*/
__u8 _usb_dci_vusb20_add_dTD (
          _usb_device_handle         handle, /* [IN] the USB_dev_initialize state structure */
          xd_struct_ptr_t            xd_ptr  /* [IN] The transfer descriptor address */
   )
{ /* Body */
   volatile USB_DEV_STATE_STRUCT_PTR          usb_dev_ptr;
   volatile EpTrStructPtr_t           dTD_ptr, temp_dTD_ptr, first_dTD_ptr = NULL;
   volatile EpQueueHeadStructPtr_t   ep_queue_head_ptr;
   __u32                                      curr_pkt_len, remaining_len; 
   __u32                                      curr_offset, temp, bit_pos, temp_ep_stat;
   ScratchStructPtr_t                         tmp_scratch_ptr;
   volatile __u32                             tmp_reg_val;
   #ifdef TRIP_WIRE
   boolean                                      read_safe;
   #endif
   /*********************************************************************
   For a optimal implementation, we need to detect the fact that
   we are adding DTD to an empty list. If list is empty, we can
   actually skip several programming steps esp. those for ensuring
   that there is no race condition.The following boolean will be useful
   in skipping some code here.
   *********************************************************************/
   boolean                                     usb_list_empty = FALSE;

   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_add_dTD");
   #endif
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;
 

   remaining_len = xd_ptr->wToatalLength;
   
   curr_offset = 0;
   temp = (2*xd_ptr->bEpNum + xd_ptr->bDirection);
   bit_pos = (1 << (16 * xd_ptr->bDirection + xd_ptr->bEpNum));
   
   ep_queue_head_ptr = (EpQueueHeadStructPtr_t)__raw_readl(WPCM450_UDC_ENDPOINTLISTADDR_REG(usb_dev_ptr->DEV_NUM)) +  temp;

   ep_queue_head_ptr = (EpQueueHeadStructPtr_t)((__u32)ep_queue_head_ptr | _NOMMU);
   /*********************************************************************
   This loops iterates through the length of the transfer and divides
   the data in to DTDs each handling the a max of 0x4000 bytes of data.
   The first DTD in the list is stored in a void* called first_dTD_ptr.
   This void* is later linked in to QH for processing by the hardware.   
   *********************************************************************/

   do {
      /* Check if we need to split the transfer into multiple dTDs */
      if (remaining_len > VUSB_EP_MAX_LENGTH_TRANSFER) {
         curr_pkt_len = VUSB_EP_MAX_LENGTH_TRANSFER;
      } else {
         curr_pkt_len = remaining_len;
      } /* Endif */
   
      remaining_len -= curr_pkt_len;

      /* Get a dTD from the queue */   
      EHCI_DTD_QGET(usb_dev_ptr->DTD_HEAD, usb_dev_ptr->DTD_TAIL, &dTD_ptr);
   
      if (!dTD_ptr) {
         #ifdef _DEVICE_DEBUG_
            printk("_usb_dci_vusb20_add_dTD, SUCCESSFUL");
         #endif
         return USBERR_TR_FAILED;
      } /* Endif */

      usb_dev_ptr->DTD_ENTRIES--;

      if (curr_offset == 0) {
         first_dTD_ptr = dTD_ptr;
      } /* Endif */

      #ifdef _USB_CACHE_USE_
      /**************************************************************
      USB Memzero does not bypass the cache and hence we must use
      DTD void* to update the memory and bypass the cache. If
      your DTD are allocated from an uncached regigio, you can
      eliminitate this approach and switch back to USB_memzero().
      **************************************************************/
      
         dTD_ptr->dwNextTrElemPtr   = 0;
         dTD_ptr->dwSizeIocSts      = 0;
         dTD_ptr->dwBuffPtr0        = 0;
         dTD_ptr->dwBuffPtr1        = 0;
         dTD_ptr->dwBuffPtr2        = 0;
         dTD_ptr->dwBuffPtr3        = 0;
         dTD_ptr->dwBuffPtr4        = 0;

      #else
         /* Zero the dTD. Leave the last 4 bytes as that is the scratch void* */
         USB_memzero((void *) dTD_ptr,(sizeof(EpTrStruct_t) - 4));
      #endif   

      /* Initialize the dTD */
      tmp_scratch_ptr = (ScratchStructPtr_t)((__u32)(dTD_ptr->wScratchPtr));
      tmp_scratch_ptr->pPrivate = handle;
   
      /* Set the Terminate bit */
      dTD_ptr->dwNextTrElemPtr = VUSB_EP_QUEUE_HEAD_NEXT_TERMINATE;


      /*************************************************************
      FIX ME: For hig-speed and high-bandwidth ISO IN endpoints,
      we must initialize the multiplied field so that Host can issues
      multiple IN transactions on the endpoint. See the DTD data
      structure for MultiIO field.
      
      S Garg 11/06/2003
      *************************************************************/
      
      /* Fill in the transfer size */
      if (!remaining_len) {
         dTD_ptr->dwSizeIocSts = ((curr_pkt_len << VUSBHS_TD_LENGTH_BIT_POS) | 
                                  (VUSBHS_TD_IOC) |
                                  (VUSBHS_TD_STATUS_ACTIVE));
      } else {
         dTD_ptr->dwSizeIocSts = ((curr_pkt_len << VUSBHS_TD_LENGTH_BIT_POS) |
                                   VUSBHS_TD_STATUS_ACTIVE);
      } /* Endif */
   
      /* Set the reserved field to 0 */
      dTD_ptr->dwSizeIocSts &= ~VUSBHS_TD_RESERVED_FIELDS;

      /* 4K apart buffer page pointers */
      dTD_ptr->dwBuffPtr0 = (__u32)(xd_ptr->wStartaddress + curr_offset) & NOMMU;
      dTD_ptr->dwBuffPtr1 = (__u32)(dTD_ptr->dwBuffPtr0 + 4096) & NOMMU;
      dTD_ptr->dwBuffPtr2 = (__u32)(dTD_ptr->dwBuffPtr1 + 4096) & NOMMU;
      dTD_ptr->dwBuffPtr3 = (__u32)(dTD_ptr->dwBuffPtr2 + 4096) & NOMMU;
      dTD_ptr->dwBuffPtr4 = (__u32)(dTD_ptr->dwBuffPtr3 + 4096) & NOMMU;

      curr_offset += curr_pkt_len;

      /* Maintain the first and last device transfer descriptor per 
      ** endpoint and direction 
      */
      if (!((__u32)usb_dev_ptr->EP_DTD_HEADS[temp] & NOMMU)) 
      {
         usb_dev_ptr->EP_DTD_HEADS[temp] = dTD_ptr;
         /***********************************************
         If list does not have a head, it means that list
         is empty. An empty condition is detected.
         ***********************************************/
         usb_list_empty = TRUE;
      } /* Endif */ 
   
      /* Check if the transfer is to be queued at the end or beginning */
      temp_dTD_ptr = usb_dev_ptr->EP_DTD_TAILS[temp];
      
      /* Remember which XD to use for this dTD */
      tmp_scratch_ptr->pXdForThisDtd = (void*)xd_ptr;
      
      /* New tail */
      usb_dev_ptr->EP_DTD_TAILS[temp] = dTD_ptr;
      if (temp_dTD_ptr) {
         /* Should not do |=. The Terminate bit should be zero */
         temp_dTD_ptr->dwNextTrElemPtr = ((__u32)dTD_ptr & NOMMU);
      } /* Endif */

     
   } while (remaining_len); /* EndWhile */


   /**************************************************************
   In the loop above DTD has already been added to the list
   However endpoint has not been primed yet. If list is not empty we need safter ways to add DTD to the
   existing list. Else we just skip to adding DTD to QH safely.
   **************************************************************/
   
   if(!usb_list_empty)
   {
         #ifdef TRIP_WIRE
               /*********************************************************
               Hardware v3.2+ require the use of semaphore to ensure that
               QH is safely updated.
               *********************************************************/
               
               /*********************************************************
               Check the prime bit. If set goto done
               *********************************************************/
               if (__raw_readl(WPCM450_UDC_ENDPTPRIME_REG(usb_dev_ptr->DEV_NUM)) & bit_pos) 
               {
                  goto done;
               }

               read_safe = FALSE;                      
               while(!read_safe)
               {
                  
                  /*********************************************************
                  start with setting the semaphores
                  *********************************************************/
                  __raw_writel(__raw_readl(WPCM450_UDC_USBCMD_REG(usb_dev_ptr->DEV_NUM)) | EHCI_CMD_ATDTW_TRIPWIRE_SET , WPCM450_UDC_USBCMD_REG(usb_dev_ptr->DEV_NUM));
                                        
                  /*********************************************************
                  Read the endpoint status
                  *********************************************************/
                                                 
                  temp_ep_stat = __raw_readl(WPCM450_UDC_ENDPTSTATUS_REG(usb_dev_ptr->DEV_NUM)) & bit_pos; 

                  /*********************************************************
                  Reread the ATDTW semaphore bit to check if it is cleared.
                  When hardware see a hazard, it will clear the bit or
                  else we remain set to 1 and we can proceed with priming
                  of endpoint if not already primed.
                  *********************************************************/
                  if(__raw_readl(WPCM450_UDC_USBCMD_REG(usb_dev_ptr->DEV_NUM))& EHCI_CMD_ATDTW_TRIPWIRE_SET);
                  {
                        read_safe = TRUE;
                  }

               }/*end while loop */

               /*********************************************************
               Clear the semaphore
               *********************************************************/
               __raw_writel((__raw_readl(WPCM450_UDC_USBCMD_REG(usb_dev_ptr->DEV_NUM)) &EHCI_CMD_ATDTW_TRIPWIRE_CLEAR), WPCM450_UDC_USBCMD_REG(usb_dev_ptr->DEV_NUM));
               
            #ifdef _USB_CACHE_USE_
               flush_cache_all();//zmsong              
            #endif
               /*********************************************************
               If endpoint is not active, we activate it now.               
               *********************************************************/
               if(!temp_ep_stat)
               {
                  /* No other transfers on the queue */
                  ep_queue_head_ptr->dwNextDtdPtr = (__u32)first_dTD_ptr & NOMMU;
                  ep_queue_head_ptr->dwSizeIocIntSts = 0;

                  /************************************************************* 
                   *  Fixing MC syncronisation problem.
                   *  Added for shure that all data memory transactions finished
                   **************************************************************/
                  tmp_reg_val = ep_queue_head_ptr->dwSizeIocIntSts;
                  
                  /* Prime the Endpoint */
                  
                  __raw_writel(bit_pos, WPCM450_UDC_ENDPTPRIME_REG(usb_dev_ptr->DEV_NUM));
               }

         #else   /*workaround old method */
                   
          /* Start CR 1015 */
         /* Prime the Endpoint */

         /************************************************************* 
          *  Fixing MC syncronisation problem.
          *  Added for shure that all data memory transactions finished
          **************************************************************/
         tmp_reg_val = ep_queue_head_ptr->dwSizeIocIntSts;
         
         __raw_writel(bit_pos, WPCM450_UDC_ENDPTPRIME_REG(usb_dev_ptr->DEV_NUM));
         
         if (!(__raw_readl(WPCM450_UDC_ENDPTSTATUS_REG(usb_dev_ptr->DEV_NUM)) & bit_pos)) 
         {
   
                     /* old workaround will be compiled */
                     while (__raw_readl(WPCM450_UDC_ENDPTPRIME_REG(usb_dev_ptr->DEV_NUM)) & bit_pos) {
                        /* Wait for the ENDPTPRIME to go to zero */
                     } /* EndWhile */

                     if (__raw_readl(WPCM450_UDC_ENDPTSTATUS_REG(usb_dev_ptr->DEV_NUM)) & bit_pos) 
                     {
                        /* The endpoint was not not primed so no other transfers on 
                        ** the queue 
                        */
                          goto done;
                     } /* Endif */
      
         } else {
            goto done;
         } /* Endif */

         /* No other transfers on the queue */
         ep_queue_head_ptr->dwNextDtdPtr = (__u32)first_dTD_ptr & NOMMU;
         ep_queue_head_ptr->dwSizeIocIntSts = 0;

         /************************************************************* 
          *  Fixing MC syncronisation problem.
          *  Added for shure that all data memory transactions finished
          **************************************************************/
         tmp_reg_val = ep_queue_head_ptr->dwSizeIocIntSts;
         
         /* Prime the Endpoint */
         __raw_writel(bit_pos, WPCM450_UDC_ENDPTPRIME_REG(usb_dev_ptr->DEV_NUM));
                  
         #endif

   }
   else
   {
         /* No other transfers on the queue */
         ep_queue_head_ptr->dwNextDtdPtr = (__u32)first_dTD_ptr & NOMMU;
         ep_queue_head_ptr->dwSizeIocIntSts = 0;
       #ifdef _USB_CACHE_USE_
         flush_cache_all();//zmsong
       #endif  
         /* Prime the Endpoint */

       /************************************************************* 
        *  Fixing MC syncronisation problem.
        *  Added for shure that all data memory transactions finished
        **************************************************************/
       tmp_reg_val = ep_queue_head_ptr->dwSizeIocIntSts;
       
       __raw_writel(bit_pos, WPCM450_UDC_ENDPTPRIME_REG(usb_dev_ptr->DEV_NUM));
      
   }

   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_add_dTD, SUCCESSFUL");
   #endif

done:
   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_add_dTD, SUCCESSFUL");
   #endif
   return USB_OK;
   /* End CR 1015 */
} /* EndBody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_dci_vusb20_send_data
*  Returned Value : USB_OK or error code
*  Comments       :
*        Sends data by adding and executing the dTD. Non-blocking.
*
*END*-----------------------------------------------------------------*/
__u8 _usb_dci_vusb20_send_data
   (
      /* [IN] the USB_dev_initialize state structure */
      _usb_device_handle         handle,
     
      /* [IN] The transfer descriptor address */
      xd_struct_ptr_t              xd_ptr
   )
{ /* Body */

   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_send_data, SUCCESSFUL");
   #endif

   /* Add and execute the device transfer descriptor */
   return(_usb_dci_vusb20_add_dTD(handle, xd_ptr));
} /* EndBody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_dci_vusb20_recv_data
*  Returned Value : USB_OK or error code
*  Comments       :
*        Receives data by adding and executing the dTD. Non-blocking.
*
*END*-----------------------------------------------------------------*/
__u8 _usb_dci_vusb20_recv_data
   (
      /* [IN] the USB_dev_initialize state structure */
      _usb_device_handle         handle,
     
      /* [IN] The transfer descriptor address */
      xd_struct_ptr_t              xd_ptr
   )
{ /* Body */

   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_recv_data, SUCCESSFUL");
   #endif
   /* Add and execute the device transfer descriptor */
   return(_usb_dci_vusb20_add_dTD(handle, xd_ptr));
} /* EndBody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_dci_vusb20_process_tr_complete
*  Returned Value : None
*  Comments       :
*        Services transaction complete interrupt
*
*END*-----------------------------------------------------------------*/
void _usb_dci_vusb20_process_tr_complete
   (
      /* [IN] the USB_dev_initialize state structure */
      _usb_device_handle         handle
   )
{ /* Body */
   USB_DEV_STATE_STRUCT_PTR usb_dev_ptr;
   EpTrStructPtr_t dTD_ptr; 
   EpTrStructPtr_t temp_dTD_ptr;
   EpQueueHeadStructPtr_t ep_queue_head_ptr;
   __u32 temp;
   __u32 i;
   __u32 bEpNum;
   __u32 direction;
   __u32 bit_pos;
   __u32 remaining_length = 0;
   __u32 actual_transfer_length = 0;
   __u32 errors = 0;
   xd_struct_ptr_t xd_ptr;
   xd_struct_ptr_t temp_xd_ptr = NULL;
   __u8* buff_start_address = NULL;
   boolean endpoint_detected = FALSE;
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;
   
   
   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_process_tr_complete");
   #endif
   
   /* We use separate loops for ENDPTSETUPSTAT and ENDPTCOMPLETE because the 
   ** setup packets are to be read ASAP 
   */
   
   /* Process all Setup packet received interrupts */
   bit_pos = __raw_readl(WPCM450_UDC_ENPDTSETUPSTAT_REG(usb_dev_ptr->DEV_NUM));
   
   if (bit_pos) 
   {
      for (i=0;i<USB_MAX_CONTROL_ENDPOINTS;i++)
      {
         if (bit_pos & (1 << i))
         {
            _usb_device_call_service(
                  handle,
                  i,
                  TRUE,
                  0,
                  0,
                  8,
                  0);
            
         } /* Endif */
      } /* Endfor */
   } /* Endif */
   
   /* Don't clear the endpoint setup status register here. It is cleared as a 
   ** setup packet is read out of the buffer 
   */

   /* Process non-setup transaction complete interrupts */   
   bit_pos = __raw_readl(WPCM450_UDC_ENDPTCOMPLETE_REG(usb_dev_ptr->DEV_NUM));

   /* Clear the bits in the register */
   __raw_writel(bit_pos, WPCM450_UDC_ENDPTCOMPLETE_REG(usb_dev_ptr->DEV_NUM));

  
   
   if (bit_pos) 
   {
      /* Get the endpoint number and the direction of transfer */
      
      for (i=0;i<USB_MAX_ENDPOINTS*2;i++)
      {
         endpoint_detected = FALSE;

         if ((i < USB_MAX_ENDPOINTS) && (bit_pos & (1 << i)))
         {
            bEpNum = i;
            direction = 0;
            endpoint_detected = TRUE;
         }
         else if((i >= USB_MAX_ENDPOINTS) && (bit_pos & (1 << (i+16-USB_MAX_ENDPOINTS))))
         {
            bEpNum = (i - USB_MAX_ENDPOINTS);
            direction = 1;
            endpoint_detected = TRUE;
            usb_dbg_cnt--;
         }

        
         if(endpoint_detected)
         {
 
            temp = (2*bEpNum + direction);

            /* Get the first dTD */      
            dTD_ptr = usb_dev_ptr->EP_DTD_HEADS[temp];
            
            ep_queue_head_ptr = (EpQueueHeadStructPtr_t)__raw_readl(WPCM450_UDC_ENDPOINTLISTADDR_REG(usb_dev_ptr->DEV_NUM)) + temp;

            ep_queue_head_ptr = (EpQueueHeadStructPtr_t)((__u32)ep_queue_head_ptr | _NOMMU);
            
            /* Process all the dTDs for respective transfers */
            while ((__u32)dTD_ptr & NOMMU) 
            {
               if (dTD_ptr->dwSizeIocSts & VUSBHS_TD_STATUS_ACTIVE) 
               {
                  /* No more dTDs to process. Next one is owned by VUSB */
                  break;
               } /* Endif */
               
               /* Get the correct internal transfer descriptor */
               xd_ptr = (xd_struct_ptr_t)dTD_ptr->wScratchPtr->pXdForThisDtd;
               if (xd_ptr)
               {
                  buff_start_address = xd_ptr->wStartaddress;
                  actual_transfer_length = xd_ptr->wToatalLength;
                  temp_xd_ptr = xd_ptr;
               } /* Endif */
               
               /* Get the address of the next dTD */
               temp_dTD_ptr = (EpTrStructPtr_t)(dTD_ptr->dwNextTrElemPtr & VUSBHS_TD_ADDR_MASK);

               temp_dTD_ptr = (EpTrStructPtr_t)((__u32)temp_dTD_ptr | _NOMMU);   
               
               /* Read the errors */
               errors = (dTD_ptr->dwSizeIocSts & VUSBHS_TD_ERROR_MASK);
                  
               if (!errors) 
               {
                  /* No errors */
                  /* Get the length of transfer from the current dTD */   
                  remaining_length += ((dTD_ptr->dwSizeIocSts & VUSB_EP_TR_PACKET_SIZE) >> 16);
                  actual_transfer_length -= remaining_length;
               }
               else 
               {
                  if (errors & VUSBHS_TD_STATUS_HALTED)
                  {
                     /* Clear the errors and Halt condition */
                     ep_queue_head_ptr->dwSizeIocIntSts &= ~errors;
                  } /* Endif */
               } /* Endif */

              USB_lock();
               /* Retire the processed dTD */
               _usb_dci_vusb20_cancel_transfer(handle, bEpNum, direction);

              USB_unlock();
              
               if ((__u32)temp_dTD_ptr & NOMMU)
               {
                  if ((__u32)temp_dTD_ptr->wScratchPtr->\
                     pXdForThisDtd != (__u32)temp_xd_ptr) 
                  {
                     /* Transfer complete. Call the register service function for the 
                     ** endpoint 
                     */
                     
                     _usb_device_call_service(
                          handle,
                          bEpNum,
                          FALSE,
                          direction, 
                          buff_start_address,
                          actual_transfer_length,
                          errors);
                     
                     remaining_length = 0;
                  } /* Endif */
               }
               else
               {
                  /* Transfer complete. Call the register service function for the 
                  ** endpoint 
                  */
                  
                  _usb_device_call_service(
                        handle,
                        bEpNum,
                        FALSE,
                        direction, 
                        buff_start_address,
                        actual_transfer_length,
                        errors);
                  
               } /* Endif */
               
               dTD_ptr = temp_dTD_ptr;
               errors = 0;
               
            } /* Endwhile */
         } /* Endif */
      } /* Endfor */
   } /* Endif */

     
   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_process_tr_complete, SUCCESSFUL");
   #endif
   
} /* EndBody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_dci_vusb20_isr
*  Returned Value : None
*  Comments       :
*        Services all the VUSB_HS interrupt sources
*
*END*-----------------------------------------------------------------*/

static irqreturn_t  _usb_dci_vusb20_isr (int irq, void *dev, struct pt_regs * r)
{ /* Body */
   USB_DEV_STATE_STRUCT_PTR                     usb_dev_ptr;
   __u32                                      status;
   __u32 Vector;  // Changed by WECI
   __u32 aic_isnr;  // Changed by WECI



   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)dev;


   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_isr\n");
   #endif


   
   for (;;) {
      status = __raw_readl(WPCM450_UDC_USBSTS_REG(usb_dev_ptr->DEV_NUM));
      
      if (!(status & __raw_readl(WPCM450_UDC_USBINTR_REG(usb_dev_ptr->DEV_NUM)))) {
         //printk("_usb_dci_vusb20_isr break\n");
         break;
      } /* Endif */

      /* Clear all the interrupts occured */
      __raw_writel(status,WPCM450_UDC_USBSTS_REG(usb_dev_ptr->DEV_NUM));

      if (status & EHCI_STS_RESET) {
         //printk("ehci_sts_reset\n");
         _usb_dci_vusb20_process_reset((void*)usb_dev_ptr);
      } /* Endif */

     
      if (status & EHCI_STS_PORT_CHANGE) {
         //printk("ehci_sts_port_change\n");
         _usb_dci_vusb20_process_port_change((void*)usb_dev_ptr);
      } /* Endif */
     

     
      if (status & EHCI_STS_ERR) {
         //printk("ehci_sts_err\n");
         _usb_dci_vusb20_process_error((void*)usb_dev_ptr);
      } /* Endif */
    

     
      if (status & EHCI_STS_SOF) {
         //printk("ehci_sts_sof\n");
         _usb_dci_vusb20_process_SOF((void*)usb_dev_ptr);
      } /* Endif */
     
         
      if (status & EHCI_STS_INT) {
         //printk("ehci_sts_int\n");
         _usb_dci_vusb20_process_tr_complete((void*)usb_dev_ptr);
      } /* Endif */

      
      if (status & EHCI_STS_SUSPEND) {
         //printk("ehci_sts_suspend\n");
         _usb_dci_vusb20_process_suspend((void*)usb_dev_ptr);
         return IRQ_HANDLED;
      } /* Endif */
      
      
   } /* Endfor */

   
   
   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_isr, SUCCESSFUL");
   #endif

   
   return IRQ_HANDLED;

} /* EndBody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_dci_vusb20_process_reset
*  Returned Value : None
*  Comments       :
*        Services reset interrupt
*
*END*-----------------------------------------------------------------*/
void _usb_dci_vusb20_process_reset
   (
      /* [IN] the USB_dev_initialize state structure */
      _usb_device_handle         handle
   )
{ /* Body */
   USB_DEV_STATE_STRUCT_PTR     usb_dev_ptr;
   __u32                        temp;
   __u8                         i;

   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_process_reset");
   #endif
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;

   usb_dev_ptr->USB_STATE = USB_STATE_UNKNOWN;
   
   /* Inform the application so that it can cancel all previously queued transfers */
   _usb_device_call_service(usb_dev_ptr, USB_SERVICE_BUS_RESET, 0, 0, 0, 0, 0);
   
   /* The address bits are past bit 25-31. Set the address */
   __raw_writel(__raw_readl(WPCM450_UDC_USBDEVICEADDR_REG(usb_dev_ptr->DEV_NUM))&~0xFE000000, WPCM450_UDC_USBDEVICEADDR_REG(usb_dev_ptr->DEV_NUM));
   
   /* Clear all the setup token semaphores */
   temp = __raw_readl(WPCM450_UDC_ENPDTSETUPSTAT_REG(usb_dev_ptr->DEV_NUM));
   __raw_writel(temp,WPCM450_UDC_ENPDTSETUPSTAT_REG(usb_dev_ptr->DEV_NUM));
  

   /* Clear all the endpoint complete status bits */   
   temp = __raw_readl(WPCM450_UDC_ENDPTCOMPLETE_REG(usb_dev_ptr->DEV_NUM));
   __raw_writel(temp, WPCM450_UDC_ENDPTCOMPLETE_REG(usb_dev_ptr->DEV_NUM));
   
   while (__raw_readl(WPCM450_UDC_ENDPTPRIME_REG(usb_dev_ptr->DEV_NUM)) & 0xFFFFFFFF) {
     
      /* Wait until all ENDPTPRIME bits cleared */
   } /* Endif */

   /* Write 1s to the Flush register */
   __raw_writel(0xFFFFFFFF,WPCM450_UDC_ENDPTFLUSH_REG(usb_dev_ptr->DEV_NUM));

   /* Wait until flushing completed */
   while (__raw_readl(WPCM450_UDC_ENDPTFLUSH_REG(usb_dev_ptr->DEV_NUM)) & 0xFFFFFFFF)
   {
       /* ENDPTFLUSH bit should be cleared to indicate this 
       ** operation is complete 
        */
   } /* EndWhile */
         
   /* Unstall all endpoints */
   for (i=0;i<usb_dev_ptr->MAX_ENDPOINTS;i++) {
      _usb_dci_vusb20_unstall_endpoint(handle, i, USB_RECV);
      _usb_dci_vusb20_unstall_endpoint(handle, i, USB_SEND);
   } /* Endfor */



   if (__raw_readl(WPCM450_UDC_PORTSC1_REG(usb_dev_ptr->DEV_NUM)) & 
      EHCI_PORTSCX_PORT_RESET) 
   {
      usb_dev_ptr->BUS_RESETTING = TRUE;
      usb_dev_ptr->USB_STATE = USB_STATE_POWERED;
   } 
   else
   { 
      /* re-initialize */      
      _usb_dci_vusb20_chip_initialize((_usb_device_handle)usb_dev_ptr);
      #ifdef _DEVICE_DEBUG_
         printk("_usb_dci_vusb20_process_reset, SUCCESSFUL reinit hw");
      #endif
      return;
   } /* Endif */

 
   _usb_device_call_service(usb_dev_ptr, USB_SERVICE_BUS_RESET, 0, 0, 0, 0, 0);

   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_process_reset, SUCCESSFUL");
   #endif
   
} /* EndBody */

#ifndef SUSPEND_INTERRUPT_DISABLE
/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_dci_vusb20_process_suspend
*  Returned Value : None
*  Comments       :
*        Services suspend interrupt
*
*END*-----------------------------------------------------------------*/
void _usb_dci_vusb20_process_suspend
   (
      /* [IN] the USB_dev_initialize state structure */
      _usb_device_handle         handle
   )
{ /* Body */
   USB_DEV_STATE_STRUCT_PTR                     usb_dev_ptr;
   
   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_process_suspend");
   #endif

   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;
   
   usb_dev_ptr->USB_DEV_STATE_B4_SUSPEND = usb_dev_ptr->USB_STATE;
   
   usb_dev_ptr->USB_STATE = USB_STATE_SUSPEND;

   /* Inform the upper layers */
   _usb_device_call_service(usb_dev_ptr, USB_SERVICE_SLEEP, 0, 0, 0, 0, 0);

   #ifdef __MISSING_RESUME_INTERRUPT_WORKAROUND__
   sof_after_suspend =0;
   #endif /* __MISSING_RESUME_INTERRUPT_WORKAROUND__ */

   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_process_suspend, SUCCESSFUL");
   #endif
   
} /* EndBody */
#endif

#ifndef SOF_INTERRUPT_DISABLE
/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_dci_vusb20_process_SOF
*  Returned Value : None
*  Comments       :
*        Services SOF interrupt
*
*END*-----------------------------------------------------------------*/
void _usb_dci_vusb20_process_SOF
   (
      /* [IN] the USB_dev_initialize state structure */
      _usb_device_handle         handle
   )
{ /* Body */
   USB_DEV_STATE_STRUCT_PTR                     usb_dev_ptr;
  

   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_process_SOF");
   #endif
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;
   

   /* Inform the upper layer */   
   _usb_device_call_service(
        usb_dev_ptr,
        USB_SERVICE_SOF,
        0,
        0,
        0, 
        __raw_readl(WPCM450_UDC_FRINDEX_REG(usb_dev_ptr->DEV_NUM)),
        0);

  #ifdef __MISSING_RESUME_INTERRUPT_WORKAROUND__
  #ifdef __USB_OTG__
   /* 10 SOFs after the suspend means that we should provide
   a resume callback */
   if ((usb_dev_ptr->USB_STATE == USB_STATE_SUSPEND) &&
      (sof_after_suspend > TB_A_SUSPEND))
   {
       usb_dev_ptr->USB_STATE = usb_dev_ptr->USB_DEV_STATE_B4_SUSPEND;
       /* Inform the upper layers */
       _usb_device_call_service(usb_dev_ptr, USB_SERVICE_RESUME, 0, 0, 0, 0, 0);
   }
   #endif
   #endif /* __MISSING_RESUME_INTERRUPT_WORKAROUND__ */
   

   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_process_SOF, SUCCESSFUL");
   #endif

} /* EndBody */
#endif

#ifndef PORT_CHANGE_INTERRUPT_DISABLE
/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_dci_vusb20_process_port_change
*  Returned Value : None
*  Comments       :
*        Services port change detect interrupt
*
*END*-----------------------------------------------------------------*/
void _usb_dci_vusb20_process_port_change
   (
      /* [IN] the USB_dev_initialize state structure */
      _usb_device_handle         handle
   )
{ /* Body */
   USB_DEV_STATE_STRUCT_PTR                     usb_dev_ptr;
   

   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_process_port_change");
   #endif
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;
   

   if (usb_dev_ptr->BUS_RESETTING) {
      /* Bus reset operation complete */
      usb_dev_ptr->BUS_RESETTING = FALSE;
   } /* Endif */

#ifdef __MISSING_RESUME_INTERRUPT_WORKAROUND__
   port_change_log[port_change_counter] = __raw_readl(WPCM450_UDC_PORTSC1_REG(usb_dev_ptr->DEV_NUM));
   port_change_counter++;
#endif /* __MISSING_RESUME_INTERRUPT_WORKAROUND__ */

   if (!(__raw_readl(WPCM450_UDC_PORTSC1_REG(usb_dev_ptr->DEV_NUM)) & 
         EHCI_PORTSCX_PORT_RESET)) 
   {
      /* Get the speed */
      if (__raw_readl(WPCM450_UDC_PORTSC1_REG(usb_dev_ptr->DEV_NUM)) & 
         EHCI_PORTSCX_PORT_HIGH_SPEED) 
      {
         usb_dev_ptr->SPEED = USB_SPEED_HIGH;
      } else {
         usb_dev_ptr->SPEED = USB_SPEED_LOW;
      } /* Endif */

      /* Inform the upper layers of the speed of operation */      
      _usb_device_call_service(usb_dev_ptr, USB_SERVICE_SPEED_DETECTION, 0, 0, 
         0, usb_dev_ptr->SPEED, 0);
   
   } /* Endif */
      
#ifndef SMALL_CODE_SIZE      
   if (__raw_readl(WPCM450_UDC_PORTSC1_REG(usb_dev_ptr->DEV_NUM)) & 
      EHCI_PORTSCX_PORT_SUSPEND) 
   {
   
      usb_dev_ptr->USB_DEV_STATE_B4_SUSPEND = usb_dev_ptr->USB_STATE;
      usb_dev_ptr->USB_STATE = USB_STATE_SUSPEND;

      /* Inform the upper layers */
      _usb_device_call_service(usb_dev_ptr, USB_SERVICE_SUSPEND, 0, 0, 0, 0, 0);
   } /* Endif */
#endif
   
   if (!(__raw_readl(WPCM450_UDC_PORTSC1_REG(usb_dev_ptr->DEV_NUM)) & 
      EHCI_PORTSCX_PORT_SUSPEND) && 
      (usb_dev_ptr->USB_STATE == USB_STATE_SUSPEND)) 
   {
      usb_dev_ptr->USB_STATE = usb_dev_ptr->USB_DEV_STATE_B4_SUSPEND;
      /* Inform the upper layers */
      _usb_device_call_service(usb_dev_ptr, USB_SERVICE_RESUME, 0, 0, 0, 0, 0);

      #ifdef _DEVICE_DEBUG_
         printk("_usb_dci_vusb20_process_port_change, SUCCESSFUL, resumed");
      #endif
      return;
   } /* Endif */

   usb_dev_ptr->USB_STATE = USB_STATE_DEFAULT;
   
   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_process_port_change, SUCCESSFUL");
   #endif

     
} /* EndBody */
#endif

#ifndef ERROR_INTERRUPT_DISABLE
/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_dci_vusb20_process_error
*  Returned Value : None
*  Comments       :
*        Services error interrupt
*
*END*-----------------------------------------------------------------*/
void _usb_dci_vusb20_process_error
   (
      /* [IN] the USB_dev_initialize state structure */
      _usb_device_handle         handle
   )
{ /* Body */
   USB_DEV_STATE_STRUCT_PTR                     usb_dev_ptr;
   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_process_error");
   #endif
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;
   
   /* Increment the error count */
   usb_dev_ptr->ERRORS++;

   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_process_error, SUCCESSFUL");
   #endif
   
} /* EndBody */
#endif
/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_dci_vusb20_set_speed_full
*  Returned Value : None
*  Comments       :
*        Force the controller port in full speed mode.
*
*END*-----------------------------------------------------------------*/
void _usb_dci_vusb20_set_speed_full
   (
      /* [IN] the USB_dev_initialize state structure */
      _usb_device_handle         handle,
     
      /* The port number on the device */
      __u8                     port_number
   )
{ /* Body */
   USB_DEV_STATE_STRUCT_PTR                     usb_dev_ptr;
   
   __u32                                      port_control;
   
   
   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_set_speed_full");
   #endif
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;
   

   port_control = __raw_readl(WPCM450_UDC_PORTSC1_REG(usb_dev_ptr->DEV_NUM) + port_number*sizeof(__u32));   
   port_control |= EHCI_PORTSCX_FORCE_FULL_SPEED_CONNECT;
   __raw_writel(port_control,WPCM450_UDC_PORTSC1_REG(usb_dev_ptr->DEV_NUM) + port_number*sizeof(__u32));

   
   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_set_speed_full, SUCCESSFUL");
   #endif
   
} /* EndBody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_dci_vusb20_suspend_phy
*  Returned Value : None
*  Comments       :
*        Suspends the PHY in low power mode
*
*END*-----------------------------------------------------------------*/
void _usb_dci_vusb20_suspend_phy
   (
      /* [IN] the USB_dev_initialize state structure */
      _usb_device_handle         handle,
     
      /* The port number on the device */
      __u8                     port_number
   )
{ /* Body */
   USB_DEV_STATE_STRUCT_PTR                     usb_dev_ptr;
   __u32                                      port_control;
   
   
   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_set_speed_full");
   #endif
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;
   

   port_control = __raw_readl(WPCM450_UDC_PORTSC1_REG(usb_dev_ptr->DEV_NUM) + port_number*sizeof(__u32));   
   port_control |= EHCI_PORTSCX_PHY_CLOCK_DISABLE;
   __raw_writel(port_control,WPCM450_UDC_PORTSC1_REG(usb_dev_ptr->DEV_NUM) + port_number*sizeof(__u32));
   
      
   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_set_speed_full, SUCCESSFUL");
   #endif
   
} /* EndBody */



/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_dci_vusb20_set_address
*  Returned Value : None
*  Comments       :
*        Sets the newly assigned device address
*
*END*-----------------------------------------------------------------*/
void _usb_dci_vusb20_set_address
   (
      /* [IN] the USB_dev_initialize state structure */
      _usb_device_handle         handle,
     
      /* Address of the device assigned by the host */
      __u8                     address
   )
{ /* Body */
   USB_DEV_STATE_STRUCT_PTR                     usb_dev_ptr;
  
   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_set_address");
   #endif
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;

   
   #ifdef SET_ADDRESS_HARDWARE_ASSISTANCE   
   /***********************************************************
   Hardware Rev 4.0 onwards have special assistance built in
   for handling the set_address command. As per the USB specs
   a device should be able to receive the response on a new
   address, within 2 msecs after status phase of set_address is
   completed. Since 2 mili second may be a very small time window
   (on high interrupt latency systems) before software could 
   come to the code below and write the device register,
   this routine will be called in advance when status phase of
   set_address is still not finished. The following line in the
   code will set the bit 24 to '1' and hardware will take
   the address and queue it in an internal buffer. From which
   it will use it to decode the next USB token. Please look
   at hardware rev details for the implementation of this
   assistance.
   
   Also note that writing bit 24 to 0x01 will not break
   any old hardware revs because it was an unused bit.
   ***********************************************************/
   /* The address bits are past bit 25-31. Set the address 
   also set the bit 24 to 0x01 to start hardware assitance*/
   __raw_writel(
     ((__u32)address << VUSBHS_ADDRESS_BIT_SHIFT) | (0x01 << (VUSBHS_ADDRESS_BIT_SHIFT -1)),
     WPCM450_UDC_USBDEVICEADDR_REG(usb_dev_ptr->DEV_NUM)); 
   #else
   __raw_writel(
      (__u32)address << VUSBHS_ADDRESS_BIT_SHIFT,WPCM450_UDC_USBDEVICEADDR_REG(usb_dev_ptr->DEV_NUM)); 
   #endif
   
   usb_dev_ptr->USB_STATE = USB_STATE_ADDRESS;
   
   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_set_address, SUCCESSFUL");
   #endif
   
} /* EndBody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_dci_vusb20_get_setup_data
*  Returned Value : None
*  Comments       :
*        Reads the Setup data from the 8-byte setup buffer
*
*END*-----------------------------------------------------------------*/
void _usb_dci_vusb20_get_setup_data
   (
      /* [IN] the USB_dev_initialize state structure */
      _usb_device_handle         handle,
            
      /* [IN] the Endpoint number */
      __u8                     bEpNum,
            
      /* [OUT] address of the buffer to read the setup data into */
      __u8*                  buffer_ptr
   )
{ /* Body */
   USB_DEV_STATE_STRUCT_PTR                     usb_dev_ptr;
   EpQueueHeadStructPtr_t              ep_queue_head_ptr;
   int i;
   #ifdef TRIP_WIRE
   boolean                                      read_safe;
   #endif

   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_get_setup_data");
   #endif
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;

   /* Get the endpoint queue head */
   ep_queue_head_ptr = (EpQueueHeadStructPtr_t)
      __raw_readl(WPCM450_UDC_ENDPOINTLISTADDR_REG(usb_dev_ptr->DEV_NUM)) + 2*bEpNum + 
      USB_RECV;
   
   ep_queue_head_ptr = (EpQueueHeadStructPtr_t)((__u32)ep_queue_head_ptr | _NOMMU);
   
   /********************************************************************
   CR 1219. Hardware versions 2.3+ have a implementation of tripwire 
   semaphore mechanism that requires that we read the contents of 
   QH safely by using the semaphore. Read the USBHS document to under
   stand how the code uses the semaphore mechanism. The following are
   the steps in brief
   
   1. USBCMD Write ‘1’ to Setup Tripwire in register.
   2. Duplicate contents of dQH.StatusBuffer into local software byte
      array.
   3  Read Setup TripWire in register. (if set - continue; if
      cleared goto 1.)
   4. Write '0' to clear Setup Tripwire in register.
   5. Process setup packet using local software byte array copy and
      execute status/handshake phases.
   
           
   ********************************************************************/
  #ifdef TRIP_WIRE  /*if semaphore mechanism is used the following code
                     is compiled in*/
  read_safe = FALSE;                      
  while(!read_safe)
  {
      
      
      /*********************************************************
      start with setting the semaphores
      *********************************************************/

      __raw_writel(__raw_readl(WPCM450_UDC_USBCMD_REG(usb_dev_ptr->DEV_NUM)) | EHCI_CMD_SETUP_TRIPWIRE_SET , WPCM450_UDC_USBCMD_REG(usb_dev_ptr->DEV_NUM));
      

      /*********************************************************
      Duplicate the contents of SETUP buffer to our buffer
      *********************************************************/
        #ifdef _USB_CACHE_USE_
        for(i=0; i <8; i++)
        {
              *(buffer_ptr + i) = ep_queue_head_ptr->dwSetupBuffer[i];
        }
        #else    
         /* Copy the setup packet to private buffer */
         USB_memcopy((__u8*)ep_queue_head_ptr->dwSetupBuffer, buffer_ptr, 8);
        #endif 
      /*********************************************************
      If setup tripwire semaphore is cleared by hardware it means
      that we have a danger and we need to restart.
      else we can exit out of loop safely.
      *********************************************************/
      if(__raw_readl(WPCM450_UDC_USBCMD_REG(usb_dev_ptr->DEV_NUM)) & EHCI_CMD_SETUP_TRIPWIRE_SET)
      {
          read_safe = TRUE; /* we can proceed exiting out of loop*/
      }
  }
  
   /*********************************************************
   Clear the semaphore bit now
   *********************************************************/

  __raw_writel(
        __raw_readl(WPCM450_UDC_USBCMD_REG(usb_dev_ptr->DEV_NUM)) & EHCI_CMD_SETUP_TRIPWIRE_CLEAR ,
        WPCM450_UDC_USBCMD_REG(usb_dev_ptr->DEV_NUM)); 

  
  #else   /*when semaphore is not used */    
     #ifdef _USB_CACHE_USE_
        for(int i=0; i <8; i++)
        {
           *(buffer_ptr + i) = ep_queue_head_ptr->dwSetupBuffer[i];
        }
     #else    
      /* Copy the setup packet to private buffer */
      USB_memcopy((__u8*)ep_queue_head_ptr->dwSetupBuffer, buffer_ptr, 8);
     #endif 
  #endif   
  
   /* Clear the bit in the ENDPTSETUPSTAT */
   __raw_writel((1 << bEpNum), WPCM450_UDC_ENPDTSETUPSTAT_REG(usb_dev_ptr->DEV_NUM));

   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_get_setup_data, SUCCESSFUL");
   #endif
   
} /* EndBody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_dci_vusb20_init_endpoint
*  Returned Value : None
*  Comments       :
*        Initializes the specified endpoint and the endpoint queue head
*
*END*-----------------------------------------------------------------*/
__u8 _usb_dci_vusb20_init_endpoint
   (
      /* [IN] the USB_dev_initialize state structure */
      _usb_device_handle         handle,
            
      /* [IN] the transaction descriptor address */
      xd_struct_ptr_t              xd_ptr
   )
{ /* Body */
   USB_DEV_STATE_STRUCT_PTR                  usb_dev_ptr;
   EpQueueHeadStructPtr_t           ep_queue_head_ptr;
   volatile __u32                            bit_pos;
   volatile __u32                            reg_val;
   volatile __u32                            reg_addr;

   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_init_endpoint");
   #endif
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;
   

   /* Get the endpoint queue head address */
   ep_queue_head_ptr = (EpQueueHeadStructPtr_t)
      __raw_readl(WPCM450_UDC_ENDPOINTLISTADDR_REG(usb_dev_ptr->DEV_NUM)) + 
      2*xd_ptr->bEpNum + xd_ptr->bDirection;

   ep_queue_head_ptr = (EpQueueHeadStructPtr_t)((__u32)ep_queue_head_ptr | _NOMMU);
      
   bit_pos = (1 << (16 * xd_ptr->bDirection + xd_ptr->bEpNum));
   
   /* Check if the Endpoint is Primed */
   if ((!(__raw_readl(WPCM450_UDC_ENDPTPRIME_REG(usb_dev_ptr->DEV_NUM)) & bit_pos)) && 
      (!(__raw_readl(WPCM450_UDC_ENDPTSTATUS_REG(usb_dev_ptr->DEV_NUM)) & bit_pos))) 
   { 
      /* Set the max packet length, interrupt on Setup and Mult fields */
      if (xd_ptr->bEpType == USB_ISOCHRONOUS_ENDPOINT) 
      {
         /* Mult bit should be set for isochronous endpoints */
         ep_queue_head_ptr->dwMaxPktLength = ((xd_ptr->wMaxpacketSize << 16) | 
            ((xd_ptr->bMaxPktsPerUframe ?  xd_ptr->bMaxPktsPerUframe : 1) << 
            VUSB_EP_QUEUE_HEAD_MULT_POS));
      }
      else
      {
         if (xd_ptr->bEpType != USB_CONTROL_ENDPOINT)
         {
            ep_queue_head_ptr->dwMaxPktLength = ((xd_ptr->wMaxpacketSize << 16) | 
               (xd_ptr->bDontZeroTerminate ? 
               VUSB_EP_QUEUE_HEAD_ZERO_LEN_TER_SEL : 0));
         }
         else
         {
            ep_queue_head_ptr->dwMaxPktLength = ((xd_ptr->wMaxpacketSize << 16) | 
               VUSB_EP_QUEUE_HEAD_IOS);
         } /* Endif */
      } /* Endif */
      
      /* Enable the endpoint for Rx and Tx and set the endpoint type */

     reg_val = __raw_readl(WPCM450_UDC_ENDPTCTRL0_REG(usb_dev_ptr->DEV_NUM)) |
         ((xd_ptr->bDirection ? (EHCI_EPCTRL_TX_ENABLE | EHCI_EPCTRL_TX_DATA_TOGGLE_RST) : (EHCI_EPCTRL_RX_ENABLE | EHCI_EPCTRL_RX_DATA_TOGGLE_RST)) | 
         (xd_ptr->bEpType << (xd_ptr->bDirection ? EHCI_EPCTRL_TX_EP_TYPE_SHIFT : EHCI_EPCTRL_RX_EP_TYPE_SHIFT)));

     reg_addr = (WPCM450_UDC_ENDPTCTRL0_REG(usb_dev_ptr->DEV_NUM) + xd_ptr->bEpNum*sizeof(__u32));
      
      __raw_writel(reg_val,reg_addr);
   }
   else 
   { 
      #ifdef _DEVICE_DEBUG_
         printk("_usb_dci_vusb20_init_endpoint, error ep init");
      #endif
      return USBERR_EP_INIT_FAILED;
   } /* Endif */
      
   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_init_endpoint, SUCCESSFUL");
   #endif

   return USB_OK;
   
} /* EndBody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_dci_vusb20_get_transfer_status
*  Returned Value : USB_OK or error code
*  Comments       :
*        Gets the status of a transfer
*
*END*-----------------------------------------------------------------*/
__u8 _usb_dci_vusb20_get_transfer_status
   (
      /* [IN] the USB_dev_initialize state structure */
      _usb_device_handle         handle,
     
      /* [IN] the Endpoint number */
      __u8                     bEpNum,
            
      /* [IN] direction */
      __u8                     direction
   )
{ /* Body */
   USB_DEV_STATE_STRUCT_PTR                     usb_dev_ptr;
   EpTrStructPtr_t                      dTD_ptr;
   xd_struct_ptr_t                                xd_ptr;
   __u8                                       status;
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;

   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_get_transfer_status");
   #endif
   
   /* Unlink the dTD */
   dTD_ptr = usb_dev_ptr->EP_DTD_HEADS[2*bEpNum + direction];


   if ((__u32)dTD_ptr & NOMMU) {
      /* Get the transfer descriptor for the dTD */
      xd_ptr = (xd_struct_ptr_t)dTD_ptr->wScratchPtr->pXdForThisDtd;
      status = xd_ptr->bStatus;
   } else {
      status = USB_STATUS_IDLE;
   } /* Endif */
   
   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_get_transfer_status, SUCCESSFUL");
   #endif

   return (status);

} /* EndBody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_dci_vusb20_get_transfer_details
*  Returned Value : void* to structure that has details for transfer
*        Gets the status of a transfer
*
*END*-----------------------------------------------------------------*/
xd_struct_ptr_t  _usb_dci_vusb20_get_transfer_details
   (
      /* [IN] the USB_dev_initialize state structure */
      _usb_device_handle         handle,
     
      /* [IN] the Endpoint number */
      __u8                     bEpNum,
            
      /* [IN] direction */
      __u8                     direction
   )
{ /* Body */
   USB_DEV_STATE_STRUCT_PTR                     usb_dev_ptr;
   EpTrStructPtr_t                      dTD_ptr, temp_dTD_ptr;
   xd_struct_ptr_t                                xd_ptr;
   __u32                                      temp, remaining_bytes;
   EpQueueHeadStructPtr_t              ep_queue_head_ptr;

   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;

   temp = (2*bEpNum + direction);

   /* get a void* to QH for this endpoint */   
   ep_queue_head_ptr = (EpQueueHeadStructPtr_t)
      __raw_readl(WPCM450_UDC_ENDPOINTLISTADDR_REG(usb_dev_ptr->DEV_NUM)) + 
      temp;

   ep_queue_head_ptr = (EpQueueHeadStructPtr_t)((__u32)ep_queue_head_ptr | _NOMMU);
   
   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_get_transfer_status");
   #endif
   
   /* Unlink the dTD */
   dTD_ptr =usb_dev_ptr->EP_DTD_HEADS[2*bEpNum + direction];

   if ((__u32)dTD_ptr & NOMMU) 
   {
      
      /* Get the transfer descriptor for the dTD */
      xd_ptr = (xd_struct_ptr_t)dTD_ptr->wScratchPtr->pXdForThisDtd;
      if(!xd_ptr) return NULL;
      
      /* Initialize the transfer length field */
      xd_ptr->wSoFar =0;
      remaining_bytes =0;
      
      /*if length of this transfer is greater than 20K
      we have multiple DTDs to count */
      if(xd_ptr->wToatalLength > VUSB_EP_MAX_LENGTH_TRANSFER)
      {
         /* it is a valid DTD. We should parse all DTDs for this XD
         and find the total bytes used so far */
         temp_dTD_ptr = dTD_ptr;
      
         /*loop through the list of DTDS until an active DTD is found
         or list has finished */
         while(!(dTD_ptr->dwNextTrElemPtr & VUSBHS_TD_NEXT_TERMINATE))         
         {
            
            
            /**********************************************************
            If this DTD has been overlayed, we take the actual length 
            from QH.
            **********************************************************/

            if ((__u32)(ep_queue_head_ptr->dwCurrDtdPtr & VUSBHS_TD_ADDR_MASK) ==
                  (__u32)temp_dTD_ptr & NOMMU)
            {
                remaining_bytes += 
                  ((ep_queue_head_ptr->dwSizeIocIntSts & VUSB_EP_TR_PACKET_SIZE) >> 16);
            }
            else
            {
               /* take the length from DTD itself */
                remaining_bytes += 
                  ((temp_dTD_ptr->dwSizeIocSts & VUSB_EP_TR_PACKET_SIZE) >> 16);
            }
   
            dTD_ptr = temp_dTD_ptr;
             
            /* Get the address of the next dTD */
            temp_dTD_ptr = (EpTrStructPtr_t)
                  (temp_dTD_ptr->dwNextTrElemPtr & VUSBHS_TD_ADDR_MASK);
         }
         xd_ptr->wSoFar = xd_ptr->wToatalLength - remaining_bytes;

      }
      else
      {
         /*look at actual length from QH*/
         xd_ptr->wSoFar = xd_ptr->wToatalLength - 
            ((ep_queue_head_ptr->dwSizeIocIntSts & VUSB_EP_TR_PACKET_SIZE) >> 16);
         
      }
      
   } else {
      xd_ptr = NULL;
   } /* Endif */
   
   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_get_transfer_status, SUCCESSFUL");
   #endif

   return (xd_ptr);

} /* EndBody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_dci_vusb20_shutdown
*  Returned Value : None
*  Comments       :
*        Shuts down the VUSB_HS Device
*
*END*-----------------------------------------------------------------*/
void _usb_dci_vusb20_shutdown
   (
      /* [IN] the USB_dev_initialize state structure */
      _usb_device_handle         handle
   )
{ /* Body */
   USB_DEV_STATE_STRUCT_PTR  usb_dev_ptr;
   __u32                     tmp_reg_val;
   

   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_shutdown");
   #endif
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;

   
   
   /* Disable interrupts */

   __raw_writel(
    (__raw_readl(WPCM450_UDC_USBINTR_REG(usb_dev_ptr->DEV_NUM)) &
     ~(EHCI_INTR_INT_EN | EHCI_INTR_ERR_INT_EN |
     EHCI_INTR_PORT_CHANGE_DETECT_EN | EHCI_INTR_RESET_EN)),
     WPCM450_UDC_USBINTR_REG(usb_dev_ptr->DEV_NUM));

   USB_memfree((void*)usb_dev_ptr->DRIVER_MEMORY);

   #ifdef __USB_OTG__
      #ifdef HNP_HARDWARE_ASSISTANCE
      /********************************************************
      If we are shutting device control down under HNP, we should
      skip shutting down and resetting because it will be
      done by Host routines that will initialize the host..
      ********************************************************/
        if (usb_otg_state_struct_ptr->STATE_STRUCT_PTR->B_HNP_ENABLE == TRUE)
        {
           return;
        }

     #endif
   #endif

   /* Reset the Run the bit in the command register to stop VUSB */
   __raw_writel(
        (__raw_readl(WPCM450_UDC_USBCMD_REG(usb_dev_ptr->DEV_NUM)) & (~EHCI_CMD_RUN_STOP)),
        WPCM450_UDC_USBCMD_REG(usb_dev_ptr->DEV_NUM));
   
   tmp_reg_val = __raw_readl(WPCM450_GCR_MFSEL1);
   tmp_reg_val |= (0x1<<23); 
   __raw_writel(tmp_reg_val,WPCM450_GCR_MFSEL1);
  
   tmp_reg_val = __raw_readl(WPCM450_GCR_INTCR);
   tmp_reg_val &= (~(0x0<<13));
   __raw_writel(tmp_reg_val, WPCM450_GCR_INTCR);
   
    /* Reset the controller to get default values */
    __raw_writel(EHCI_CMD_CTRL_RESET, WPCM450_UDC_USBCMD_REG(usb_dev_ptr->DEV_NUM));
   
   
      #ifdef _DEVICE_DEBUG_
         printk("_usb_dci_vusb20_shutdown, SUCCESSFUL");
      #endif
   
   
} /* EndBody */


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_dci_vusb20_cancel_transfer
*  Returned Value : USB_OK or error code
*  Comments       :
*        Cancels a transfer
*
*END*-----------------------------------------------------------------*/
__u8 _usb_dci_vusb20_cancel_transfer
   (
      /* [IN] the USB_dev_initialize state structure */
      _usb_device_handle         handle,
     
      /* [IN] the Endpoint number */
      __u8                     bEpNum,
            
      /* [IN] direction */
      __u8                     direction
   )
{ /* Body */
   USB_DEV_STATE_STRUCT_PTR                     usb_dev_ptr;
   EpTrStructPtr_t                      dTD_ptr, check_dTD_ptr;
   EpQueueHeadStructPtr_t              ep_queue_head_ptr;
   xd_struct_ptr_t                                xd_ptr;
   __u32                                      temp, bit_pos;
   volatile __u32                      tmp_reg_val;
   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_cancel_transfer");
   #endif

      
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;
   
   
   bit_pos = (1 << (16 * direction + bEpNum));
   temp = (2*bEpNum + direction);
   
   ep_queue_head_ptr = (EpQueueHeadStructPtr_t)
      __raw_readl(WPCM450_UDC_ENDPOINTLISTADDR_REG(usb_dev_ptr->DEV_NUM)) + 
      temp;


   ep_queue_head_ptr = (EpQueueHeadStructPtr_t)((__u32)ep_queue_head_ptr | _NOMMU);
   
   /* Unlink the dTD */
   dTD_ptr = usb_dev_ptr->EP_DTD_HEADS[temp];
   
   if ((__u32)dTD_ptr & NOMMU) 
   {
      check_dTD_ptr = (EpTrStructPtr_t)((__u32)dTD_ptr->dwNextTrElemPtr & VUSBHS_TD_ADDR_MASK);
      
      if (dTD_ptr->dwSizeIocSts & VUSBHS_TD_STATUS_ACTIVE) 
      {
         /* Flushing will halt the pipe */
         /* Write 1 to the Flush register */
         __raw_writel(bit_pos, WPCM450_UDC_ENDPTFLUSH_REG(usb_dev_ptr->DEV_NUM));

                 
         /* Wait until flushing completed */
         while (__raw_readl(WPCM450_UDC_ENDPTFLUSH_REG(usb_dev_ptr->DEV_NUM)) & bit_pos)
         {
           /* ENDPTFLUSH bit should be cleared to indicate this 
            ** operation is complete 
            */
         } /* EndWhile */
         
         while (__raw_readl(WPCM450_UDC_ENDPTSTATUS_REG(usb_dev_ptr->DEV_NUM)) & bit_pos) 
         {
            /* Write 1 to the Flush register */
            __raw_writel(bit_pos,WPCM450_UDC_ENDPTFLUSH_REG(usb_dev_ptr->DEV_NUM));
         
            /* Wait until flushing completed */
            while (__raw_readl(WPCM450_UDC_ENDPTFLUSH_REG(usb_dev_ptr->DEV_NUM)) & bit_pos) 
            {
              /* ENDPTFLUSH bit should be cleared to indicate this 
               ** operation is complete 
               */
            } /* EndWhile */
         } /* EndWhile */
      } /* Endif */
      
      /* Retire the current dTD */
      dTD_ptr->dwSizeIocSts = 0;
      dTD_ptr->dwNextTrElemPtr = VUSBHS_TD_NEXT_TERMINATE;
      
      /* The transfer descriptor for this dTD */
      xd_ptr = (xd_struct_ptr_t)dTD_ptr->wScratchPtr->pXdForThisDtd;
      dTD_ptr->wScratchPtr->pPrivate = (void*)usb_dev_ptr;
      /* Free the dTD */
      _usb_dci_vusb20_free_dTD((void*)dTD_ptr);
      
      /* Update the dTD head and tail for specific endpoint/direction */
      if (!check_dTD_ptr) 
      {
         usb_dev_ptr->EP_DTD_HEADS[temp] = NULL;
         usb_dev_ptr->EP_DTD_TAILS[temp] = NULL;
         if (xd_ptr) {
            xd_ptr->wScratchPtr->pPrivate = (void*)usb_dev_ptr;
            /* Free the transfer descriptor */
            _usb_device_free_XD((void*)xd_ptr);
         } /* Endif */
         /* No other transfers on the queue */
         ep_queue_head_ptr->dwNextDtdPtr = VUSB_EP_QUEUE_HEAD_NEXT_TERMINATE;
         ep_queue_head_ptr->dwSizeIocIntSts = 0;
      }
      else
      {
         check_dTD_ptr = (EpTrStructPtr_t)((__u32)check_dTD_ptr | _NOMMU);
         usb_dev_ptr->EP_DTD_HEADS[temp] = check_dTD_ptr;
            
         if (xd_ptr) 
         {
            if ((__u32)check_dTD_ptr->wScratchPtr->\
               pXdForThisDtd != (__u32)xd_ptr) 
            {
               xd_ptr->wScratchPtr->pPrivate = (void*)usb_dev_ptr;
               /* Free the transfer descriptor */
               _usb_device_free_XD((void*)xd_ptr);
            } /* Endif */
         } /* Endif */
         
         if (check_dTD_ptr->dwSizeIocSts & VUSBHS_TD_STATUS_ACTIVE)
         {
            /* Start CR 1015 */
            /* Prime the Endpoint */
         #ifdef _USB_CACHE_USE_
            flush_cache_all();//zmsong
         #endif   

            /************************************************************* 
             *  Fixing MC syncronisation problem.
             *  Added for shure that all data memory transactions finished
             **************************************************************/
            tmp_reg_val = ep_queue_head_ptr->dwSizeIocIntSts;
           
            __raw_writel(bit_pos,WPCM450_UDC_ENDPTPRIME_REG(usb_dev_ptr->DEV_NUM));

            if (!(__raw_readl(WPCM450_UDC_ENDPTSTATUS_REG(usb_dev_ptr->DEV_NUM)) & bit_pos)) 
            {
               while (__raw_readl(WPCM450_UDC_ENDPTPRIME_REG(usb_dev_ptr->DEV_NUM)) & bit_pos)
               {
                  /* Wait for the ENDPTPRIME to go to zero */
               } /* EndWhile */

               if (__raw_readl(WPCM450_UDC_ENDPTSTATUS_REG(usb_dev_ptr->DEV_NUM)) & bit_pos) 
               {
                  /* The endpoint was not not primed so no other transfers on 
                  ** the queue 
                  */
                  goto done;
               } /* Endif */
            } else {
               goto done;
            } /* Endif */

            /* No other transfers on the queue */
            ep_queue_head_ptr->dwNextDtdPtr = (__u32)check_dTD_ptr & NOMMU;
            ep_queue_head_ptr->dwSizeIocIntSts = 0;
           #ifdef _USB_CACHE_USE_
            flush_cache_all();//zmsong
           #endif
            /************************************************************* 
             *  Fixing MC syncronisation problem.
             *  Added for shure that all data memory transactions finished
             **************************************************************/
            tmp_reg_val = ep_queue_head_ptr->dwSizeIocIntSts;
           
            /* Prime the Endpoint */
            __raw_writel(bit_pos,WPCM450_UDC_ENDPTPRIME_REG(usb_dev_ptr->DEV_NUM));
         } /* Endif */
      } /* Endif */
   } /* Endif */
   
done:
   
   return USB_OK;

   /* End CR 1015 */

   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_cancel_transfer, SUCCESSFUL");
   #endif
   
} /* EndBody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_dci_vusb20_deinit_endpoint
*  Returned Value : None
*  Comments       :
*        Disables the specified endpoint and the endpoint queue head
*
*END*-----------------------------------------------------------------*/
__u8 _usb_dci_vusb20_deinit_endpoint
   (
      /* [IN] the USB_dev_initialize state structure */
      _usb_device_handle         handle,
            
      /* [IN] the Endpoint number */
      __u8                     bEpNum,
            
      /* [IN] direction */
      __u8                     direction
   )
{ /* Body */
   EpQueueHeadStruct_t *           ep_queue_head_ptr;
   __u32                           bit_pos;
   USB_DEV_STATE_STRUCT_PTR        usb_dev_ptr;
   
   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_deinit_endpoint");
   #endif
   

   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;
   
   /* Ger the endpoint queue head address */
   ep_queue_head_ptr = (EpQueueHeadStructPtr_t)
      __raw_readl(WPCM450_UDC_ENDPOINTLISTADDR_REG(usb_dev_ptr->DEV_NUM)) + 
      (2*bEpNum + direction);
      
   bit_pos = (1 << (16 * direction + bEpNum));
      
   /* Check if the Endpoint is Primed */
   if ((!(__raw_readl(WPCM450_UDC_ENDPTPRIME_REG(usb_dev_ptr->DEV_NUM)) & bit_pos)) && 
      (!(__raw_readl(WPCM450_UDC_ENDPTSTATUS_REG(usb_dev_ptr->DEV_NUM)) & bit_pos))) 
   { 
      /* Reset the max packet length and the interrupt on Setup */
      ep_queue_head_ptr->dwMaxPktLength = 0;
      
      /* Disable the endpoint for Rx or Tx and reset the endpoint type */

      __raw_writel(
          (__raw_readl(WPCM450_UDC_ENDPTCTRL0_REG(usb_dev_ptr->DEV_NUM) + bEpNum*sizeof(__u32)) &
          ((direction ? ~EHCI_EPCTRL_TX_ENABLE : ~EHCI_EPCTRL_RX_ENABLE) | 
          (direction ? ~EHCI_EPCTRL_TX_TYPE : ~EHCI_EPCTRL_RX_TYPE))),
          (WPCM450_UDC_ENDPTCTRL0_REG(usb_dev_ptr->DEV_NUM) + bEpNum*sizeof(__u32)));
   } else 
   { 
      #ifdef _DEVICE_DEBUG_
         printk("_usb_dci_vusb20_deinit_endpoint, error deinit failed");
      #endif
      return USBERR_EP_DEINIT_FAILED;
   } /* Endif */
      
   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_deinit_endpoint, SUCCESSFUL");
   #endif

   return USB_OK;
   
} /* EndBody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_dci_vusb20_assert_resume
*  Returned Value : None
*  Comments       :
*        Resume signalling for remote wakeup
*
*END*-----------------------------------------------------------------*/
void _usb_dci_vusb20_assert_resume
   (
      /* [IN] the USB_dev_initialize state structure */
      _usb_device_handle         handle
   )
{ /* Body */
   __u32                           temp;
   USB_DEV_STATE_STRUCT_PTR        usb_dev_ptr;
   
   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_assert_resume");
   #endif
   
  
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle; 

   /* Assert the Resume signal */   
   temp = __raw_readl(WPCM450_UDC_PORTSC1_REG(usb_dev_ptr->DEV_NUM));
   temp &= ~EHCI_PORTSCX_W1C_BITS;
   temp |= EHCI_PORTSCX_PORT_FORCE_RESUME;
   __raw_writel(temp,WPCM450_UDC_PORTSC1_REG(usb_dev_ptr->DEV_NUM));
   
   /* Port change interrupt will be asserted at the end of resume 
   ** operation 
   */

   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_assert_resume, SUCCESSFUL");
   #endif

} /* EndBody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_dci_vusb20_stall_endpoint
*  Returned Value : None
*  Comments       :
*        Stalls the specified endpoint
*
*END*-----------------------------------------------------------------*/
void _usb_dci_vusb20_stall_endpoint
   (
      /* [IN] the USB_dev_initialize state structure */
      _usb_device_handle         handle,
            
      /* [IN] the Endpoint number */
      __u8                     bEpNum,
            
      /* [IN] direction */
      __u8                     direction
   )
{ /* Body */
  
   EpQueueHeadStruct_t *            ep_queue_head_ptr;
   USB_DEV_STATE_STRUCT_PTR        usb_dev_ptr;
   
   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_stall_endpoint");
   #endif
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;  
   
   /* Get the endpoint queue head address */
   ep_queue_head_ptr = (EpQueueHeadStructPtr_t)
      __raw_readl(WPCM450_UDC_ENDPOINTLISTADDR_REG(usb_dev_ptr->DEV_NUM)) +
      2*bEpNum + direction;

   ep_queue_head_ptr = (EpQueueHeadStructPtr_t)((__u32)ep_queue_head_ptr | _NOMMU);
   
   /* Stall the endpoint for Rx or Tx and set the endpoint type */
   if (ep_queue_head_ptr->dwMaxPktLength & VUSB_EP_QUEUE_HEAD_IOS) {
      /* This is a control endpoint so STALL both directions */

      __raw_writel(
        (__raw_readl(WPCM450_UDC_ENDPTCTRL0_REG(usb_dev_ptr->DEV_NUM) + bEpNum*sizeof(__u32)) |
        (EHCI_EPCTRL_TX_EP_STALL | EHCI_EPCTRL_RX_EP_STALL)),
        (WPCM450_UDC_ENDPTCTRL0_REG(usb_dev_ptr->DEV_NUM) + bEpNum*sizeof(__u32)));
   } else {   
   
       __raw_writel(
        (__raw_readl(WPCM450_UDC_ENDPTCTRL0_REG(usb_dev_ptr->DEV_NUM) + bEpNum*sizeof(__u32)) |
        (direction ? EHCI_EPCTRL_TX_EP_STALL : 
        EHCI_EPCTRL_RX_EP_STALL)),
        (WPCM450_UDC_ENDPTCTRL0_REG(usb_dev_ptr->DEV_NUM) + bEpNum*sizeof(__u32)));
        
   } /* Endif */

   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_stall_endpoint, SUCCESSFUL");
   #endif
      
} /* EndBody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_dci_vusb20_unstall_endpoint
*  Returned Value : None
*  Comments       :
*        Unstall the specified endpoint in the specified direction
*
*END*-----------------------------------------------------------------*/
void _usb_dci_vusb20_unstall_endpoint
   (
      /* [IN] the USB_dev_initialize state structure */
      _usb_device_handle         handle,
            
      /* [IN] the Endpoint number */
      __u8                     bEpNum,
            
      /* [IN] direction */
      __u8                     direction
   )
{ /* Body */
  
   __u32 tmp_reg_val;
   USB_DEV_STATE_STRUCT_PTR        usb_dev_ptr;
   
   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_unstall_endpoint");
   #endif

   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;
   
   tmp_reg_val = __raw_readl(WPCM450_UDC_ENDPTCTRL0_REG(usb_dev_ptr->DEV_NUM) + bEpNum*sizeof(__u32));

   tmp_reg_val &= (direction ? (~EHCI_EPCTRL_TX_EP_STALL) : (~EHCI_EPCTRL_RX_EP_STALL));
   
   /* Enable the endpoint for Rx or Tx and set the endpoint type */
   __raw_writel(
         tmp_reg_val,
         (WPCM450_UDC_ENDPTCTRL0_REG(usb_dev_ptr->DEV_NUM) + bEpNum*sizeof(__u32)));
        
   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_unstall_endpoint, SUCCESSFUL");
   #endif
      
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : _usb_dci_vusb20_get_endpoint_status
* Returned Value : None
* Comments       :
*     Gets the endpoint status
* 
*END*--------------------------------------------------------------------*/
__u8 _usb_dci_vusb20_get_endpoint_status
   (
      /* [IN] Handle to the USB device */
      _usb_device_handle   handle,
      
      /* [IN] Endpoint to get */
      __u8               ep
   )
{ /* Body */
   
   __u8                            bEpNum = ep & 0x0f;
   __u8                            ep_dir = ep & 0x80; 
   USB_DEV_STATE_STRUCT_PTR        usb_dev_ptr;

   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_get_endpoint_status");
   #endif
   

   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;

   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_get_endpoint_status, SUCCESSFUL");
   #endif
   
   return ((__raw_readl(WPCM450_UDC_ENDPTCTRL0_REG(usb_dev_ptr->DEV_NUM) + bEpNum*sizeof(__u32)) & 
      (ep_dir ? EHCI_EPCTRL_TX_EP_STALL : EHCI_EPCTRL_RX_EP_STALL)) ? 1 : 0);

} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : _usb_dci_vusb20_set_test_mode
* Returned Value : None
* Comments       :
*     sets/resets the test mode
* 
*END*--------------------------------------------------------------------*/
void _usb_dci_vusb20_set_test_mode
   (
      /* [IN] Handle to the USB device */
      _usb_device_handle handle,
      
      /* [IN] Test mode */
      __u16 test_mode
   )
{ /* Body */
    
   __u32                           temp;
   USB_DEV_STATE_STRUCT_PTR        usb_dev_ptr;
   
   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_set_test_mode");
   #endif
   
  
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;
   
   temp = __raw_readl(WPCM450_UDC_ENDPTCTRL0_REG(usb_dev_ptr->DEV_NUM));

   __raw_writel(
      (temp | EHCI_EPCTRL_TX_DATA_TOGGLE_RST),
      WPCM450_UDC_ENDPTCTRL0_REG(usb_dev_ptr->DEV_NUM));

   if (test_mode == USB_TEST_MODE_TEST_PACKET) {
      _usb_device_send_data(handle, 0, test_packet, 
         USB_TEST_MODE_TEST_PACKET_LENGTH);
   } /* Endif */
   
   temp = __raw_readl(WPCM450_UDC_PORTSC1_REG(usb_dev_ptr->DEV_NUM));
   temp &= ~EHCI_PORTSCX_W1C_BITS;
   
   __raw_writel(
    (temp | ((__u32)test_mode << 8)),
    WPCM450_UDC_PORTSC1_REG(usb_dev_ptr->DEV_NUM));
      
   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_set_test_mode, SUCCESSFUL");
   #endif
      
 
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : _usb_dci_vusb20_set_endpoint_status
* Returned Value : None
* Comments       :
*     Sets the endpoint registers e.g. to enable TX, RX, control
* 
*END*--------------------------------------------------------------------*/
void _usb_dci_vusb20_set_endpoint_status
   (
      /* [IN] Handle to the USB device */
      _usb_device_handle   handle,
      
      /* [IN] Endpoint to set */
      __u8               ep,
      
      /* [IN] Endpoint characteristics */
      __u8               stall
   )
{ /* Body */

   USB_DEV_STATE_STRUCT_PTR        usb_dev_ptr;   

   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_set_endpoint_status");
   #endif
   
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;

   if (stall) {
      __raw_writel(
        (__raw_readl(WPCM450_UDC_PORTSC1_REG(usb_dev_ptr->DEV_NUM) + ep*sizeof(__u32)) |
        (EHCI_EPCTRL_TX_EP_STALL | EHCI_EPCTRL_RX_EP_STALL)),
        (WPCM450_UDC_PORTSC1_REG(usb_dev_ptr->DEV_NUM) + ep*sizeof(__u32)));
   } else {
       __raw_writel(
        (__raw_readl(WPCM450_UDC_PORTSC1_REG(usb_dev_ptr->DEV_NUM) + ep*sizeof(__u32)) &
        ( ~(EHCI_EPCTRL_TX_EP_STALL | EHCI_EPCTRL_RX_EP_STALL))),
        (WPCM450_UDC_PORTSC1_REG(usb_dev_ptr->DEV_NUM) + ep*sizeof(__u32)));
   } /* Endif */

   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_set_endpoint_status, SUCCESSFUL");
   #endif
   
} /* EndBody */


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_dci_vusb20_reset_data_toggle
*  Returned Value : None
*  Comments       :
*        Reset the data toggle for the specified endpoint in the
*  specified direction
*
*END*-----------------------------------------------------------------*/
void _usb_dci_vusb20_reset_data_toggle
   (
      /* [IN] the USB_dev_initialize state structure */
      _usb_device_handle         handle,
            
      /* [IN] the Endpoint number */
      __u8                     bEpNum,
            
      /* [IN] direction */
      __u8                     direction
   )
{ /* Body */
     
   EpQueueHeadStruct_t *           ep_queue_head_ptr;
   USB_DEV_STATE_STRUCT_PTR        usb_dev_ptr;
   
   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_reset_data_toggle");
   #endif
   
  
   usb_dev_ptr = (USB_DEV_STATE_STRUCT_PTR)handle;

   /* Get the endpoint queue head address */
   ep_queue_head_ptr = (EpQueueHeadStructPtr_t)
      __raw_readl(WPCM450_UDC_ENDPOINTLISTADDR_REG(usb_dev_ptr->DEV_NUM)) + 
      2*bEpNum + direction;
   
   ep_queue_head_ptr = (EpQueueHeadStructPtr_t)((__u32)ep_queue_head_ptr | _NOMMU);
   
   /* Reset the data toggle for this endpoint. */
   if (ep_queue_head_ptr->dwMaxPktLength & VUSB_EP_QUEUE_HEAD_IOS) {
      /* This is a control endpoint so reset both directions */
      __raw_writel(
        (__raw_readl(WPCM450_UDC_ENDPTCTRL0_REG(usb_dev_ptr->DEV_NUM) + bEpNum*sizeof(__u32)) |
         (EHCI_EPCTRL_TX_DATA_TOGGLE_RST | EHCI_EPCTRL_RX_DATA_TOGGLE_RST)),
         (WPCM450_UDC_ENDPTCTRL0_REG(usb_dev_ptr->DEV_NUM) + bEpNum*sizeof(__u32)));
   } else {   
      __raw_writel(
        (__raw_readl(WPCM450_UDC_ENDPTCTRL0_REG(usb_dev_ptr->DEV_NUM) + bEpNum*sizeof(__u32)) |
         (direction ? EHCI_EPCTRL_TX_DATA_TOGGLE_RST : 
         EHCI_EPCTRL_RX_DATA_TOGGLE_RST)),
         (WPCM450_UDC_ENDPTCTRL0_REG(usb_dev_ptr->DEV_NUM) + bEpNum*sizeof(__u32)));
   } /* Endif */

   
   #ifdef _DEVICE_DEBUG_
      printk("_usb_dci_vusb20_reset_data_toggle, SUCCESSFUL");
   #endif
      
} /* EndBody */



/*
*/
static int 
_usb_irq_init
     (
      __u8   vector_number,
      irqreturn_t (*isr_ptr)(int, void *, struct pt_regs *),
      void* handle
     )
{
  int retval = 0;
  
  retval = request_irq(vector_number, isr_ptr,SA_INTERRUPT, "wpcm450_udc", handle);

  __raw_writel(0x47, WPCM450_AIC_SCR21);

  return retval;
}



/* EOF */

