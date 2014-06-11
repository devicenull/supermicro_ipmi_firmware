#kill process
killall -10 ikvmserver
killall -9 usb
killall -9 uima
killall -9 uiso
sleep 1

#remove driver
/sbin/rmmod usb_hid.ko
/sbin/rmmod ikvm_vmass.ko
sleep 1

#insert driver
/sbin/insmod usb_hid.ko
/sbin/insmod ikvm_vmass.ko
sleep 1

#re-init process
/bin/usb&
/bin/uima&
/bin/uiso&
/bin/ikvmserver&

