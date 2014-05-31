#ifndef SYSLOGLOGUTILS_H
#define SYSLOGLOGUTILS_H

#include <cmpidt.h>

#include "IPMI_TextRedirectionService.h"


CMPIObjectPath *makeTextRedirectionServicePath(const CMPIBroker *broker, 
                                               const char * classname,
                                               const char * namespace);

CMPIInstance *makeTextRedirectionServiceInstance(const CMPIBroker *broker, 
                                                 const char * classname,
                                                 const char * namespace);

int IPMI_TextRedirectionServiceProvider_Operation(const char *method);

#endif

