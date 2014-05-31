#ifndef _UPDATE_MODE_H_
#define _UPDATE_MODE_H_
const unsigned char Spattern[64] = "ATENs_FW";

int init_flash_dev(unsigned char *origin_nmmode);
void exit_flash_dev(unsigned char origin_nmmode);
int open_flash_fd(int *dev_fd,const unsigned char *dev_name );
int close_flash_fd(int dev_fd);
int update_bios(unsigned char *buf_ptr, unsigned char *progress_arg, unsigned int interface);
int check_fw_tag(unsigned char *src, unsigned int size);

#endif	// End of #ifndef _UPDATE_MODE_H_
