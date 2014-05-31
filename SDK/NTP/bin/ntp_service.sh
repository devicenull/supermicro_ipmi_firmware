ntp_option=`cat /etc/ntp.conf | grep 'enable'`
wait_time=15
retry=12
update_result=""
count=0

/bin/ntp_setenv /nv/ntp/ntp.conf /nv/timezone

if [ "$ntp_option" = "#enable" ]
then
    ntp_server_ip=`cat /etc/ntp.conf | grep server | awk '{print $2}' | sed 1q`
    if [ "$ntp_server_ip" != "" ]
    then

		while [ $count -lt $retry ]
		do
	        update_result=`/bin/ntpdate $ntp_server_ip 2>&1`

			if [ -z "$update_result" ]
			then 
				break
			else								
				echo "[NTP] NTP update failed. It will try again in $wait_time second(s). [$count]"
				sleep $wait_time
			fi

			count=$(( $count+1 ))			
		done

		if [ $count -eq $retry ]
		then
			echo "[NTP] Time synchronization fail. System will continue to synchronize system time by using the ntpd daemon..."
		else
			echo "[NTP] Time synchronization success. System will continue to synchronize system time by using the ntpd daemon..."
			if [ -d /nv/ddns ]; then /bin/sh /sbin/ddns_update.sh ; fi
		fi
		
        /bin/ntpd -g -c /etc/ntp.conf -p /tmp/ntpd.pid
        /bin/ntp_setenv -done
    fi
fi
