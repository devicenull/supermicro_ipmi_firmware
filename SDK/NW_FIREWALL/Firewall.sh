#!/bin/sh
Tmp_File=/tmp/iptable_rule
Tmp_File1=/tmp/iptable_tmp

#Network firewall
if [ ! -d "/nv/ipctrl" ]; then
	mkdir -p /nv/ipctrl
else
	for i in `ls /nv/ipctrl/*.sav`
	do
		if [ ! -f $Tmp_File ];then
			cp  $i $Tmp_File
		else
			cat $i | grep "\-A"  >/tmp/iptable_tmp
		  	sed -i '/:OUTPUT/r /tmp/iptable_tmp' $Tmp_File
		  	rm /tmp/iptable_tmp
		fi
	done	
fi

if [ -f $Tmp_File ];then
	echo "update Firewall rules"
	/sbin/iptables-restore < $Tmp_File
	rm $Tmp_File
fi