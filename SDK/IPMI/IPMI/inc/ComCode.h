     
#ifndef _COMCODE_H_
#define _COMCODE_H_  

#define COMP_NORMAL                                                        0x00

#define COMP_PAYLOAD_ACTIVE                                                0x80
#define COMP_PAYLOAD_ALREADY_DEACTIVED                                     0x80
#define COMP_PARAMETER_NOT_SUPPORT                                         0x80

#define COMP_PAYLOAD_DISABLE                                               0x81
#define COMP_LOST_ARBITRATION                                              0x81
#define COMP_SET_IN_PROCESS                                                0x81

#define COMP_BUS_ERROR                                                     0x82
#define COMP_READ_ONLY                                                     0x82
#define COMP_PAYLOAD_ACTIVATION_LIMIT_REACH                                0x82
#define COMP_WRITE_ONLY                                                    0x82

#define COMP_CAN_NOT_ACTIVATE_WITH_ENCRYPTION                              0x83
#define COMP_NACK_ON_WRITE                                                 0x83

#define COMP_CAN_NOT_ACTIVATE_WITHOUT_ENCRYPTION                           0x84

#define COMP_NODEBUXY                                                      0xC0

#define COMP_INVALID                                                       0xC1

#define COMP_INVALID_RESERVATION_ID										   0xC5

#define COMP_REQUEST_DATA_TRUNCATED                                        0xC6

#define COMP_DATA_LENGTH_INVALID                                           0xC7

#define COMP_REQUEST_DATA_FIELD_LENGTH_LIMIT_EXCEEDED                      0xC8

#define COMP_OUTOFRANGE                                                    0xC9

#define COMP_REQUEST_SENSOR_DATA_RECORD_NOT_FOUND                          0xCB

#define COMP_INVALIDDATA                                                   0xCC

#define COMP_RESPONSE_NOT_PROVIDED										   0xCE

#define COMP_DESTINATION_UNAVAILABLE                                       0xD3 

#define COMP_PTIVILEGE_FAIL												   0xD4

#define COMP_PARAMETER_NOT_SUPPORT_IN_PRESENT_STATE                        0xD5

#define COMP_SUB_FUNC_HAS_BEEN_DISABLE                                     0xD6

#define COMP_UNSPECIFIED_ERROR											   0xff

#endif  





