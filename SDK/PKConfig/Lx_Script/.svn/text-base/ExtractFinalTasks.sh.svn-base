#! /bin/bash

#	$1 :	Project Path
#	$2 :	SDK .c Source code path
#	$3 :	SDK relative .h file path

if [ -d "$1/TmpLibExSDK" ]; then
	mv -f $1/TmpLibExSDK/*.c $2
	mv -f $1/TmpLibExSDK/inc/*.h $3
fi;

echo "$2/../Makefile"

sed -i -e 's/CFLAGS +=/CFLAGS += -I$(SDKROOT)\/SDKFILES\/DEFAULT\/inc/' $2/../../Makefile
rm -Rf $1/TmpLibExSDK
