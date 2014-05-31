#!/bin/sh
INTERVAL=1
count=0
RETRIES=3
while [  $count -le $RETRIES ];
do
    /bin/msmtp $*
    if [ $? -eq 0 ];then break;fi
    sleep $INTERVAL
    let count=count+1;
done
