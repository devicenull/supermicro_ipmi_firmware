/*
 * cimslp.c
 *
 * (C) Copyright IBM Corp. 2006
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:        Sven Schuetz <sven@de.ibm.com>
 * Contributions:
 *
 * Description:
 *
 * Control functions, main if running standlone, or start thread
 * function if running in sfcb
 *
*/

#include <getopt.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <slp.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

#include "config.h"
#include "cimslpCMPI.h"
#include "cimslpSLP.h"
#include "cimslp.h"
#include "trace.h"

#ifdef HAVE_SLP
#include "control.h"
#endif


extern void addStartedAdapter(int pid);

void freeCFG(cimomConfig *cfg) {
	
	free(cfg->cimhost);
	free(cfg->cimpassword);
	free(cfg->cimuser);
	free(cfg->commScheme);
	free(cfg->port);
}

void setUpDefaults(cimomConfig *cfg) {
	cfg->commScheme = strdup("http");
	cfg->cimhost = strdup("localhost");
	cfg->port = strdup("5988");
	cfg->cimuser = strdup("");
	cfg->cimpassword = strdup("");
	cfg->keyFile = NULL;
	cfg->trustStore = NULL;
	cfg->certFile = NULL;	
}

void setUpTimes(int * slpLifeTime, int * sleepTime) {
	if (*slpLifeTime < 16) {
		*slpLifeTime = 16;
	}
	if (*slpLifeTime > SLP_LIFETIME_MAXIMUM) {
		*slpLifeTime = SLP_LIFETIME_DEFAULT;
	}
		
	*sleepTime = *slpLifeTime -15;		
}

static void handleSigUsr1(int sig)
{
	deregisterCIMService();
	mlogf(M_INFO,M_SHOW,"--- %s terminating %d\n",processName,getpid());
	exit(0);
}

static void handleSigHup(int sig)
{
	mlogf(M_INFO,M_SHOW,"--- %s restarting %d\n",processName,getpid());
}

#ifdef HAVE_SLP

void forkSLPAgent(cimomConfig cfg, int slpLifeTime, int sleepTime)
{
    cimSLPService service;
    int pid;
    pid = fork();
    if (pid < 0) {
      char *emsg=strerror(errno);
      mlogf(M_ERROR,M_SHOW, "-#- slp agent fork: %s",emsg);
      exit(2);
    }
    if (pid == 0) {
	setSignal(SIGUSR1, handleSigUsr1,0);
	setSignal(SIGINT, SIG_IGN,0);
	setSignal(SIGTERM, SIG_IGN,0);
	setSignal(SIGHUP, handleSigHup,0);
	if(strcasecmp(cfg.commScheme, "http") == 0) {
		processName = "SLP Agent for HTTP Adapter";
	} else {
		processName = "SLP Agent for HTTPS Adapter";
	}
    	while(1) {
    		service = getSLPData(cfg);
    		registerCIMService(service, slpLifeTime);
    		sleep(sleepTime);
    	}
      	/*if awaked-exit*/
      	exit(0);
    } else {
        slppid=pid;
        addStartedAdapter(pid);
    }
}

