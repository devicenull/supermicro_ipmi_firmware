/*
 * $Id: mrwlock.c,v 1.2 2005/11/20 17:43:54 bestorga-oss Exp $
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
 * Description: Multiple Reader/Single Writer Locks
 */

#include "mrwlock.h"

int MReadLock(MRWLOCK *mrwl)
{
  if (mrwl && pthread_mutex_lock(&mrwl->mrw_mutex) == 0) {
    mrwl->mrw_rnum += 1;
    return pthread_mutex_unlock(&mrwl->mrw_mutex);
  } else {
    return -1;
  }
}

int MReadUnlock(MRWLOCK *mrwl)
{
  if (mrwl && pthread_mutex_lock(&mrwl->mrw_mutex) == 0) {
    mrwl->mrw_rnum -= 1;
    if (mrwl->mrw_rnum == 0)
      pthread_cond_broadcast(&mrwl->mrw_cond);
    return pthread_mutex_unlock(&mrwl->mrw_mutex);
  } else {
    return -1;
  }
}

int MWriteLock(MRWLOCK *mrwl)
{
  if (mrwl && pthread_mutex_lock(&mrwl->mrw_mutex) == 0) {
    while (mrwl->mrw_rnum) 
      pthread_cond_wait(&mrwl->mrw_cond,&mrwl->mrw_mutex);
    return 0;
  } else {
    return -1;
  }
}

int MWriteUnlock(MRWLOCK *mrwl)
{
  if (mrwl && pthread_mutex_unlock(&mrwl->mrw_mutex) == 0) {
    return 0; 
  } else {
    return -1;
  }
}

int MRWInit(MRWLOCK *mrwl)
{
  static pthread_mutex_t mi=PTHREAD_MUTEX_INITIALIZER;
  static pthread_cond_t ci=PTHREAD_COND_INITIALIZER;
  mrwl->mrw_mutex=mi;
  mrwl->mrw_cond=ci;
  mrwl->mrw_rnum=0;
  return 0;
}
