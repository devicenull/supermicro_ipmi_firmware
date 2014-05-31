#ifndef __usbprv_h__
#define __usbprv_h__ 1
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
** $Date: 2008/02/26 09:13:33Z $
** $Revision: 1.2 $
*******************************************************************************
*** Comments:      
***   This file contains the internal USB specific type definitions
***                                                               
**************************************************************************
**END*********************************************************/

#include <linux/string.h>
#include <linux/interrupt.h>
#include <asm/arch/regs-irq.h>


#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define _NOMMU ((unsigned long) 0xc0000000) //Get Physical Address for DMA
#define NOMMU (~_NOMMU)

#define _USB_CACHE_USE_


void USB_lock(void);  
void USB_unlock(void); 




/**********************************************************
The following lines have been commented because when MQX
is not used, the RTOS abstration layer provides the
implementation of USB_memalloc routine which aligns
the pointers on cache line size.
**********************************************************/

#ifdef _USB_CACHE_USE_
#define USB_memalloc(n)                      kmalloc(n, GFP_KERNEL)
#define USB_memfree(ptr)                     kfree(ptr)
#else
void* USB_memalloc(__u32 size); 
void USB_memfree(void* ptr); 
#endif

#define USB_memzero(ptr,n)                   memset(ptr,0,n)
#define USB_memcopy(src,dst,n)               memcpy(dst,src,n)



#endif

/* EOF */

