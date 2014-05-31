/*
 * $Id: mlog.h,v 1.3 2005/11/20 17:43:54 bestorga-oss Exp $
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
 * Contributors: 
 *
 * Description: Logging
 *
 */

#ifndef MLOG_H
#define MLOG_H

#define M_DEBUG 1
#define M_INFO  2
#define M_ERROR 3

#define M_SHOW  1
#define M_QUIET 0

void startLogging(const char *name);
void mlogf(int priority, int errout, const char* fmt, ...);

#endif
