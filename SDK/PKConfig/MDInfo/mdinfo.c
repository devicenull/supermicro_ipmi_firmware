#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mdinfo.h"
#include "../../IPMI/UpdateMode/inc/UpdateMode.h"

#define DO_CONVERT 1
#define SIZE_16M 0x1000000
#define MAX_IMG_NUM	5

enum
{
	MCU_HERMON,
	MCU_ASPEED
};

struct St_ImgInfo
{
	unsigned char ucaCrc[4];
	unsigned int uiStartAddr;
	unsigned int uiLen;
};

int char2hex(unsigned char *str, int fixed_len, int option)
{
	int i, j;
	
	/* Check length */
	j = strlen((const char *)str);
	if(j > fixed_len)
	{
		return -1;
	}
	else if(j < fixed_len)
	{
		for(i = fixed_len; i > (fixed_len-j); i--)
			str[i-1] = str[i-(fixed_len-j)-1];
		for(i = 0; i < (fixed_len-j); i++)
			str[i] = '0';
	}
	str[fixed_len] = 0;
	

	if(DO_CONVERT == option)
	{
		for(j = 0; j < fixed_len/2; j++)
		{
			for(i = j*2; i < (j*2+2); i++)
			{
				if('0' <= str[i] && str[i] <= '9')
					str[i] = str[i] - 48;
				else if('A' <= str[i] && str[i] <= 'F')
					str[i] = str[i] - 55;
				else if('a' <= str[i] && str[i] <= 'f')
					str[i] = str[i] - 87;
				else
					return -1;
			}
			str[j] = str[j*2]*16 + str[j*2+1];
		}
		str[j] = 0;
	}
	return 0;
}

unsigned long crc32(unsigned long crc,const unsigned char *buf, unsigned int len)
{
    crc = crc ^ 0xffffffffL;
    while (len >= 8)
    {
      DO8(buf);
      len -= 8;
    }
    if (len) do {
      DO1(buf);
    } while (--len);
    return crc ^ 0xffffffffL;
}

