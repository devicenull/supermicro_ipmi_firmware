#ifndef _HWM_H
#define _HWM_H

// HWM structure
typedef struct
{
	unsigned char bank;
	unsigned char offset;
	unsigned char value;
} HW_MONITOR_CONFIG;
  

typedef struct
{
    BYTE b_BankReg;
    BYTE b_I2CBusID;
    BYTE b_SlaveAdd;    
} SensorChipInfoType;

#endif
