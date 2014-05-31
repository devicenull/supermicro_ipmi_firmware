#!/bin/sh
export PATH=/SMASH:/SFCB/local/bin:/SFCB/local/sbin:/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin
export LD_LIBRARY_PATH=/wsman/openwsman/lib:/SFCB/local/lib:/SFCB/zlib/lib:/SFCB/curl/lib:/lib:/usr/lib:/usr/local/lib

/dropbear/sbin/dropbear -p $1 -I 600
#/usr/sbin/telnetd -p $2 -l /SMASH/msh

