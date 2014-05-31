#define AddIPV6Address(x,y,z) HandleIPV6Address(x,y,1,z)
#define RemoveIPv6Address(x,y,z) HandleIPV6Address(x,y,2,z)

#define IPV6_CONFIG_FILE "/etc/network/netconfig"
#define IPV6_CONFIG_FILE_NOLINK "/nv/network/netconfig"
#define IPV6_SH_FILE "/etc/network/network6"
#define PROC_IFINET6_PATH "/proc/net/if_inet6"
#define IPV6_ADDR_LOOPBACK      0x0010U
#define MAX_IPV6_ADDR_LEN   (sizeof "ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255"+10)
#define MAXIPV6ADDRNUM  5

#define IPADDRV6_SUCCESS 0
#define IPADDRV6_FAIL 1

#define IPV6ADDR_ADD 1
#define IPV6ADDR_DEL 2
#define IPV6ADDR_UNKNOWN 3

#define IPV6_AUTOCONF_ON 1
#define IPV6_AUTOCONF_OFF 0

#define IPV6_VLAN_DOWN 0
#define IPV6_VLAN_UP 1

#define IPV6_VLAN_SUCCESS 0
#define IPV6_VLAN_FAIL 0

typedef struct St_IPV6_Cfg{
    int num;
    char ipv6_addr[MAXIPV6ADDRNUM][MAX_IPV6_ADDR_LEN];
    char valid[MAXIPV6ADDRNUM];
} St_IPV6_Cfg_Tag;

extern int lock_operation(int fd, int cmd, int lock_type, off_t offset, int whence, off_t len);
extern int ipv6_file_lock(int fd);
extern int ipv6_file_unlock(int fd);

extern int HandleIPV6Address(int ChnlNo,unsigned char *p_addr,int b_flag, char autoconfig);

extern int ipv6_full_addr(const char *ipv6_string, int str_len, char *addr_buf);
