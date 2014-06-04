#!/bin/bash

# Generate the necessary content for the web filesystem (the 2nd cramfs filesystem)
# Should go into /home/devicenull/ipmi/SDK/FileSystem/Host/HERMON/webfs/*

WEBFS_DIR="/home/devicenull/ipmi/SDK/FileSystem/Host/HERMON/webfs/"

cd /home/devicenull/ipmi/SDK/
cp -r Web_Server/OS/Linux/Web_Style/SUPERMICRO/* $WEBFS_DIR
rm $WEBFS_DIR/HTTPCMDTable.o $WEBFS_DIR/Makefile $WEBFS_DIR/multi_language_table.o $WEBFS_DIR/extract.in
mkdir -p $WEBFS_DIR/cgi
cp Web_Server/OS/Linux/*.cgi $WEBFS_DIR/cgi/
cp JavaAP/SOL/SOL__V0.5.11.jar.pack.gz $WEBFS_DIR/
cp JavaAP/KVM/iKVM__V1.69.21.0x0.jar.pack.gz $WEBFS_DIR/
ln -s cgi $WEBFS_DIR/cgi-bin
