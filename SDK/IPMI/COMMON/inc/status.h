
#ifndef _STATUS_H_
#define _STATUS_H_




typedef enum
{
	STATUS_FAIL = 0,
    STATUS_SUCCESS = 1,      
    STATUS_READY,
    STATUS_SUSPEND,
    STATUS_TIMEOUT,
    STATUS_BUSY,
    STATUS_NAK,             
    STATUS_AL,              
    STATUS_TR,               
    STATUS_RECOVERY,
} STATUS;




typedef enum
{
	ACPI_S0 = 0,                
    ACPI_S1 = 0x01,             
    ACPI_S2 = 0x02,             
    ACPI_S3 = 0x03,             
    ACPI_S4 = 0x04,             
    ACPI_S5 = 0x05,             
    ACPI_S4_S5 = 0x06,          
    ACPI_G3 = 0x07,             
    ACPI_SLEEPING = 0x08,       
    ACPI_G1_SLEEPING = 0x09,    
    ACPI_SET_OVERRIDE = 0x0A,   
    ACPI_SET_LEGACY_ON = 0x20,  
    ACPI_SET_LEGACY_OFF = 0x21, 
    ACPI_SET_UNKNOWN = 0x2A,    
    ACPI_NO_CHANGE = 0x7F       
} POWERSTATUS;


#endif  





