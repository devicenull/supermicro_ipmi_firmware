

#ifndef _IPMB_H_
#define _IPMB_H_

#include <linux/ioctl.h> 
#include <asm/io.h>
#include <asm/hardware.h>


#undef PDEBUG             
#ifdef IPMB_DEBUG
    #ifdef __KERNEL__
        #define PDEBUG(fmt, args...) printk( KERN_DEBUG "ipmb: " fmt, ## args)
    #else
        #define PDEBUG(fmt, args...) fprintf(stderr, fmt, ## args)
    #endif
#else
    #define PDEBUG(fmt, args...) 
#endif

#undef PDEBUGG
#define PDEBUGG(fmt, args...) 

#define IPMB_MAJOR 168

#ifndef IPMB_MAJOR
#define IPMB_MAJOR 0   
#endif

#ifndef IPMB_NR_DEVS
#define IPMB_NR_DEVS 6    
#endif

#define TABLE_SIZE 16

#define BUFFER_SIZE 256

#define Q_TIME_OUT    0

struct i2cCtlInfo_t 
{
    char b_Channel;
    char b_waittype;
    char b_Ctrl;
    char b_Type;
    char b_Slaveaddr;
    char result;
    char ret_data;
    char rx_len;
    char tx_len;
    char *rx_buf;
    char *tx_buf;
};
struct ipmb_dev
{
	int dev_index;
	struct cdev cdev;
	struct semaphore sem;
	wait_queue_head_t waitq;
	wait_queue_head_t waitqwr;
	struct i2cCtlInfo_t ipmbCtlInfo;
	char txb[2][BUFFER_SIZE];
    char rxb[2][BUFFER_SIZE];
};


int     hermon_ipmb_open(struct inode *inode, struct file *filp);
ssize_t hermon_ipmb_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
int     hermon_ipmb_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg);

typedef enum
{
    EMPTY = 0,
    USED,
    RESPONSED
} En_EntryStatusTag;

struct table_entry
{
    En_EntryStatusTag entryStatus;
    int seq_no;
    int owner_pid;
};




#define IPMB_IOC_MAGIC  'i'

#define IPMB_IOCSCONF   _IOW(IPMB_IOC_MAGIC,  1, struct i2cCtlInfo_t)
#define IPMB_IOCXMSG    _IOWR(IPMB_IOC_MAGIC, 2, struct i2cCtlInfo_t)
#define IPMB_IOCSTART   _IOW(IPMB_IOC_MAGIC, 3, struct i2cCtlInfo_t)
#define IPMB_IOCSTOP    _IOW(IPMB_IOC_MAGIC, 4, struct i2cCtlInfo_t)
#define IPMB_IOC_MAXNR 4

int ipmb_config_channel(unsigned long arg, struct file *filp);
int ipmb_write_read_msg(unsigned long arg, struct file *filp);

#endif 