int slpAgent()
{
	int slpLifeTime = SLP_LIFETIME_DEFAULT;
	int sleepTime;
	cimomConfig cfgHttp, cfgHttps;
	int enableHttp,enableHttps=0;

	extern char * configfile;

	_SFCB_ENTER(TRACE_SLP, "slpAgent");	
	
	setupControl(configfile);
	
	setUpDefaults(&cfgHttp);	
	setUpDefaults(&cfgHttps);

	sleep(1);

	long i;
	
	if (!getControlBool("enableHttp", &enableHttp)) {
		getControlNum("httpPort", &i);
		free(cfgHttp.port);
		cfgHttp.port = malloc(6 * sizeof(char)); //portnumber has max. 5 digits
		sprintf(cfgHttp.port, "%d", (int)i);
	}
	if (!getControlBool("enableHttps", &enableHttps)) {
		free(cfgHttps.commScheme);
		cfgHttps.commScheme = strdup("https");
		getControlNum("httpsPort", &i);
		free(cfgHttps.port);
		cfgHttps.port = malloc(6 * sizeof(char)); //portnumber has max. 5 digits		
		sprintf(cfgHttps.port, "%d", (int)i);
		getControlChars("sslClientTrustStore", &cfgHttps.trustStore);
		getControlChars("sslCertificateFilePath:", &cfgHttps.certFile);
		getControlChars("sslKeyFilePath", &cfgHttps.keyFile);
	}
	
	getControlNum("slpRefreshInterval", &i);
	slpLifeTime = (int)i;
	setUpTimes(&slpLifeTime, &sleepTime);
	
	if(enableHttp) forkSLPAgent(cfgHttp, slpLifeTime, sleepTime);
	if(enableHttps) forkSLPAgent(cfgHttps, slpLifeTime, sleepTime);
    freeCFG(&cfgHttp);
    freeCFG(&cfgHttps);
	_SFCB_RETURN(0);
}
#endif

#ifdef HAVE_SLP_ALONE

int main(int argc, char *argv[])
{
	int c,j;
	int slpLifeTime = SLP_LIFETIME_DEFAULT;
	int sleepTime;
	cimSLPService as; //Service which is going to be advertised
	
	cimomConfig cfg;
	
	setUpDefaults(&cfg);
	

	
	static const char * help[] =
	{
	"Options:",
	" -c, --cimhost      hostname",
	" -n, --hostport     portnumber",
	" -u, --cimuser      cimserver username",
	" -p, --cimpassword  cimserver password ",
	" -s, --commscheme   http or https",
	" -l, --lifetime     SLP liftime and cimslp process refresh/sleep time in seconds",	
	" -h, --help         display this text\n",
	NULL
	};
	
	
	
	static struct option const long_options[] =
	{
		{ "cimhost", required_argument, 0, 'c' },
		{ "hostport", required_argument, 0, 'n' },
		{ "cimuser", required_argument, 0, 'u' },
		{ "cimpassword", required_argument, 0, 'p' },
		{ "commscheme", required_argument, 0, 's' },
		{ "lifetime", required_argument, 0, 's' },		
		{ "help", no_argument, 0, 'h' },
		{ 0, 0, 0, 0 }
	};

	while ((c = getopt_long(argc, argv, "c:n:u:p:s:l:h", long_options, 0)) != -1)
	{
		switch(c)
		{
			case 0:
				break;
			case 'c':
				free(cfg.cimhost);
				cfg.cimhost = strdup(optarg);
				break;
			case 'n':
				free(cfg.port);
				cfg.port = strdup(optarg);
				break;
			case 'u':
				free(cfg.cimuser);
				cfg.cimuser = strdup(optarg);
				break;
			case 'p':
				free(cfg.cimpassword);
				cfg.cimpassword = strdup(optarg);
				break;
			case 's':
				free(cfg.commScheme);
				cfg.commScheme = strdup(optarg);
				break;
			case 'l':
				slpLifeTime = atoi(optarg);
				break;				
			case 'h':
				for(j=0; help[j] != NULL; j++) {
					printf("%s\n", help[j]);
				}
				exit(0);
			default:
				for(j=0; help[j] != NULL; j++) {
					printf("%s\n", help[j]);
				}
				exit(0);
				break;
		}
	}
	
	setUpTimes(&slpLifeTime, &sleepTime);
		
	int rt;
	while(1) {
		as = getSLPData(cfg);
		rt = registerCIMService(as, slpLifeTime);
		if(rt == 1) {
			printf("No url-string could be constructed. This is usually due to a lacking CIM_ObjectManager instance.\n");
					exit(0);
		}
		if(rt < 0) {
			printf("Error registering with slpd ... I will try again next interval.\n");
		}		
		sleep(sleepTime);
	}
	
	freeCFG(&cfg);
	//return 0;
}
#endif
