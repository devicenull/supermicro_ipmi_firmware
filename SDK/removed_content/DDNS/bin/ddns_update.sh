#!/bin/sh

PATH=/sbin:/bin:/usr/sbin:/usr/bin
export PATH

basedir="/usr/local/etc/ddns"           # working directory
source "$basedir"/"DDNS_CONFIG"

if [ "$enable" != "true" ]; then
    #echo "ddns disable."
    exit 0    
fi

ntp_enable=`cat /etc/ntp.conf | grep enable | sed -e "s/#//"`
if [ "$ntp_enable" != "enable" ]; then
    #echo "ntp disabled."
    exit 0
fi


hostname=`cat /etc/bmc_hostname`
if [ "$hostname" == "" ]; then
    echo "[ddns_update error] empty hostname."
    exit 1
fi

# 0. keyin your parameters
keyfile="$basedir"/"$keyname"			# your ddns' key (filename)
ttl=600						# the ttl time ( 10 min. )
outif=""
#outif="eth0"					# Your interface (connect to internet)

outif_test1=`ifconfig | grep bond0 | awk '{print $1}'`
outif_test2=`ifconfig | grep eth0 | awk '{print $1}'`
outif_test3=`ifconfig | grep eth1 | awk '{print $1}'`
if [ ! -z $outif_test1 ]; then
	outif=$outif_test1
else
	if [ ! -z $outif_test2 ]; then
		outif=$outif_test2
	else		
		outif=$outif_test3
	fi
fi

# Get your new IP
newip=`ifconfig "$outif" | grep 'inet addr' | \
	awk '{print $2}' | sed -e "s/addr\://"`
checkip=`echo $newip | grep "^[0-9]"`

if [ "$checkip" == "" ]; then
	echo "$0: The interface can't connect internet...."
	exit 1
fi

netmask=`ifconfig $outif | grep 'inet addr' | awk '{print $4}' | sed -e "s/Mask\://"`

max_num=4

mask_tmp=""
ip_tmp=""
domain_tmp=""

host_prefix=""
domain_prefix=""
#for((i=$max_num;i>0;i=i-1))
for i in 4 3 2 1
do
    mask_tmp=`echo $netmask | awk -F '.' '{print $'$i'}'`
    ip_tmp=`echo $checkip | awk -F '.' '{print $'$i'}'`

    if [ "$mask_tmp" != "0" ] || [ "$domain_prefix" != "" ]; then
        domain_tmp=$((ip_tmp&$mask_tmp))
        domain_prefix="$domain_prefix""$domain_tmp".
    fi

    host_prefix="$host_prefix""$ip_tmp".
done

domain_prefix=$domain_prefix"in-addr.arpa"
host_prefix=$host_prefix"in-addr.arpa"

# create the temporal file
tmpfile=/nv/tmp.txt
tmpfile_r=/nv/tmp_r.txt
cd $basedir
echo "server $dns_server" 			>  $tmpfile
echo "update delete $hostname A " 		>> $tmpfile
echo "update add    $hostname $ttl A $newip" 	>> $tmpfile
echo "send" 					>> $tmpfile

echo "server $dns_server" > $tmpfile_r
echo "zone $domain_prefix" >> $tmpfile_r
echo "update delete $host_prefix PTR" >> $tmpfile_r
echo "update add $host_prefix $ttl PTR $hostname" >> $tmpfile_r
echo "send" >> $tmpfile_r

# send your IP & new hostname to server
if [ "$enable_auth" = "true" ]; then
	echo "Do Authentication with TSIG(0)"
nsupdate -k $keyfile -v $tmpfile
nsupdate -k $keyfile -v $tmpfile_r
else
	echo "Do Authentication without TSIG(0)"
	nsupdate -v $tmpfile
	nsupdate -v $tmpfile_r
fi
