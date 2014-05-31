#!/bin/bash
./mkimage -n 1stFS -u 2 -l 0x00d00000 -e 0x00d00000 -b 0x40180000 -i rootfs -o out_rootfs_img.bin -f
zip out_kernel.bin.tmp kernel.bin
./mkimage -n kernel -u 3 -l 0x8000 -e 0x8000 -b 0x40980000 -i out_kernel.bin.tmp -o out_kernel.bin -acxz
./mkimage -n 2ndFS -u 4 -l 0x00d00000 -e 0x00d00000 -b 0x40B80000 -i webfs -o out_webfs_img.bin -f
./mkbin mkbin.inf HERMONEVB_all.bin 16
rm kernel.bin rootfs webfs
