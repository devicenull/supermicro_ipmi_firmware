#ifndef SYSLOGLOGUTILS_H
#define SYSLOGLOGUTILS_H

#include <cmpidt.h>

#include "IPMI_LogRecord.h"

CMPIObjectPath *makeLogRecordPath(const CMPIBroker *broker, 
                                  const char * classname,
				  const char * namespace,
				  int j);

CMPIInstance *makeLogRecordInstance(const CMPIBroker *broker, 
                                    const char * classname,
                                    const char * namespace, 
				    char *record_data,
				    int j);

#endif

