%{
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <ftw.h>
#include <libgen.h>
//#include"SFCC_API/inc/SFCC_API.h"

extern char buf[];
extern char* myptr;
extern char* mylim;

extern void display_history_list();
extern void history_finish();
//extern int SFCC_API_Show(ShowInfo *show, char *NameSpace, char *ClassName, char *SubClassName, int index);

static int my_yyinput(char* buf, int max);
static void free_resource();

static int do_exit(int, char**);
static int do_cd(int, char**);
static int do_version(int, char**);
static int do_show(int, char**);
static int do_help(int, char**);
static int do_delete(int, char**);
static int do_start(int, char**);
static int do_stop(int, char**);
static int do_reset(int, char**);
static int selectdir(const struct dirent *dir);


static void add_arg(const char* xarg);
static void add_simple_arg(const char* xarg);
static void reset_args();
static void do_list_cmd();
static int do_pipe_cmd(int argc, char** argv);
static int do_simple_cmd(int argc, char** argv, int prefd[], int postfd[]);

#undef 	YY_INPUT
#define YY_INPUT(b, r, ms) (r = my_yyinput(b, ms))

char* argbuf[200];
int argcnt = 0;

typedef int (*buildin_cmd_handle)(int, char**);
typedef struct
{
	const char* cmd;
	buildin_cmd_handle handle;
} CMD_ENTRY;

typedef struct ResourceTableType
{
	char *resourcename;
	char *classname;
	char *subclassname;
	char *subinstancename;
	int cmd;
	char *help;
} ResourceTable;

#define CD_CMD		0x0001
#define SHOW_CMD	0x0002
#define HELP_CMD	0x0004
#define VERSION_CMD	0x0008
#define EXIT_CMD	0x0010
#define RESET_CMD	0x0020
#define STOP_CMD	0x0040
#define START_CMD	0x0080
#define DELETE_CMD	0x0100

ResourceTable St_ResourceTable[] = 
{
	/* (resource name , CIM class name, cmd enable), */
	/* cmd enable:
	   bit 8: delete
	   bit 7: start 
	   bit 6: stop
	   bit 5: reset
	   bit 4: exit 
	   bit 3: version
	   bit 2: help
	   bit 1: show
	   bit 0: cd	
	*/
    { "smash", NULL, NULL, NULL, 0x001f, "The managed element is the root"},
    { "system", NULL, NULL, NULL, 0x001f, "This target is used to manage system settings and operations"},
    { "pwrmgtsvc", "IPMI_PowerManagementService", NULL, NULL, 0x00ff, "This target is used to power control service"},
    { "logs", NULL, NULL, NULL, 0x001f, "Contains the collection of SEL log Elements"},
    { "chassis", "CIM_Chassis", NULL, NULL, 0x001f, "This target is used to manage the physical chassis"},
    { "fans", NULL, NULL, NULL, 0x001f, "Manage fan sensors"},
    { "voltages", NULL, NULL, NULL, 0x001f, "Manage voltage sensors"},
    { "log", "IPMI_RecordLog", "IPMI_LogRecord", "record", 0x011f, "System Event Log (SEL)"},
    { "record", "IPMI_LogRecord", NULL, NULL, 0x001e, "Contains all the records currently in SEL"},
    { "sensors", "CIM_ComputerSystem", "CIM_Sensor", "sensor", 0x001f, "Contains the collection of sensor Elements"},
    { "sensor", "CIM_Sensor", NULL, NULL, 0x001e, "Contains all the sensor status"},
    {0, 0, 0, 0, 0, 0}
};

const CMD_ENTRY buildin_cmd_table[] = 
{
	{"exit", 	do_exit},
	{"cd", 		do_cd},
	{"version", do_version},
	{"show", 	do_show},
	{"help", 	do_help},
	{"delete", 	do_delete},
	{"start", 	do_start},
	{"stop", 	do_stop},
	{"reset", 	do_reset},
	{0, 0}
};

%}

%x		QUOTE
%x		SINGQUOTE
blank	[ \t]

%%

