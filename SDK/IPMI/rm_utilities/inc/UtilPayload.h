
#ifndef _PAYLOAD_UTIL_H_
#define _PAYLOAD_UTIL_H_

#ifndef _PAYLOAD_UTIL_C_
extern void UtilSetUserPayloadAccess(BYTE, BYTE, BYTE, BYTE *);
extern void UtilSuspendOrResumePayloadEncryption(BYTE);
extern const St_PayloadTag PayloadTypeInfo[];
#endif

#endif

