#include"SFCC_API/inc/SFCC_API.h"
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

static int selectdir(const struct dirent *dir);

int main(int argc, char* argv[])
{
	int test;
	ShowInfo *show;
	char instance[100];
	int i, total;
	PList *p;
	PList *now;
	struct dirent **namelist;
	show = malloc(sizeof(ShowInfo *));

	test = SFCC_API_Show(show, "root/cimv2", NULL , argv[1], atoi(argv[2]));
	total = scandir(argv[4],&namelist,selectdir,0);
	if (total > show->NumSubClass)
	{	
		for(i=total;i>show->NumSubClass;i--)
		{
			sprintf(instance,"%s/%s%03d",argv[4],argv[3],i);
			rmdir(instance);
		}
		
	}
	else if (total < show->NumSubClass)
	{
		for(i=total+1;i<=show->NumSubClass;i++)
		{
			sprintf(instance,"%s/%s%03d",argv[4],argv[3],i);
			mkdir(instance,0777);
		}
	}

	p = show->PropertyList;
	while(p != NULL )
	{
		now = p;
		p = p->next;
		free(now->name);
		free(now->value);
		free(now);
	}
	free(show->PropertyList);
	free(show);
	return 0;
}

int selectdir(const struct dirent *dir)
{
	if(0 == strcasecmp(dir->d_name,".") || 0 == strcasecmp(dir->d_name,".."))
		return 0;
	else
		return 1;
}
