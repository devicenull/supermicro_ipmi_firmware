/*
 * $Id: mrwlock.h,v 1.2 2005/11/20 17:43:54 bestorga-oss Exp $
 *
 * (C) Copyright IBM Corp. 2003
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
 * Description: Multiple Reader/Single Writer Lock
 * This module facilitates scalable multiple reader single writer
 * processing paradigms.
 *
 */

#ifndef MRWLOCK_H
#define MRWLOCK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>

typedef struct _MRWLOCK {
  pthread_mutex_t mrw_mutex;
  pthread_cond_t  mrw_cond;
  unsigned        mrw_rnum;
} MRWLOCK;

/* Macro for stativ MRW Lock Definition */
#define MRWLOCK_DEFINE(n) MRWLOCK n={PTHREAD_MUTEX_INITIALIZER, \
                                     PTHREAD_COND_INITIALIZER, \
                                     0}
extern int MRWInit(MRWLOCK *mrwl);
extern int MRWTerm(MRWLOCK *mrwl);
extern int MReadLock(MRWLOCK *mrwl);
extern int MReadUnlock(MRWLOCK *mrwl);
extern int MWriteLock(MRWLOCK *mrwl);
extern int MWriteUnlock(MRWLOCK *mrwl);

#ifdef __cplusplus
}
#endif

#endif

