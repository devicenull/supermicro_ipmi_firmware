/*******************************************************************************
** File          : $HeadURL$ 
** Author        : $Author: afishman $
** Project       : HSCTRL 
** Instances     : 
** Creation date : 
********************************************************************************/

#include <asm/page.h>
#include <asm/pgtable.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <asm/arch/regs-irq.h>
#include <asm/arch/map.h>
#include "wpcm450_usb_utils.h"

/* Macro for aligning the EP queue head to 32 byte boundary */
#define USB_MEM32_ALIGN(n)         ((n) + (-(n) & 31))
#define USB_CACHE_ALIGN(n)         USB_MEM32_ALIGN(n)


#define PSP_CACHE_LINE_SIZE        (32)
#if PSP_CACHE_LINE_SIZE
#define PSP_MEMORY_ALIGNMENT       (PSP_CACHE_LINE_SIZE-1)
#else
#define PSP_MEMORY_ALIGNMENT       (3)
#endif
#define PSP_MEMORY_ALIGNMENT_MASK  (~PSP_MEMORY_ALIGNMENT)

#if 0
__u8* usb_memory_alocate_base = (__u8*)WPCM450_VA_USB1_MEM;
#endif

#ifndef _USB_CACHE_USE_
/******************************************************************************
   Allocate non-cacheable memory
   This function is near-clone of __vmalloc. Unlike
   vmalloc rm_alloc_nocache allocates memory area that is
   contiguous in physical address space
******************************************************************************/
void* USB_memalloc(__u32 size)
{
#if 1
        volatile int pages;
        void *start_addr;
        volatile struct page *page;
        volatile struct page **page_array;
        int i;

        size = PAGE_ALIGN(size);
        pages = size/PAGE_SIZE;

        /* Allocate contiguous area in physical address space
         */
        page = (struct page*)alloc_pages(GFP_KERNEL, get_order(size));
        if (!page)
                return NULL;

        page_array = (struct page**)kmalloc(pages * sizeof(struct page *), GFP_KERNEL);
        if (!page_array)
        {
                __free_pages(page, get_order(size));
                return NULL;
        }
        for(i=0; i<pages; i++)
                page_array[i] = page++;

        start_addr = vmap(page_array, pages, VM_ALLOC, __pgprot(_L_PTE_DEFAULT & (~(L_PTE_CACHEABLE | L_PTE_BUFFERABLE))));
        kfree(page_array);
        if (!start_addr)
                __free_pages(page, get_order(size));

        return start_addr;
#else
    void * unaligned_ptr = usb_memory_alocate_base;
    __u8* aligned_ptr =  (__u8*) USB_CACHE_ALIGN( (__u32) unaligned_ptr);
    __u32* temp = (__u32*) (aligned_ptr);
    (*temp) = (__u32) usb_memory_alocate_base;
    usb_memory_alocate_base += (USB_CACHE_ALIGN(size) + PSP_CACHE_LINE_SIZE*2);
    return (void*)(aligned_ptr + PSP_CACHE_LINE_SIZE);
#endif

}

/* Release memory block allocated by rm_alloc_nocache
 */
void USB_memfree(void *free_addr)
{
#if 1
        vunmap(free_addr);
#else
    __u32* temp = (__u32*) ((__u8*)free_addr - PSP_CACHE_LINE_SIZE);
    usb_memory_alocate_base = (void *) (*temp);
    return;
#endif
}


#endif 
