#ifndef SYSLOGLOGUTILS_H
#define SYSLOGLOGUTILS_H

#include <cmpidt.h>

#include "Discrete_Sensor.h"



CMPIObjectPath *makeDiscrete_SensorPath(const CMPIBroker *broker, 
                                  const char * classname,
				  const char * namespace, 
				  int nn);


CMPIInstance *makeDiscrete_SensorInstance(const CMPIBroker *broker, 
                                    const char * classname,
                                    const char * namespace, 
				    int nn);

#endif

