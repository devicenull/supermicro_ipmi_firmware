#ifndef SYSLOGLOGUTILS_H
#define SYSLOGLOGUTILS_H

#include <cmpidt.h>

#include "IPMI_PowerManagementService.h"


CMPIObjectPath *makePowerManagementServicePath(const CMPIBroker *broker, 
                                  const char * classname,
				  const char * namespace);

CMPIInstance *makePowerManagementServiceInstance(const CMPIBroker *broker, 
                                    const char * classname,
                                    const char * namespace);

int IPMI_PowerManagementServiceProvider_Operation(const char *method);

#endif

