/*
 * $Id: mlog.c,v 1.4 2005/11/20 17:43:54 bestorga-oss Exp $
 *
 * (C) Copyright IBM Corp. 2003, 2004
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:       Viktor Mihajlovski <mihajlov@de.ibm.cim>
 * Contributors: Michael Schuele <schuelem@de.ibm.com>
 *               Anas Nashif <nashif@planux.com>
 *
 * Description: Logger support.
 *
 */

const char *_mlog_id = "$Id: mlog.c,v 1.4 2005/11/20 17:43:54 bestorga-oss Exp $";

#include "mlog.h"
#include <syslog.h>
#include <stdarg.h>
#include <stdio.h>

void startLogging(const char *name)
{
  openlog(name,LOG_PID,LOG_DAEMON);
  setlogmask(LOG_UPTO(LOG_INFO));
}

void mlogf(int priority, int errout, const char *fmt, ...)
{
  va_list ap,apc;
  int priosysl;

  char buf[4096];

  switch (priority) {
  case M_DEBUG:
    priosysl=LOG_DEBUG;
    break;
  case M_INFO:
    priosysl=LOG_INFO;
    break;
  case M_ERROR:
  default:
    priosysl=LOG_ERR;
    break;
  }
  va_start(ap,fmt);
  
  vsnprintf(buf,4096,fmt,ap);
  syslog(priosysl,buf);

  if (errout) {
    va_start(apc,fmt);
    vfprintf(stderr,fmt,apc);
    va_end(apc);
  }
  va_end(ap);
}

