/* This is a generated file, don't edit */

#define NUM_APPLETS 87

const char applet_names[] ALIGN1 = ""
"arp" "\0"
"arping" "\0"
"ash" "\0"
"awk" "\0"
"bbconfig" "\0"
"cat" "\0"
"chmod" "\0"
"chown" "\0"
"cp" "\0"
"dd" "\0"
"df" "\0"
"dmesg" "\0"
"du" "\0"
"echo" "\0"
"egrep" "\0"
"fgrep" "\0"
"grep" "\0"
"gunzip" "\0"
"gzip" "\0"
"halt" "\0"
"hostname" "\0"
"ifconfig" "\0"
"ifdown" "\0"
"ifenslave" "\0"
"ifup" "\0"
"init" "\0"
"insmod" "\0"
"ip" "\0"
"ipaddr" "\0"
"iplink" "\0"
"iproute" "\0"
"kill" "\0"
"killall" "\0"
"killall5" "\0"
"linuxrc" "\0"
"ln" "\0"
"login" "\0"
"ls" "\0"
"lsmod" "\0"
"mesg" "\0"
"mkdir" "\0"
"mknod" "\0"
"mktemp" "\0"
"mount" "\0"
"mountpoint" "\0"
"mt" "\0"
"mv" "\0"
"netstat" "\0"
"nice" "\0"
"passwd" "\0"
"ping" "\0"
"ping6" "\0"
"pipe_progress" "\0"
"pivot_root" "\0"
"poweroff" "\0"
"ps" "\0"
"pwd" "\0"
"realpath" "\0"
"reboot" "\0"
"reset" "\0"
"rm" "\0"
"rmmod" "\0"
"route" "\0"
"run-parts" "\0"
"rx" "\0"
"script" "\0"
"sed" "\0"
"sendmail" "\0"
"setsid" "\0"
"sh" "\0"
"sleep" "\0"
"start-stop-daemon" "\0"
"tar" "\0"
"telnetd" "\0"
"tftp" "\0"
"top" "\0"
"touch" "\0"
"traceroute" "\0"
"udhcpc" "\0"
"umount" "\0"
"uname" "\0"
"unzip" "\0"
"usleep" "\0"
"vconfig" "\0"
"wget" "\0"
"which" "\0"
"zcat" "\0"
;

int (*const applet_main[])(int argc, char **argv) = {
arp_main,
arping_main,
ash_main,
awk_main,
bbconfig_main,
cat_main,
chmod_main,
chown_main,
cp_main,
dd_main,
df_main,
dmesg_main,
du_main,
echo_main,
grep_main,
grep_main,
grep_main,
gunzip_main,
gzip_main,
halt_main,
hostname_main,
ifconfig_main,
ifupdown_main,
ifenslave_main,
ifupdown_main,
init_main,
insmod_main,
ip_main,
ipaddr_main,
iplink_main,
iproute_main,
kill_main,
kill_main,
kill_main,
init_main,
ln_main,
login_main,
ls_main,
lsmod_main,
mesg_main,
mkdir_main,
mknod_main,
mktemp_main,
mount_main,
mountpoint_main,
mt_main,
mv_main,
netstat_main,
nice_main,
passwd_main,
ping_main,
ping6_main,
pipe_progress_main,
pivot_root_main,
halt_main,
ps_main,
pwd_main,
realpath_main,
halt_main,
reset_main,
rm_main,
rmmod_main,
route_main,
run_parts_main,
rx_main,
script_main,
sed_main,
sendgetmail_main,
setsid_main,
ash_main,
sleep_main,
start_stop_daemon_main,
tar_main,
telnetd_main,
tftp_main,
top_main,
touch_main,
traceroute_main,
udhcpc_main,
umount_main,
uname_main,
unzip_main,
usleep_main,
vconfig_main,
wget_main,
which_main,
gunzip_main,
};
const uint16_t applet_nameofs[] ALIGN2 = {
0x0000,
0x0004,
0x000b,
0x000f,
0x0013,
0x001c,
0x0020,
0x0026,
0x002c,
0x002f,
0x0032,
0x0035,
0x003b,
0x003e,
0x0043,
0x0049,
0x004f,
0x0054,
0x005b,
0x0060,
0x0065,
0x006e,
0x0077,
0x007e,
0x0088,
0x008d,
0x0092,
0x0099,
0x009c,
0x00a3,
0x00aa,
0x00b2,
0x00b7,
0x00bf,
0x00c8,
0x00d0,
0x80d3,
0x00d9,
0x00dc,
0x00e2,
0x00e7,
0x00ed,
0x00f3,
0x00fa,
0x0100,
0x010b,
0x010e,
0x0111,
0x0119,
0x811e,
0x4125,
0x012a,
0x0130,
0x013e,
0x0149,
0x0152,
0x0155,
0x0159,
0x0162,
0x0169,
0x016f,
0x0172,
0x0178,
0x017e,
0x0188,
0x018b,
0x0192,
0x0196,
0x019f,
0x01a6,
0x01a9,
0x01af,
0x01c1,
0x01c5,
0x01cd,
0x01d2,
0x01d6,
0x41dc,
0x01e7,
0x01ee,
0x01f5,
0x01fb,
0x0201,
0x0208,
0x0210,
0x0215,
0x021b,
};
const uint8_t applet_install_loc[] ALIGN1 = {
0x32,
0x31,
0x11,
0x11,
0x11,
0x11,
0x13,
0x11,
0x11,
0x21,
0x21,
0x22,
0x22,
0x12,
0x11,
0x11,
0x33,
0x10,
0x11,
0x32,
0x11,
0x11,
0x11,
0x11,
0x31,
0x11,
0x21,
0x12,
0x31,
0x32,
0x21,
0x12,
0x33,
0x41,
0x13,
0x21,
0x41,
0x33,
0x31,
0x12,
0x31,
0x21,
0x33,
0x01,
};

#define MAX_APPLET_NAME_LEN 17
