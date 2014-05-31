#!/bin/sh
PATH=/SMASH:/SFCB/local/bin:/SFCB/local/sbin:/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin
export PATH
export rm_files="def_hostname bmc_hostname ddns PSBlock FWBlock SDRBlock network ntp SELBlock server.pem timezone vm_image.conf wsman ipctrl lighttpd.conf enSSL.config config_syslog system_elog system_logena system_istate"
export cp_files="def_hostname hostname_for_dhcp bmc_hostname ddns SDRBlock FWBlock PSBlock MouseMode network ntp resolv.conf server.pem timezone vm_image.conf wsman ipctrl lighttpd.conf enSSL.config config_syslog system_elog system_logena system_istate"

