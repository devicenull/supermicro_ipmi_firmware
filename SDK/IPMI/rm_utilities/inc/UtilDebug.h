#ifndef _UTILDEBUG_H_
#define _UTILDEBUG_H_

#ifndef ipmi_log
#define ipmi_log(fmt, args...)  fprintf(stderr, fmt, ## args)
#endif

#define WriteIOMM(x,y) RW_IOMEM(x,y,1)
#define ReadIOMM(x,y)  RW_IOMEM(x,y,0)
#ifdef _UTILDEBUG_C_

#else
extern int DEBUG_LOG(const char *format,...);
extern int RW_IOMEM(DWORD dwAddr ,DWORD *dwValue,int b_RW);
extern void console_log(const char *degmsg, ... );
extern void UtilGetMoreMemory(void);
extern int mmap_addr(int *fd,off_t target,void *virt_addr,unsigned long map_size);
extern int ummap_addr(int fd,void *virt_addr,unsigned long map_size);
#endif

#endif
