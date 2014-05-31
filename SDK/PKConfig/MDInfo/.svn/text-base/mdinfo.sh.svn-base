#!/bin/bash
# note: if cpu is yarkon, it will exit mdinfo script
[ "$1" == "2" ] && exit0
ConFile_Path=$2
echo $ConFile_Path
FWv1=`cat $ConFile_Path/platform_X9DRW.xml |awk '/FirmwareRev/ {print $4}'|cut -d x -f2 |sed "s/\"//"`
FWv2=`cat $ConFile_Path/platform_X9DRW.xml |awk '/FirmwareRev/ {print $5}'|cut -d x -f2 |sed "s/\"//"`

echo "Firmware revision is $FWv1.$FWv2"
if [ "$1" == "0" ]; then
cd ../PKConfig/MDInfo/;./mdinfo -c 0 -v $FWv1 -m $FWv2 -f ../../images/HERMONEVB_all.bin
fi
if [ "$1" == "1" ]; then
cd ../PKConfig/MDInfo/;./mdinfo -c 1 -v $FWv1 -m $FWv2 -f ../../images/AST2050_all.bin
fi
mv *.bin ../../images