"\""			{BEGIN QUOTE;}
<QUOTE>[^\n"]+	{add_arg(yytext);}
<QUOTE>"\""		{BEGIN 0;}
<QUOTE>\n		{BEGIN 0; do_list_cmd(); reset_args();}
";"				{add_simple_arg(yytext);}
">"				{add_simple_arg(yytext);}
"<"				{add_simple_arg(yytext);}
"|"				{add_simple_arg(yytext);}
[^ \t\n|<>;"]+	{add_arg(yytext);}
\n				{do_list_cmd(); reset_args();}
.				;

%%

/* string to low*/
inline char* strlwr( char* str )
{
   char* orig = str;
   // process the string
   for ( ; *str != '\0'; str++ )
       *str = tolower(*str);
   return orig;
}

/* check if command support for target*/
int cmd_check(char *bname, int cmd)
{
	int i=0;
//	printf("IN_cmd_check : bname = %s ; cmd = %d\n",bname,cmd);
	while(St_ResourceTable[i].resourcename) 
	{
		if(strcasecmp(St_ResourceTable[i].resourcename, bname) == 0)
			break;
		i++;
	}
	if (0 == St_ResourceTable[i].resourcename)
	{
		printf("Undefine resource... \n");	
	}
	
	if (cmd & St_ResourceTable[i].cmd)
		return 1;
	else
		return 0;
}

/* build dynamic smash tree*/
int buildfs(const char *file, const struct stat *sb,int flag)
{
	char bname[255];
	char UFcT[255];
	int i;
	char *number;
	sprintf(bname,"%s",basename(file));
	for(i=0; isdigit(bname[i])== 0; i++)
	{
		UFcT[i] = bname[i];	
	}
	UFcT[i]='\0';
	number=index(bname,bname[i]);
	
	i = 0;
	while(St_ResourceTable[i].resourcename) 
	{
		if(strcasecmp(St_ResourceTable[i].resourcename, UFcT) == 0)
			break;
		i++;
	}
	
	if(NULL != St_ResourceTable[i].subclassname)
	{
		if (fork()==0)
		{
			if (-1 == execl("/SMASH/buildFS","buildFS",St_ResourceTable[i].subclassname,number,St_ResourceTable[i].subinstancename,file,(char *)0))
			{
				printf("1:%s \n",strerror(errno));
				exit(0);
			}
		}
		wait(NULL);
	}	
	
	return 0;
}

static buildin_cmd_handle get_cmd_handle(const char* cmd)
{
	int i = 0;
	while(buildin_cmd_table[i].cmd) {
		if(strcasecmp(buildin_cmd_table[i].cmd, cmd) == 0)
			return buildin_cmd_table[i].handle;
		i++;
	}
	return 0;
}
static void free_resource()
{
	reset_args();
}

static int do_exit(int argc, char** argv)
{
	int val = 0;
	if(argc > 1)
		val = atoi(argv[1]);
	free_resource();
	history_finish();
	exit(val);
	return 0;
}

static int do_cd(int argc, char** argv)
{
	int i;
	char *p;
	//char current[255];
	char *current;
	char target[255];
	char *k;
	
//	sprintf(current,"%s",get_current_dir_name());
	current=getcwd(NULL,0);
//	if(argc == 1) 
//	{
//		printf("%s\n",get_current_dir_name());
//	}
//	else 
	ftw("/tmp/smash",buildfs,0);
	if(argc >= 2) 
	{
		/* process -help option */
		for(i=1 ; i<argc ; i++)
		{
			if(NULL != argv[i])
			if(0 == strcasecmp(argv[i],"-h") || 0 == strcasecmp(argv[i],"-help"))
			{
				argv[i]=NULL;
				printf("The CD command is used to navigate the target address space.\n");
				return 0;
			}
		}
		/* process -version option */
		for(i=1 ; i<argc ; i++)
		{
			if(NULL != argv[i])
			if(0 == strcasecmp(argv[i],"-v") || 0 == strcasecmp(argv[i],"-version"))
			{
				argv[i]=NULL;
				printf("Version 1.00 \n");
				return 0;
			}
		}
		/* process change target */
		for(i=1 ; i<argc ; i++)
		{
			if(NULL != argv[i])
			{
				argv[i] = strlwr(argv[i]);
				if('/' == argv[i][0])
					sprintf(target,"/tmp/smash%s",argv[i]);
				else
					sprintf(target,"%s",argv[i]);
				if( -1 == chdir(target))
					printf("Invalid Targets!.\n");
				break;
			}
		}
	}
	
//	k = get_current_dir_name();
	k = getcwd(NULL,0);
	p = strstr(k,"/tmp/smash");
	if (NULL == p)
	{
		chdir(current);
		printf("Invalid Targets!.\n");
	}
	else
	{
		p = strstr(k,"/system1");
		if (NULL == p)
			printf("/ \n");
		else
			printf("%s\n",p);
	}
//	printf("%s\n",get_current_dir_name());
	if(k) free(k);
	return 0;
}

int do_version(int argc, char** argv)
{
	printf("ATEN SMASH CLP Shell Version 1.00 \n");	
	return 0;
}

int do_start(int argc, char** argv)
{
	int i;
	char bname[255];
	char UFcT[255];
	//char path[255];
	char *path;
	char target[255];
	ftw("/tmp/smash",buildfs,0);
//	sprintf(path,"%s",get_current_dir_name());
	path = getcwd(NULL,0);
	char *p;
	char *k;
	/* process -help option */
	for(i=1 ; i<argc ; i++)
	{
		if(NULL != argv[i])
		if(0 == strcasecmp(argv[i],"-h") || 0 == strcasecmp(argv[i],"-help"))
		{
			argv[i]=NULL;
			printf("The START command is used to start target service such as startup power.\n");
			return 0;
		}
	}
	/* process -version option */
	for(i=1 ; i<argc ; i++)
	{
		if(NULL != argv[i])
		if(0 == strcasecmp(argv[i],"-v") || 0 == strcasecmp(argv[i],"-version"))
		{
			argv[i]=NULL;
			printf("Version 1.00\n");
			return 0;
		}
	}
	/* change to target */
	for(i=1 ; i<argc ; i++)
	{
		if(NULL != argv[i])
		{
			argv[i] = strlwr(argv[i]);
			if('/' == argv[i][0])
				sprintf(target,"/tmp/smash%s",argv[i]);
			else
				sprintf(target,"%s",argv[i]);
			if( -1 == chdir(target))
			{
				printf("Invalid Targets!.\n");
				return 0;
			}
			break;
		}
	}	
	/*check if this target valid */
//	k = get_current_dir_name();
	k = getcwd(NULL,0);
	p = strstr(k,"/tmp/smash");
	if (NULL == p)
	{
		chdir(path);
		printf("Invalid Targets!.\n");
		chdir(path);
		return 0;	
	}
	else
	{
	}

//	sprintf(bname,"%s",basename(get_current_dir_name()));
	sprintf(bname,"%s",basename(getcwd(NULL,0)));
	for(i=0; isdigit(bname[i])== 0; i++)
	{
		UFcT[i] = bname[i];	
	}
	UFcT[i]='\0';

	i = 0;
	while(St_ResourceTable[i].resourcename) 
	{
		if(strcasecmp(St_ResourceTable[i].resourcename, UFcT) == 0)
			break;
		i++;
	}

	if (0 == cmd_check(UFcT,START_CMD))
	{
		printf("unsupport command for target!..please see help target. \n");
	}
	else
	{
		if (fork()==0)
		{
			if (-1 == execl("/SMASH/start","start",St_ResourceTable[i].classname, "StartService",(char *)0))
			{
				printf("1:%s \n",strerror(errno));
				exit(0);
			}
		}
		wait(NULL);		
		printf("start done... \n");	
	}
	chdir(path);
	return 0;
}

int do_stop(int argc, char** argv)
{
	int i;
	char bname[255];
	char UFcT[255];
//	char path[255];
	char *path;
	char target[255];
	ftw("/tmp/smash",buildfs,0);
//	sprintf(path,"%s",get_current_dir_name());
	path = getcwd(NULL,0);
	char *p;
	char *k;
	/* process -help option */
	for(i=1 ; i<argc ; i++)
	{
		if(NULL != argv[i])
		if(0 == strcasecmp(argv[i],"-h") || 0 == strcasecmp(argv[i],"-help"))
		{
			argv[i]=NULL;
			printf("The STOP command is used to stop target service such as shut down power.\n");
			return 0;
		}
	}
	/* process -version option */
	for(i=1 ; i<argc ; i++)
	{
		if(NULL != argv[i])
		if(0 == strcasecmp(argv[i],"-v") || 0 == strcasecmp(argv[i],"-version"))
		{
			argv[i]=NULL;
			printf("Version 1.00\n");
			return 0;
		}
	}
	/* change to target */
	for(i=1 ; i<argc ; i++)
	{
		if(NULL != argv[i])
		{
			argv[i] = strlwr(argv[i]);
			if('/' == argv[i][0])
				sprintf(target,"/tmp/smash%s",argv[i]);
			else
				sprintf(target,"%s",argv[i]);
			if( -1 == chdir(target))
			{
				printf("Invalid Targets!.\n");
				return 0;
			}
			break;
		}
	}	
	/*check if this target valid */
//	k = get_current_dir_name();
	k = getcwd(NULL,0);
	p = strstr(k,"/tmp/smash");
	if (NULL == p)
	{
		chdir(path);
		printf("Invalid Targets!.\n");
		chdir(path);
		return 0;	
	}
	else
	{
	}

//	sprintf(bname,"%s",basename(get_current_dir_name()));
	sprintf(bname,"%s",basename(getcwd(NULL,0)));
	for(i=0; isdigit(bname[i])== 0; i++)
	{
		UFcT[i] = bname[i];	
	}
	UFcT[i]='\0';

	i = 0;
	while(St_ResourceTable[i].resourcename) 
	{
		if(strcasecmp(St_ResourceTable[i].resourcename, UFcT) == 0)
			break;
		i++;
	}

	if (0 == cmd_check(UFcT,STOP_CMD))
	{
		printf("unsupport command for target!..please see help target. \n");
	}
	else
	{
		if (fork()==0)
		{
			if (-1 == execl("/SMASH/stop","stop",St_ResourceTable[i].classname, "StopService",(char *)0))
			{
				printf("1:%s \n",strerror(errno));
				exit(0);
			}
		}
		wait(NULL);		
		printf("stop done... \n");	
	}
	chdir(path);
	return 0;
}

int do_reset(int argc, char** argv)
{
	int i;
	char bname[255];
	char UFcT[255];
//	char path[255];
	char *path;
	char target[255];
	ftw("/tmp/smash",buildfs,0);
//	sprintf(path,"%s",get_current_dir_name());
	path = getcwd(NULL,0);
	char *p;
	char *k;
	/* process -help option */
	for(i=1 ; i<argc ; i++)
	{
		if(NULL != argv[i])
		if(0 == strcasecmp(argv[i],"-h") || 0 == strcasecmp(argv[i],"-help"))
		{
			argv[i]=NULL;
			printf("The RESET command is used to restart target service such as reset power.\n");
			return 0;
		}
	}
	/* process -version option */
	for(i=1 ; i<argc ; i++)
	{
		if(NULL != argv[i])
		if(0 == strcasecmp(argv[i],"-v") || 0 == strcasecmp(argv[i],"-version"))
		{
			argv[i]=NULL;
			printf("Version 1.00\n");
			return 0;
		}
	}
	/* change to target */
	for(i=1 ; i<argc ; i++)
	{
		if(NULL != argv[i])
		{
			argv[i] = strlwr(argv[i]);
			if('/' == argv[i][0])
				sprintf(target,"/tmp/smash%s",argv[i]);
			else
				sprintf(target,"%s",argv[i]);
			if( -1 == chdir(target))
			{
				printf("Invalid Targets!.\n");
				return 0;
			}
			break;
		}
	}	
	/*check if this target valid */
//	k = get_current_dir_name();
	k = getcwd(NULL,0);
	p = strstr(k,"/tmp/smash");
	if (NULL == p)
	{
		chdir(path);
		printf("Invalid Targets!.\n");
		chdir(path);
		return 0;	
	}
	else
	{
	}

//	sprintf(bname,"%s",basename(get_current_dir_name()));
	sprintf(bname,"%s",basename(getcwd(NULL,0)));
	for(i=0; isdigit(bname[i])== 0; i++)
	{
		UFcT[i] = bname[i];	
	}
	UFcT[i]='\0';

	i = 0;
	while(St_ResourceTable[i].resourcename) 
	{
		if(strcasecmp(St_ResourceTable[i].resourcename, UFcT) == 0)
			break;
		i++;
	}

	if (0 == cmd_check(UFcT,RESET_CMD))
	{
		printf("unsupport command for target!..please see help target. \n");
	}
	else
	{
		if (fork()==0)
		{
			if (-1 == execl("/SMASH/reset","reset",St_ResourceTable[i].classname, "ResetService",(char *)0))
			{
				printf("1:%s \n",strerror(errno));
				exit(0);
			}
		}
		wait(NULL);		
		printf("reset done... \n");	
	}
	chdir(path);
	return 0;
}

int do_delete(int argc, char** argv)
{
	int i;
	char bname[255];
	char UFcT[255];
//	char path[255];
	char *path;
	char target[255];
	ftw("/tmp/smash",buildfs,0);
//	sprintf(path,"%s",get_current_dir_name());
	path = getcwd(NULL,0);
	char *p;
	char *k;
	char kill[30];
	/* process -help option */
	for(i=1 ; i<argc ; i++)
	{
		if(NULL != argv[i])
		if(0 == strcasecmp(argv[i],"-h") || 0 == strcasecmp(argv[i],"-help"))
		{
			argv[i]=NULL;
			printf("The DELETE command is used to delete target instances such as record log\n");
			return 0;
		}
	}
	/* process -version option */
	for(i=1 ; i<argc ; i++)
	{
		if(NULL != argv[i])
		if(0 == strcasecmp(argv[i],"-v") || 0 == strcasecmp(argv[i],"-version"))
		{
			argv[i]=NULL;
			printf("Version 1.00\n");
			return 0;
		}
	}
	/* change to show target */
	for(i=1 ; i<argc ; i++)
	{
		if(NULL != argv[i])
		{
			argv[i] = strlwr(argv[i]);
			p = strrchr(argv[i],'/'); //parse target path/killitem
			if(NULL == p)
			{
				sprintf(target,".");
				strcpy(kill,argv[i]);
			}
			else
			{
				strcpy(kill,p+1);
//				sprintf(p,"");
				if('/' == argv[i][0])
					sprintf(target,"/tmp/smash%s",argv[i]);
				else
					sprintf(target,"%s",argv[i]);
			}	
			if( -1 == chdir(target))
			{
				printf("Invalid Targets!.\n");
				chdir(path);
				return 0;
			}
			break;
		}
	}	

//	k = get_current_dir_name();
	k = getcwd(NULL,0);
	p = strstr(k,"/tmp/smash");
	if (NULL == p)
	{
		chdir(path);
		printf("Invalid Targets!.\n");
		chdir(path);
		return 0;	
	}
	else
	{
	}

	//sprintf(bname,"%s",basename(get_current_dir_name()));
	sprintf(bname,"%s",basename(getcwd(NULL,0)));
	for(i=0; isdigit(bname[i])== 0; i++)
	{
		UFcT[i] = bname[i];	
	}
	UFcT[i]='\0';

	i = 0;
	while(St_ResourceTable[i].resourcename) 
	{
		if(strcasecmp(St_ResourceTable[i].resourcename, UFcT) == 0)
			break;
		i++;
	}

	if (0 == cmd_check(UFcT,DELETE_CMD))
	{
		printf("unsupport command for target!..please see help target. \n");
	}
	else
	{
		if(0 == strcasecmp(kill,"record*"))
		{
			if (fork()==0)
			{
				if (-1 == execl("/SMASH/deletemethod","deletemethod",St_ResourceTable[i].classname, "clearlog",(char *)0))
				{
					printf("1:%s \n",strerror(errno));
					exit(0);
				}
			}
			wait(NULL);		
			printf("Delete %s done... \n",kill);	
		}
		else
		{
			printf("Invalid target!\n");
		}
	}
	chdir(path);
	return 0;
}

/* Don't display . and .. target*/
int selectdir(const struct dirent *dir)
{
	if(0 == strcasecmp(dir->d_name,".") || 0 == strcasecmp(dir->d_name,".."))
		return 0;
	else
		return 1;
}

int do_help(int argc, char** argv)
{
	int i;
	//char path[255];
	char *path;
	char bname[255];
	char UFcT[255];
	char target[255];
	char *number;

	ftw("/tmp/smash",buildfs,0);
	//sprintf(path,"%s",get_current_dir_name());
	path = getcwd(NULL,0);
		
	/* process -help option */
	for(i=1 ; i<argc ; i++)
	{
		if(NULL != argv[i])
		if(0 == strcasecmp(argv[i],"-h") || 0 == strcasecmp(argv[i],"-help"))
		{
			argv[i]=NULL;
			printf("The HELP command is used to descript about targets.\n");
			return 0;
		}
	}
	/* process -version option */
	for(i=1 ; i<argc ; i++)
	{
		if(NULL != argv[i])
		if(0 == strcasecmp(argv[i],"-v") || 0 == strcasecmp(argv[i],"-version"))
		{
			argv[i]=NULL;
			printf("Version 1.00\n");
			return 0;
		}
	}

	/* change to help target */
	for(i=1 ; i<argc ; i++)
	{
		if(NULL != argv[i])
		{
			argv[i] = strlwr(argv[i]);
			if('/' == argv[i][0])
				sprintf(target,"/tmp/smash%s",argv[i]);
			else
				sprintf(target,"%s",argv[i]);
			if( -1 == chdir(target))
			{
				printf("Invalid Targets!.\n");
			}
			break;
		}
	}
	
	//get resource name and mapping to CIM class for search
	//sprintf(bname,"%s",basename(get_current_dir_name()));
	sprintf(bname,"%s",basename(getcwd(NULL,0)));
	for(i=0; isdigit(bname[i])== 0; i++)
	{
		UFcT[i] = bname[i];	
	}
	UFcT[i]='\0';
	number=index(bname,bname[i]);
		
	i = 0;
	while(St_ResourceTable[i].resourcename) 
	{
		if(strcasecmp(St_ResourceTable[i].resourcename, UFcT) == 0)
			break;
		i++;
	}	

	printf("\n  %s\n",St_ResourceTable[i].help);

		printf("\n  Verbs :\n");
		if(St_ResourceTable[i].cmd & CD_CMD)
			printf("	cd\n");
		if(St_ResourceTable[i].cmd & SHOW_CMD)
			printf("	show\n");
		if(St_ResourceTable[i].cmd & HELP_CMD)
			printf("	help\n");
		if(St_ResourceTable[i].cmd & VERSION_CMD)
			printf("	version\n");
		if(St_ResourceTable[i].cmd & EXIT_CMD)
			printf("	exit\n");
		if(St_ResourceTable[i].cmd & DELETE_CMD)
			printf("	delete\n");
		if(St_ResourceTable[i].cmd & START_CMD)
			printf("	start\n");
		if(St_ResourceTable[i].cmd & STOP_CMD)
			printf("	stop\n");
		if(St_ResourceTable[i].cmd & RESET_CMD)
			printf("	reset\n");
		printf("\n");

	chdir(path);
	return 0;
}


int do_show(int argc, char** argv)
{
	int i, total;
	int verbs = 0;
	int targets = 0;
	int properties = 0;
	int displayall= 1;
	//char path[255];
	char *path;
	char bname[255];
	char UFcT[255];
	char target[255];
	char *number;
	char *p;
	char *k;
	struct dirent **namelist;
//	ShowInfo *show;
//	show = malloc(sizeof(ShowInfo *));
//	PList *p;
//	while(1)
//	{


	ftw("/tmp/smash",buildfs,0);
//	sprintf(path,"%s",get_current_dir_name());
	path = getcwd(NULL,0);
	/* process -help option */
	for(i=1 ; i<argc ; i++)
	{
		if(NULL != argv[i])
		if(0 == strcasecmp(argv[i],"-h") || 0 == strcasecmp(argv[i],"-help"))
		{
			argv[i]=NULL;
			printf("The SHOW command is used to display information about targets.\n");
			return 0;
		}
	}
	/* process -version option */
	for(i=1 ; i<argc ; i++)
	{
		if(NULL != argv[i])
		if(0 == strcasecmp(argv[i],"-v") || 0 == strcasecmp(argv[i],"-version"))
		{
			argv[i]=NULL;
			printf("Version 1.00\n");
			return 0;
		}
	}
	/* process -display option */
	for(i=1 ; i<argc ; i++)
	{
		if(NULL != argv[i])
		if(0 == strcasecmp(argv[i],"-d") || 0 == strcasecmp(argv[i],"-display"))
		{
			argv[i]=NULL;
			if(NULL != argv[i+1])
			{
				displayall = 0;
				if(0 == strcasecmp(argv[i+1],"verbs"))
					verbs=1;
				else if(0 == strcasecmp(argv[i+1],"targets"))
					targets=1;
				else if(0 == strcasecmp(argv[i+1],"properties"))
					properties=1;
				else if(0 == strcasecmp(argv[i+1],"all"))
					displayall=1;
				else
				{
					printf("Display arguments error\n");
					return 0;
				}
				argv[i+1]=NULL;
			}
		}
	}
	/* change to show target */
	for(i=1 ; i<argc ; i++)
	{
		if(NULL != argv[i])
		{
			argv[i] = strlwr(argv[i]);
			if('/' == argv[i][0])
				sprintf(target,"/tmp/smash%s",argv[i]);
			else
				sprintf(target,"%s",argv[i]);
			if( -1 == chdir(target))
			{
				printf("Invalid Targets!.\n");
				return 0;
			}
			break;
		}
	}	
	
	
	
	/*check if this target valid */
//	k = get_current_dir_name();
	k = getcwd(NULL,0);
	p = strstr(k,"/tmp/smash");
	if (NULL == p)
	{
		chdir(path);
		printf("Invalid Targets!.\n");
		chdir(path);
		return 0;	
	}
	else
	{
		p = strstr(k,"/system1");
		if (NULL == p)
			printf("/ \n");
		else
			printf("%s\n",p);
	}

	//get resource name and mapping to CIM class for search
	//sprintf(bname,"%s",basename(get_current_dir_name()));
	sprintf(bname,"%s",basename(getcwd(NULL,0)));
	for(i=0; isdigit(bname[i])== 0; i++)
	{
		UFcT[i] = bname[i];	
	}
	UFcT[i]='\0';
	number=index(bname,bname[i]);
	
//	printf("In_SHOW : UFcT = %s \n",UFcT);
	if (0 == cmd_check(UFcT,SHOW_CMD))
	{
		printf("unsupport command for target!..please see help target. \n");
	}
	

	if(displayall==1 || targets==1)
	{
		printf("\n  Targets :\n");
		//total = scandir(get_current_dir_name(),&namelist,selectdir,alphasort);
		total = scandir(getcwd(NULL,0),&namelist,selectdir,alphasort);
		if(total > 0)
		{
			for(i=0; i<total; i++)
				printf("	%s\n",namelist[i]->d_name);
		}
		else
			printf("	none\n");
			
		if(namelist) free(namelist);
	}

	i = 0;
	while(St_ResourceTable[i].resourcename) 
	{
		if(strcasecmp(St_ResourceTable[i].resourcename, UFcT) == 0)
			break;
		i++;
	}	


	if(displayall==1 || properties==1)	
	{
		printf("\n  Properties :\n");
		if(NULL != St_ResourceTable[i].classname)
		{
			if (fork()==0)
			{
				if (-1 == execl("/SMASH/ShowProperty","ShowProperty",St_ResourceTable[i].classname, number,(char *)0))
				{
					printf("1:%s \n",strerror(errno));
					exit(0);
				}
			}
			wait(NULL);
//			printf("	Print\n");
		}
		else
			printf("	None\n");
	}

	if(displayall==1 || verbs==1)
	{
		printf("\n  Verbs :\n");
		if(St_ResourceTable[i].cmd & CD_CMD)
			printf("	cd\n");
		if(St_ResourceTable[i].cmd & SHOW_CMD)
			printf("	show\n");
		if(St_ResourceTable[i].cmd & HELP_CMD)
			printf("	help\n");
		if(St_ResourceTable[i].cmd & VERSION_CMD)
			printf("	version\n");
		if(St_ResourceTable[i].cmd & EXIT_CMD)
			printf("	exit\n");
		if(St_ResourceTable[i].cmd & DELETE_CMD)
			printf("	delete\n");
		if(St_ResourceTable[i].cmd & START_CMD)
			printf("	start\n");
		if(St_ResourceTable[i].cmd & STOP_CMD)
			printf("	stop\n");
		if(St_ResourceTable[i].cmd & RESET_CMD)
			printf("	reset\n");
		printf("\n");

	}

	chdir(path);

	if(k) free(k);
//	if(namelist) free(namelist);
//	}

	return 0;
}

static void add_simple_arg(const char* arg)
{
	argbuf[argcnt] = (char*)malloc(strlen(arg)+1);
	strcpy(argbuf[argcnt], arg);
	argcnt++;
	argbuf[argcnt] = 0;
}

// $HOME
// $$
// $HOME$
// $HOME$HOME
static void add_arg(const char* xarg)
{
	char* arg;

	char buf[200];
	char xbuf[200];
	int i,j,k;
	int len = strlen(xarg);
	

	k = 0;

	for(i = 0; i < len; i++) {
		if(xarg[i] == '$') {
			if(xarg[i+1] == '$') {//$$,get pid
				int pid = getpid();
				sprintf(buf+k, "%d", pid);
				k = strlen(buf);
				i++;
			} else if(xarg[i+1] == 0){//$ and end
				buf[k] = '$';
				k++;
				break;
			} else {//$HOME or $HOME$OTHER
				for(j = i+1; j < len; j++) {
					if(xarg[j] == '$') 
						break;
					xbuf[j-i-1] = xarg[j];
				}
				xbuf[j-i-1] = 0;
				i = j-1;
				if((arg = getenv(xbuf))) {
					strcpy(buf+k, arg);
					k += strlen(arg);
				}
			}
			
		} else {
			buf[k] = xarg[i];
			k++; 
		}
	}
	buf[k] = 0;
	if(k > 0)
		add_simple_arg(buf);
}

static void do_list_cmd()
{
	int i = 0;
	int j = 0;
	char* p;
	while(argbuf[i]) {
		if(strcasecmp(argbuf[i], ";") == 0) {//	 ;
			p = argbuf[i];
			argbuf[i] = 0;
			do_pipe_cmd(i-j, argbuf+j);
			argbuf[i] = p;
			j = ++i;
		} else
			i++;
	}
	do_pipe_cmd(i-j, argbuf+j);
}

static int do_pipe_cmd(int argc, char** argv)
{
	int i = 0;
	int j = 0;
	int prepipe = 0;
	int prefd[2];
	int postfd[2];
	char* p;

	while(argv[i]) {
		if(strcasecmp(argv[i], "|") == 0) { // pipe
			p = argv[i];
			argv[i] = 0;

			pipe(postfd); 		//create the post pipe
			//be sure not close pipe in, otherwise whenever father write to this pipe,
			//cause a Broken pipe.
			//close(postfd[0]);	//father close pipe in
			
			if(prepipe)	
				do_simple_cmd(i-j, argv+j, prefd, postfd);
			else
				do_simple_cmd(i-j, argv+j, 0, postfd);
			argv[i] = p;
			prepipe = 1;
			prefd[0] = postfd[0];
			prefd[1] = postfd[1];
			j = ++i;
		} else
			i++;
	}
	if(prepipe)
		do_simple_cmd(i-j, argv+j, prefd, 0);
	else 
		do_simple_cmd(i-j, argv+j, 0, 0);
	return 0;
}


static int predo_for_redirect(int argc, char** argv, int* re)
{
	int i;
	int redirect = 0;	
	for(i = 1; i < argc; i++) {
		if(strcasecmp(argv[i], "<") == 0) {
			redirect = 1;
			argv[i] = 0;
			break;
		} else if(strcasecmp(argv[i], ">") == 0) {
			redirect = 2;
			argv[i] = 0;
			break;	
		}
	}
	if(redirect) {// need redirect stdin or stdout
		if(argv[i+1]) {
			int fd;
			if(redirect == 2) {
				if((fd = open(argv[i+1], O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR)) == -1) {
					fprintf(stderr, "Open out %s failed\n", argv[i+1]);
					return 1;
				}
				dup2(fd, STDOUT_FILENO);
			} else {//redirect == 1
				if((fd = open(argv[i+1], O_RDONLY, S_IRUSR|S_IWUSR)) == -1) {
					fprintf(stderr, "Open in %s failed\n", argv[i+1]);
					return 1;
				}
				dup2(fd, STDIN_FILENO);
			}
		} else {
			fprintf(stderr, "Bad redirect, need more arg\n");
			return 1;
		}
	}
	if(re)
		*re = redirect;
	return 0;
}

static int do_simple_cmd(int argc, char** argv, int prefd[], int postfd[])
{
	int pid;
	int status;
	buildin_cmd_handle hd;
	if(argc == 0)
		return 0;

	if(prefd == 0 && postfd == 0) {
	// a very simple buitin command, not have pre and post pipe, father should do the
	// builtin command itself.
		if((hd = get_cmd_handle(argv[0]))) {
			if(predo_for_redirect(argc, argv, 0))
				return 1;
			(*hd)(argc, argv);
			return 0;
		}
	}
	
	if((pid = fork()) == 0) {//child
		// reset the signal INT handle to default
		int redirect = 0;
		signal(SIGINT, SIG_DFL);

		if(predo_for_redirect(argc, argv, &redirect))
			exit(1);
		
		if(redirect != 1 && prefd) {//has a pre pipe, redirect stdin
			close(prefd[1]);
			if(prefd[0] != STDIN_FILENO) {
	//			fprintf(stderr, "redirect stdin\n");
				dup2(prefd[0], STDIN_FILENO);
				close(prefd[0]);
			}
		}
		if(redirect != 2 && postfd) {//has a post pipe, redirect stdout
			close(postfd[0]);
			if(postfd[1] != STDOUT_FILENO) {
	//			fprintf(stderr, "redirect stdout\n");
				dup2(postfd[1], STDOUT_FILENO);
				close(postfd[1]);
			}
		}
		if((hd = get_cmd_handle(argv[0]))) {
			(*hd)(argc, argv);
			exit(0);
		}

//		char buffer[100];
//		if(file_exist(argv[0], buffer)) {
//			execv(buffer, argv);
//		}
//		else {
//			fprintf(stderr, "-msh: %s: command not found\n", argv[0]);
			fprintf(stderr, "%s command not support now.\n", argv[0]);
			exit(0);
//		}
	}
	waitpid(pid, &status, 0);
	if(postfd) { // no
		close(postfd[1]); // must close this fd here.
	}
	return 0;
}


static void reset_args()
{
	int i;
	for(i = 0; i < argcnt; i++) {
		free(argbuf[i]);
		argbuf[i] = 0;
	}
	argcnt = 0;
}

static int my_yyinput(char* buf, int max)
{
	int n;
	n = (max < (mylim-myptr)) ? max : (mylim-myptr);

	if(n > 0) {
		memcpy(buf, myptr, n);
		myptr += n;
	}
	return n;
}

