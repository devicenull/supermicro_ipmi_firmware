#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "SFCC_API/inc/SFCC_API.h"
#include "autoconfig.h"
#include "type.h"
#include "status.h"
#include "gdef.h"
#include "OS_api.h"
#include "gparam.h"
#include "flash.h"

extern unsigned char b_num_of_burn;

void release_online_counter()
{
    os_mutex_get(&at_lock_FS_upload_MTX, OS_WAIT_FOREVER);
    online_counter = 0;
	os_mutex_put(&at_lock_FS_upload_MTX);
}
void release_file()
{
	char cmd[128];
	memset(cmd, 0, sizeof(cmd));
	sprintf(cmd, "rm /tmp/fwimage.bin");
    system(cmd);
}
int ATEN_Shm_AT()
{
	if (NVRAM_Shm_AT() != STATUS_SUCCESS)
	{
		return -1;
	}	
	if (GlobalVar_Shm_AT(&at_p_St_GlobalVar) != STATUS_SUCCESS)
	{
		NVRAM_Shm_DT();	
		return -1;
	}
	if (PS_Shm_AT(&at_p_St_PS) != STATUS_SUCCESS)
	{
    	GlobalVar_Shm_DT(at_p_St_GlobalVar);						
    	NVRAM_Shm_DT();			
		return -1;
	}    	
	return 0;
}
int ATEN_Shm_DT()
{
    PS_Shm_DT(at_p_St_PS);
    GlobalVar_Shm_DT(at_p_St_GlobalVar);						
    NVRAM_Shm_DT();			
	return 0;
}
int main(int argc, char* argv[])
{
	int test, b_file_size;
    pid_t pid;
    int FWUpload_ShareMID, res;
    void *at_p_FWUpload_Shared_mem=(void*)0; 
    unsigned char * data;
    struct stat fp_stat;
    FILE *fp;
    char cmd[128];
    
    memset(cmd, 0, sizeof(cmd));
    
    if (ATEN_Shm_AT() != 0)
    {
    	printf("exec load failed.\n");
    	return -1;		
    }
    
    os_mutex_get(&at_lock_FS_upload_MTX, OS_WAIT_FOREVER); 
    if(online_counter == 0){  	
    	online_counter = 1;  
    }
    else
    {     	
    	os_mutex_put(&at_lock_FS_upload_MTX);
    	ATEN_Shm_DT();
    	printf("Warning! Other user is updating firmware, please wait for system restart. \n");
    	return -1;	
    }
    os_mutex_put(&at_lock_FS_upload_MTX);

    int sut = strncasecmp(argv[3], "tftp", 4);
    if(0 == sut){
    	char host[128];
    	char *src_file, *tmp;
    	memset(host, 0, sizeof(host));
    	tmp = strstr(argv[3]+strlen("tftp://"),"/");
    	if(tmp == NULL)
    	{
    		release_online_counter();
    		ATEN_Shm_DT();
    		printf("The command format is error\n");
    	}
    	memcpy(host, argv[3]+strlen("tftp://"), tmp-argv[3]-strlen("tftp://"));
    	src_file = tmp + 1;
    	sprintf(cmd, "tftp -r %s -l /tmp/fwimage.bin -g %s", src_file, host);
    
    	printf("Wait for file downloading...\n");
    }
    else
    	sprintf(cmd, "wget %s -O /tmp/fwimage.bin", argv[3]);
    system(cmd);
    
    res = stat("/tmp/fwimage.bin", &fp_stat);
    if(0 != res)
    {
    	release_online_counter();
		release_file();
		ATEN_Shm_DT();
		printf("Getting file from source was failed.\n");
		return -1;
    }
    

    FWUpload_ShareMID = shm_get_shmid(FW_UPLOAD_KEY);	
	if(FWUpload_ShareMID != -1)
  	{
  		at_p_FWUpload_Shared_mem = shmat(FWUpload_ShareMID,(void*)0,SHM_RND);
	    if(at_p_FWUpload_Shared_mem == (void*)-1)
	    {
		    release_online_counter();
			release_file();	
			ATEN_Shm_DT();	
			printf("Error Occurred 1\n");	
			return -1;
	    }
  	}
  	else
  	{
		FWUpload_ShareMID = shmget(FW_UPLOAD_KEY,fp_stat.st_size + 8,0666|IPC_CREAT);
		if(FWUpload_ShareMID == -1)
		{
			release_online_counter();	
			release_file();
			ATEN_Shm_DT();
			printf("Error Occurred 2\n");	
			return -1;
	  	}
		at_p_FWUpload_Shared_mem = shmat(FWUpload_ShareMID,(void*)0,SHM_RND);
		if(at_p_FWUpload_Shared_mem == (void*)-1)
		{
			release_online_counter();
			release_file();
			ATEN_Shm_DT();
			printf("Error Occurred 3\n");	
			return -1;
		}	
	}
	

	data = (unsigned char *)at_p_FWUpload_Shared_mem;
    fp = fopen("/tmp/fwimage.bin", "r");	
    if(fp == NULL)
    {
    	shmdt(at_p_FWUpload_Shared_mem);
    	release_online_counter();
		release_file();
		ATEN_Shm_DT();
		printf("Copy image failed 1.\n");	
		return -1;
    }
    
    os_mutex_get(&at_lock_FS_upload_MTX, OS_WAIT_FOREVER);		   
	memset(data, 0, fp_stat.st_size + 8);
	b_file_size = fread(data, sizeof(char), fp_stat.st_size, fp);	
	os_mutex_put(&at_lock_FS_upload_MTX);	 
	
	fclose(fp);
	
	if(b_file_size != fp_stat.st_size)
	{
		shmdt(at_p_FWUpload_Shared_mem);
    	release_online_counter();
		release_file();	
		ATEN_Shm_DT();
		printf("Copy image failed 2.\n");	
		return -1;
    }
	
	b_num_of_burn = search_header(data, b_file_size);
	if(0 == b_num_of_burn)
    {
    	shmdt(at_p_FWUpload_Shared_mem);
    	release_online_counter();
		release_file();
		ATEN_Shm_DT();
		printf("The image has incorrect format.\n");
		return -1;
    }
    shmdt(at_p_FWUpload_Shared_mem);
	printf("Start to Upload, please wait...\n");

	test = SFCC_API_Load("root/cimv2",argv[1], 1, argv[2]);	
	if(SFCC_SUCCESS == test)
		printf("Update complete, please wait device reboot.");
	else
	{
		printf("Update failed\n.");
		release_online_counter();
		release_file();		
	}
	ATEN_Shm_DT();

	return 0;
}
