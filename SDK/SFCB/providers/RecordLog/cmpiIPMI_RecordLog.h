#ifndef SYSLOGLOGUTILS_H
#define SYSLOGLOGUTILS_H

#include <cmpidt.h>

#include "IPMI_RecordLog.h"


CMPIObjectPath *makeRecordLogPath(const CMPIBroker *broker, 
                                  const char * classname,
				  const char * namespace);

CMPIInstance *makeRecordLogInstance(const CMPIBroker *broker, 
                                    const char * classname,
                                    const char * namespace);

int IPMI_RecordLogProvider_Operation(const char *method);
#endif

