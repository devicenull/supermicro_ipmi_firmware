/*
 *  linux/include/asm-arm/arch-wpcm450/memory.h
 *
 *  from linux/include/asm-arm/arch-rpc/memory.h
 *
 *  Copyright (C) 1996,1997,1998 Russell King.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Changelog:
 *   20-Oct-1996 RMK	Created
 *   31-Dec-1997 RMK	Fixed definitions to reduce warnings
 *   11-Jan-1998 RMK	Uninlined to reduce hits on cache
 *   08-Feb-1998 RMK	Added __virt_to_bus and __bus_to_virt
 *   21-Mar-1999 RMK	Renamed to memory.h
 *		 RMK	Added TASK_SIZE and PAGE_OFFSET
 *   05-Apr-2004 BJD    Copied and altered for arch-wpcm450
 *   17-Mar-2005 LCVR   Modified for S3C2400
*/

#ifndef __ASM_ARCH_MEMORY_H
#define __ASM_ARCH_MEMORY_H

/*
 * DRAM starts at 0x00000000 for WPCM450
 */
#define PHYS_OFFSET	UL(0x00000000)

/*
 * These are exactly the same on the WPCM450 as the
 * physical memory view.
*/

#define __virt_to_bus(x) __virt_to_phys(x)
#define __bus_to_virt(x) __phys_to_virt(x)

#endif
