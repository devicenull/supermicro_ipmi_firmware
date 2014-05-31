#ifndef _UTILUSBHID_H
#define	_UTILUSBHID_H

#include "../../COMMON/inc/type.h"
#include "../../COMMON/inc/status.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define ATEN_MOUSE_IOC_MAGIC  'b'
#define ATEN_MOUSE_IOCHOTPLUG _IOW(ATEN_MOUSE_IOC_MAGIC, 1, struct mouseCtlInfo_t)
#define ATEN_MOUSE_IOCGETMODE _IOR(ATEN_MOUSE_IOC_MAGIC, 2, struct mouseCtlInfo_t)
#define ATEN_MOUSE_IOCSETMODE _IOW(ATEN_MOUSE_IOC_MAGIC, 3, struct mouseCtlInfo_t)
#define ATEN_MOUSE_IOCRESYNC  _IOW(ATEN_MOUSE_IOC_MAGIC, 4, struct mouseCtlInfo_t)

typedef enum {
    UNKNOWN = 0,
    ABMOUSE,
    NOMMOUSE,
    SGMOUSE
} En_MouseModeTag;

struct mouseCtlInfo_t {
    En_MouseModeTag En_MouseMode;
};

#ifdef _UTILUSBHID_C

STATUS UtilUsbHidInit(void);
int UtilUsbHidHotPlug(void);
En_MouseModeTag UtilUsbHidGetMouseMode(void);
int UtilUsbHidSetMouseMode(En_MouseModeTag En_MouseMode);

#else

extern STATUS UtilUsbHidInit(void);
extern int UtilUsbHidHotPlug(void);
extern En_MouseModeTag UtilUsbHidGetMouseMode(void);
extern int UtilUsbHidSetMouseMode(En_MouseModeTag En_MouseMode);

#endif

#ifdef	__cplusplus
}
#endif

#endif	

