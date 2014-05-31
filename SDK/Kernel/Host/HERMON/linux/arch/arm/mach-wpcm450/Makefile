
#
# Makefile for the linux kernel.
#

# Object file lists.

obj-y			:= cpu.o irq.o time.o clock.o devs.o
obj-m			:=
obj-n			:=
obj-			:=

# WPCM450 support files

obj-$(CONFIG_CPU_WPCM450)  += wpcm450.o
obj-$(CONFIG_WPCM450_DMA)  += dma.o

# Power Management support

obj-$(CONFIG_PM)	   += pm.o sleep.o
obj-$(CONFIG_PM_SIMTEC)	   += pm-simtec.o

# machine specific support
obj-$(CONFIG_MACH_WPCM450)	+= mach-wpcm450.o