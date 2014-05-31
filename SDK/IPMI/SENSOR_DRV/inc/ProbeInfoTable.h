
#ifndef _PROBE_INFO_TABLE_H_
#define _PROBE_INFO_TABLE_H_


#define TABLE_TAIL                                      0xFF

#define SENSOR_SCAN_INTERVAL_ONE_SECOND                 2
#define SENSOR_SCAN_INTERVAL_TWO_SECOND                 4
#define SENSOR_SCAN_INTERVAL_TEN_SECOND                 20
#define SENSOR_SCAN_AT_ACPI_S0							(0x00 << 8)
#define SENSOR_SCAN_AT_ACPI_S1							(0x01 << 8)
#define SENSOR_SCAN_AT_ACPI_S2							(0x02 << 8)
#define SENSOR_SCAN_AT_ACPI_S3							(0x03 << 8)
#define SENSOR_SCAN_AT_ACPI_S4							(0x04 << 8)
#define SENSOR_SCAN_AT_ACPI_S5							(0x05 << 8)


typedef struct
{
    BYTE b_SnrNo;
    
    DWORD dw_HWInfo1;

    DWORD dw_HWInfo2;

    St_SensorCallBackAPITag *p_CallBackPtr;
    
    St_SensorAPITag  *p_FncPtr;
}ProbeInfoTableType;

extern STATUS Register_ProbeInfoCallBack(WORD); 
#ifndef _PROBE_INFO_TABLE_C_
extern  ProbeInfoTableType ProbeTable[];
#endif
#endif  





