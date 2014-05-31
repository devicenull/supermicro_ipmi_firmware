
#
# Makefile for the linux YAFFS filesystem routines.
#

obj-$(CONFIG_YAFFS_FS) += yaffs.o

yaffs-y := yaffs_ecc.o yaffs_fs.o yaffs_guts.o
yaffs-y += yaffs_packedtags2.o
yaffs-y += yaffs_tagscompat.o yaffs_tagsvalidity.o
#add it ,by winbond ns24 zswan,2006/05/22
#when i download the sourcecode and patch it, as fallows,make linux,
#but it is error,so , i add the "yaffs_nand.o yaffs_checkptrw.o" here!
#now,it is work very good!ok happy!
yaffs-y += yaffs_mtdif.o yaffs_mtdif2.o yaffs_nand.o yaffs_checkptrw.o
#yaffs-y += yaffs_mtdif.o yaffs_mtdif2.o yaffs_nand.o yaffs_checkptrw.o