int main(int argc,char *argv[])
{
  struct St_ImgInfo a_St_ImgInfo[MAX_IMG_NUM];
	unsigned char tmpCrcBuf[64];
	FILE *fp = NULL, *fp1 = NULL;
	fpos_t pos[10];//9th index has special purpose!
	fpos_t last_pos;
	signed int complete_flag = 0;
	char filepath[256], fileptrname[32];
	unsigned long ulFinalCRC = 0;
	unsigned char buff[64];
	unsigned char *data_tmp;
	unsigned int i, j, mcu_type;
	unsigned int length = 0, nbytes = 0, bin_length = 0, bin_last = 0, dummy_res = 0;
	unsigned int checksum = 0;
	unsigned long k = 0;
	tfooter *footer_frame, footer_tmp;
	UB_header *header_frame;
	void* tmpBuf;
	unsigned long long sum=0;
	unsigned int *p_temp;	
	unsigned char Reserved_char[64]="abcd efgh ijkl ";
	unsigned char str_rfs_checksum[32];
	unsigned char str_wfs_checksum[32];
	unsigned char str_rfs_length[32];
	unsigned char str_wfs_length[32];
	unsigned char *aten_info_ptr;
	unsigned char *Firmware_revision_1_str;
  	unsigned char Firmware_revision_1_char[64]="NULL string";
  	unsigned char *Firmware_revision_2_str;
  	unsigned char Firmware_revision_2_char[64]="NULL string";
	//"NULL string"
  	int count;
	int cpuflag=0,fw_v1=0,fw_v2=0;
	unsigned long u32Length;
	const NEW_FWTAG[2]=	{0x71,0x17};
 
  if(!(argc%2))
  {	
  	printf("argument format failed!!\n");
  	return -1;
  }
  if(argc==1)
  {
  	printf("Usage:\n\tmdinfo [-c] param1 [-v] param2 [-m] param3 [-f] param4\n\n");
  	printf("param1: 0(HERMON) , 1(ASPEED)\n");
  	printf("param2: Firmware revision_1 (1 byte in HEX)\n");
  	printf("param3: Firmware revision_2 (1 byte in HEX)\n");
  	printf("param4: .bin file\n");  	
  	return;
  }
  	

  for(count=0;count<argc-1;count++)
  {
  	if( **(argv+count) == 45)
  	{
  		if( *(*(argv+count)+1) == 'f') 
  		{
  			sscanf(*(argv+count+1),"%s",&filepath);
  		}	  			
  		if( *(*(argv+count)+1) == 'v') 
  		{
  			Firmware_revision_1_str=*(argv+count+1);
  			memset(Firmware_revision_1_char,0,64);
  			strcpy((char*)Firmware_revision_1_char, (const char*)Firmware_revision_1_str);
  			if(char2hex(Firmware_revision_1_char, 2, DO_CONVERT))
        {
          printf("revision_1 Too many bytes\n");
          return-1;
        }
          fw_v1=1;
      }
      if( *(*(argv+count)+1) == 'm') 
  		{
  			Firmware_revision_2_str=*(argv+count+1);
  			strcpy((char*)Firmware_revision_2_char, (const char*)Firmware_revision_2_str);
  			if(char2hex(Firmware_revision_2_char, 2, DO_CONVERT))
  		  {
          printf("revision_2 Too many bytes\n");
          return-1;
        }
        fw_v2=1;
      }
      if( *(*(argv+count)+1) == 'c')  
  		{
  			if(**(argv+count+1) == '0')
  			{	
  				mcu_type=0;
  				cpuflag=1;			
  			}
  		  else if(**(argv+count+1) == '1')
  			{	
  				mcu_type=1;
  				cpuflag=1;
  			}
  			else
  			  cpuflag=0;
  		}     
    }
  }

  if(cpuflag==0)
  {
  	printf("[-c] param is failed\n");
  	return -1;
  }  	
  
  printf("======================== MDINFO ========================\n");
  if(mcu_type==1)
   printf("MCU = ASPEED\n");
  else
  	printf("MCU = HERMON\n");
  printf("FW revision_1 = %x\n",Firmware_revision_1_char[0]);
  printf("FW revision_2 = %x\n",Firmware_revision_2_char[0]); 

  
  
  tmpBuf = (unsigned char*)malloc(SIZE_16M*sizeof(unsigned char));
  fp = fopen(filepath, "r+b");
	if(fp != NULL)
	{
		printf("BIN file path = %s\n",filepath);
			switch(mcu_type)
			{
				case MCU_HERMON:
				{
					footer_frame = (tfooter *)malloc(1*sizeof(struct t_footer));
					j = 0;
					while (!feof(fp))
					{
						nbytes = fread(buff, 1, 64, fp);
						length += nbytes;
						memcpy((void *)footer_frame, (void *)&buff[12], sizeof(struct t_footer));
            			
            			if(0xA0FFFF9F == footer_frame->signature)
            			{
							memcpy((void *)&footer_tmp, (void *)footer_frame, sizeof(struct t_footer));
							p_temp = (unsigned int *)&footer_tmp;	
							sum=0;
							for(i=0;i<sizeof(struct t_footer)/4-1;i++)
							{
								sum+=*(p_temp+i);
							}
							sum = ~((sum&(-1LU))+(sum>>32));							
 				
            				if((unsigned int)sum == (unsigned int)footer_frame->checksum)
            				{
								sprintf(fileptrname, "No%d.bin", j);
            					fp1 = fopen (fileptrname, "w+b");
            					
            					bin_length = length - (footer_frame->base - ulFlashBase_wbl);
            					
            					fgetpos(fp, &pos[j]);
            					fseek(fp, (-1)*bin_length, SEEK_CUR);
                            	
								while(bin_length)
    							{
									nbytes = fread(buff, 1, 64, fp);
									fwrite(buff, 1, nbytes, fp1);
									bin_length -= nbytes;
								}
								a_St_ImgInfo[j].uiLen = footer_frame->length;
								a_St_ImgInfo[j].uiStartAddr = footer_frame->base- ulFlashBase_wbl;
								j++;
								fclose(fp1);
							}
            			}
					}
					unsigned int iCrtImgIdx = 0;
					for(iCrtImgIdx = 0;iCrtImgIdx<j;iCrtImgIdx++)
					{
						int readByte = 0;
						unsigned long tmpCRC = 0;				
						memset(tmpBuf,0xFF,SIZE_16M);
						fseek(fp,a_St_ImgInfo[iCrtImgIdx].uiStartAddr , SEEK_SET);
						readByte = fread(tmpBuf, 1, a_St_ImgInfo[iCrtImgIdx].uiLen, fp);
						tmpCRC = crc32(0, (const unsigned char *)tmpBuf, a_St_ImgInfo[iCrtImgIdx].uiLen);

						tmpCrcBuf[iCrtImgIdx*4] = (unsigned char)(tmpCRC);	
						tmpCrcBuf[iCrtImgIdx*4+1] = (unsigned char)(tmpCRC >> 8);	
						tmpCrcBuf[iCrtImgIdx*4+2] = (unsigned char)(tmpCRC >> 16);	
						tmpCrcBuf[iCrtImgIdx*4+3] = (unsigned char)(tmpCRC >> 24);	
					}
					ulFinalCRC = crc32(0, (const unsigned char *)tmpCrcBuf, iCrtImgIdx*4);
					j--;
					fsetpos(fp, &pos[j]);
					fseek(fp, (-1)*(52+25), SEEK_CUR);

					sprintf(fileptrname, "No%d.bin", j);			
					fp1 = fopen(fileptrname, "r+b");			
					fseek(fp1, (-1)*(52+25), SEEK_END);
					
					fputs((char *)Spattern, fp);
					
					fputs((char *)Spattern, fp1);
	

					
					fputc(Firmware_revision_1_char[0], fp);
					fputc(Firmware_revision_1_char[0], fp1);
					fputc(Firmware_revision_2_char[0], fp);
					fputc(Firmware_revision_2_char[0], fp1);
  
					fputc(NEW_FWTAG[0], fp);
					fputc(NEW_FWTAG[0], fp1);

       				fputc((unsigned char)(ulFinalCRC), fp);
					fputc((unsigned char)(ulFinalCRC), fp1);

					fputc((unsigned char)(ulFinalCRC>>8), fp);
					fputc((unsigned char)(ulFinalCRC>>8), fp1);
					       				
					fputc((unsigned char)(ulFinalCRC>>16), fp);
					fputc((unsigned char)(ulFinalCRC>>16), fp1);
					       				
					fputc((unsigned char)(ulFinalCRC>>24), fp);
					fputc((unsigned char)(ulFinalCRC>>24), fp1);
       		
					fputc(NEW_FWTAG[1], fp);
					fputc(NEW_FWTAG[1], fp1);

            		free(footer_frame);
					fclose(fp);
					fclose(fp1);
					
					break;
				}
				case MCU_ASPEED:
				{
					header_frame = (UB_header *)malloc(1*sizeof(struct UB_headerType));
					j = 0;
					while (!feof(fp))
					{
						nbytes = fread(buff, 1, 64, fp);
						length += nbytes;
						memcpy((void *)header_frame, (void *)buff, sizeof(struct UB_headerType));
            			
            			if(UBOOT_IH_MAGIC == CNV_ENDIAN(header_frame->ih_magic))
            			{
            				sprintf(fileptrname, "No%d.bin", j);
            				fp1 = fopen (fileptrname, "w+b");
            				
            				fgetpos(fp, &pos[9]);
            				
            				bin_length = (unsigned int)CNV_ENDIAN(header_frame->ih_size);
            				
            				if(1 == j)
            				{
            					bin_length += sizeof(struct UB_headerType);
            					fseek(fp, (-1)*sizeof(struct UB_headerType), SEEK_CUR);
            				}
            				
            				

            		   		if(2 == j)
								data_tmp = (unsigned char *)malloc(bin_length*sizeof(unsigned char) + 25);
            				
            				
            				bin_last = bin_length % 64;
            				dummy_res = 64 - bin_last;
							for(i = 0; i < (bin_length/64); i++)
    						{
								nbytes = fread(buff, 1, 64, fp);
								fwrite(buff, 1, nbytes, fp1);
								
								if(2 == j)
									memcpy((void *)&data_tmp[i*64], (void *)buff, 64);
								
							}
							
							fgetpos(fp, &pos[j]);
							
            				if(bin_last)
            				{
								nbytes = fread(buff, 1, bin_last, fp);
								fwrite(buff, 1, nbytes, fp1);

								if(2 == j)
									memcpy((void *)&data_tmp[i*64], (void *)buff, bin_last);
								

								fgetpos(fp, &pos[j]);
								
								nbytes = fread(buff, 1, dummy_res, fp);
            				}							
            				
            				j++;
							
							fclose(fp1);
            			}
					}
					
					j--;
					fsetpos(fp, &pos[j]);
					fseek(fp, 0, SEEK_CUR);
					
					sprintf(fileptrname, "No%d.bin", j);			
					fp1 = fopen(fileptrname, "a+b");
					fseek(fp1, 0, SEEK_END);

					fputs((char *)Spattern, fp);
					fputs((char *)Spattern, fp1);
					memcpy((void *)&data_tmp[bin_length], (void *)Spattern, 8);
					
					fputs((char *)Firmware_revision_1_char, fp);
					fputs((char *)Firmware_revision_1_char, fp1);
					memcpy((void *)&data_tmp[bin_length+8], (void *)Firmware_revision_1_char, 1);
					
					fputc(Firmware_revision_2_char[0], fp);
					fputc(Firmware_revision_2_char[0], fp1);
					memcpy((void *)&data_tmp[bin_length+9], (void *)Firmware_revision_2_char, 1);
					

					fputc(Reserved_char[0], fp);
					fputc(Reserved_char[0], fp1);
					fclose(fp1);
					memcpy((void *)&data_tmp[bin_length+10], (void *)(char *)Reserved_char, 1);					

            		fsetpos(fp, &pos[9]);
            		fseek(fp, (-1)*64, SEEK_CUR);
            		fread(buff, 1, 64, fp);
            		
            		memcpy((void *)header_frame, (void *)buff, sizeof(struct UB_headerType));

            		header_frame->ih_size = CNV_ENDIAN(bin_length + 25);
            		checksum = crc32(0, (const unsigned char *)data_tmp, bin_length + 25);
            		header_frame->ih_dcrc = CNV_ENDIAN(checksum);
            		
            		header_frame->ih_hcrc = 0;
            		checksum = crc32(0, (const unsigned char *)header_frame, sizeof(UB_header));
            		header_frame->ih_hcrc = CNV_ENDIAN(checksum);            		
            		
            		memcpy((void *)buff, (void *)header_frame, sizeof(struct UB_headerType));
            		
            		fseek(fp, (-1)*64, SEEK_CUR);
            		fwrite(buff, 1, 64, fp);
            		
					fclose(fp);
					
					
					free(data_tmp);
					free(header_frame);
					break;
				}
			
				default:
					break;
			}

			complete_flag = 1;
	}
	else
	{
		printf("Can't open this file!\n\n");
		free(tmpBuf);
		return -1;	
	}
	if(1 == complete_flag)
		printf("---> Modification is done!\n\n");
	free(tmpBuf);
	return 0;
}
