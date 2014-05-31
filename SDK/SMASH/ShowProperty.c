#include"SFCC_API/inc/SFCC_API.h"
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char* argv[])
{
	int test;
	ShowInfo *show;
	
	show = malloc(sizeof(ShowInfo *));
	PList *p;
	PList *now;
	char *pid_pos;
	
	int dump_file = 0;
	FILE *fp;
	char file_name[32];
	volatile int i,a=1;

	for(i=0;i<argc;i++)
	{
		if( (strstr(argv[i],"format=clpxml")!=0) || (strstr(argv[i],"format=keyword")!=0) ){ 
			if(argv[i+1]!=NULL)
			if( (strstr(argv[i+1],"pid=")!=0)){
				pid_pos = strchr(argv[i+1],'=');
				if(pid_pos!=0)
				{
					++pid_pos;

					dump_file = 1;
					sprintf(file_name,"/tmp/smash_%s",pid_pos);
					fp = fopen(file_name,"w");
					break;
				}
			}
		}
	}
	test = SFCC_API_Show(show, "root/cimv2", argv[1] , NULL, atoi(argv[2]));
	p = show->PropertyList;

	while(p != NULL )
	{
		if (0 != strcasecmp("CMPI_nullValue",p->value))
		{
			if(dump_file){
				fwrite(p->name,strlen(p->name),1,fp);
				fwrite("=",strlen("="),1,fp);
				fwrite(p->value,strlen(p->value),1,fp);
				fwrite("\n",strlen("\n"),1,fp);
			}else{
				printf("	%s=%s\n", p->name, p->value);
			}
		}
		now = p;
		p = p->next;
		if (now->name)
		free(now->name);
		if (now->value)
		free(now->value);
		free(now);
	}

	if(dump_file)
	{
		fclose(fp);
	}
	free(show->PropertyList);
	free(show);
	return 0;
}
