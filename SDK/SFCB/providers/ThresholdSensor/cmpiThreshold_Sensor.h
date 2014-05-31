#ifndef SYSLOGLOGUTILS_H
#define SYSLOGLOGUTILS_H

#include <cmpidt.h>

#include "Threshold_Sensor.h"

CMPIObjectPath *makeThreshold_SensorPath(const CMPIBroker *broker, 
                                  const char * classname,
				  const char * namespace, 
				  int nn);


CMPIInstance *makeThreshold_SensorInstance(const CMPIBroker *broker, 
                                    const char * classname,
                                    const char * namespace, 
				    int nn);

#endif

