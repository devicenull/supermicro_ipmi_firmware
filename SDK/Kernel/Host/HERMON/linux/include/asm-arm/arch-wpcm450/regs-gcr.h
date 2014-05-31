/* linux/include/asm-arm/arch-wpcm450/regs-gcr.h
 *
 *
 *  Internal header file for winbond wpcm450 global control register
 *
 *  Copyright (C) 2006 shirley yu (clyu2@winbond.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Modifications:
 *     21-Aug-2006 first version
 */

#ifndef __ASM_ARM_REGS_GCR_H
#define __ASM_ARM_REGS_GCR_H

#define WPCM450_GCR(x) (WPCM450_VA_GCR + (x))

#define WPCM450_GCR_PDID	  WPCM450_GCR(0x00)
#define WPCM450_GCR_MFSEL1	WPCM450_GCR(0x0c)
#define WPCM450_GCR_INTCR  	WPCM450_GCR(0x3c)

#endif /* __ASM_ARM_REGS_SERIAL_H */

