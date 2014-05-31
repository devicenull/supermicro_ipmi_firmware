#include "cmpidt.h"
#include "cmpimacs.h"
typedef struct
{  
	CMPIInstance * (* makeLogRecordInstanceCallback) (							
														const CMPIBroker *broker, 
							                            const char *classname, 
									    				const char *namespace, 
									    				char *record_data,
									    				int j
				  									);
	
}LogRecordCallbackFunctionType;
