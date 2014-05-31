#!/bin/bash
# note: if cpu is yarkon, it will exit mdinfo script
[ "$1" == "2" ] && exit0
echo "Modify version in XML files"
version=$2
ConFile_Path=$1
FWv1=$(echo $version | cut -d '.' -f 1)
FWv2=$(echo $version | cut -d '.' -f 2)
v1errflag=1
v2errflag=1

while [ "$v1errflag" == "1" -o "$v2errflag" == "1" ]
do
 FWv1size=${#FWv1}
 FWv2size=${#FWv2}
 FWv1data=$(echo $FWv1 | grep '^[0-9a-fA-F]*[0-9a-fA-F]$')
 FWv2data=$(echo $FWv2 | grep '^[0-9a-fA-F]*[0-9a-fA-F]$')
 if [ "$FWv1size" -gt "2" ]; then
 	echo "Worng FW1 ver."
 else
  	v1errflag=0
 fi
 if [ "$FWv2size" -gt "2" ]; then
 	echo "Worng FW2 ver."
 else
  	v2errflag=0
 fi
 if [ -z "$FWv1data" ]; then
  	v1errflag=1
 fi
 if [ -z "$FWv2data" ]; then
  	v2errflag=1
 fi 
 if [ "$v1errflag" == "1" -o "$v2errflag" == "1" ]; then
 	read -p "please input Firmware revision_1 (1 byte in HEX): " FWv1
 	read -p "please input Firmware revision_2 (1 byte in HEX): " FWv2
 	[ -z "$FWv1" ] && FWv1=0
 	[ -z "$FWv2" ] && FWv2=0
 fi
done

tagname=`date +%Y-%m-%d`

ConFiles=`find $ConFile_Path -name 'platform_*.xml' |xargs ls`

for i in $ConFiles
do
    sed -i -e "s/[0-9]\{4\}-[0-1][0-9]-[0-3][0-9]/$tagname/"  		   $i
    sed -i -e "s/FirmwareRev1=\"0x[0-9]*\"/FirmwareRev1=\"0x$FWv1\"/"  $i
    sed -i -e "s/FirmwareRev2=\"0x[0-9]*\"/FirmwareRev2=\"0x$FWv2\"/"  $i
done