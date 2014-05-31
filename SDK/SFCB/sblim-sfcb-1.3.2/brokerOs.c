
/*
 * brokerOs.c
 *
 * (C) Copyright IBM Corp. 2005
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:       Adrian Schuur <schuur@de.ibm.com>
 *
 * Description:
 *
 * This module implements OS dependent CMPI broker functions.
 *
*/


#include <pthread.h>
#include "native.h"
#include <stdlib.h>

static char *resolveFileName(const char *filename)
{
   char dlName[1024];
#if defined(CMPI_PLATFORM_WIN32_IX86_MSVC)
   strcpy(dlName, filename);
   strcat(dlName, ".dll");
#elif defined(CMPI_PLATFORM_LINUX_GENERIC_GNU)
   strcpy(dlName, "lib");
   strcat(dlName, filename);
   strcat(dlName, ".so");
#elif defined(CMPI_OS_HPUX)
#ifdef CMPI_PLATFORM_HPUX_PARISC_ACC
   strcpy(dlName, "lib");
   strcat(dlName, filename);
   strcat(dlName, ".so");
#else
   strcpy(dlName, "lib");
   strcat(dlName, filename);
   strcat(dlName, ".so");
#endif
#elif defined(CMPI_OS_OS400)
   strcpy(dlName, filename);
#elif defined(CMPI_OS_DARWIN)
   strcpy(dlName, "lib");
   strcat(dlName, filename);
   strcat(dlName, ".dylib");
#else
   strcpy(dlName, "lib");
   strcat(dlName, filename);
   strcat(dlName, ".so");
#endif

   return strdup(dlName);
}

static CMPI_THREAD_TYPE newThread
    (CMPI_THREAD_RETURN(CMPI_THREAD_CDECL * start) (void *), void *parm,
     int detached) {
#if defined(CMPI_PLATFORM_LINUX_GENERIC_GNU)
   pthread_t t;
   pthread_attr_t tattr;
   if (detached) {
      pthread_attr_init(&tattr);
      pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED);
      pthread_create(&t, &tattr, (void *(*)(void *)) start, parm);
   }
   else
      pthread_create(&t, NULL, (void *(*)(void *)) start, parm);
   return (CMPI_THREAD_TYPE) t;
#else
#error Platform not yet supported
#endif
}

static int threadOnce(int *once, void (*init) (void))
{
#if defined(CMPI_PLATFORM_LINUX_GENERIC_GNU)
   return pthread_once(once, init);

#else
#error Platform not yet supported
#endif
}




static int createThreadKey(CMPI_THREAD_KEY_TYPE * key, void (*cleanup) (void *))
{
#if defined(CMPI_PLATFORM_LINUX_GENERIC_GNU)
   return pthread_key_create((pthread_key_t *) key, cleanup);
#else
#error Platform not yet supported
#endif
}

static int destroyThreadKey(CMPI_THREAD_KEY_TYPE key)
{
#if defined(CMPI_PLATFORM_LINUX_GENERIC_GNU)
   return pthread_key_delete(key);
#else
#error Platform not yet supported
#endif
}

static void *getThreadSpecific(CMPI_THREAD_KEY_TYPE key)
{
#if defined(CMPI_PLATFORM_LINUX_GENERIC_GNU)
   return pthread_getspecific(key);
#else
#error Platform not yet supported
#endif
}

static int setThreadSpecific(CMPI_THREAD_KEY_TYPE key, void *value)
{
#if defined(CMPI_PLATFORM_LINUX_GENERIC_GNU)
   return pthread_setspecific(key, value);
#else
#error Platform not yet supported
#endif
}



static CMPI_MUTEX_TYPE newMutex(int opt)
{
#if defined(CMPI_PLATFORM_LINUX_GENERIC_GNU)
   static pthread_mutex_t tmpl = PTHREAD_MUTEX_INITIALIZER;
   int state = 0;
   pthread_mutex_t *m = memAlloc(MEM_TRACKED, sizeof(pthread_mutex_t), &state);
   *m = tmpl;
   return m;
#else
#error Platform not yet supported
#endif
}

static void destroyMutex(CMPI_MUTEX_TYPE m)
{
#if defined(CMPI_PLATFORM_LINUX_GENERIC_GNU)
   free(m);
#else
#error Platform not yet supported
#endif
}

static void lockMutex(CMPI_MUTEX_TYPE m)
{
#if defined(CMPI_PLATFORM_LINUX_GENERIC_GNU)
   pthread_mutex_lock((pthread_mutex_t *) m);
#else
#error Platform not yet supported
#endif
}

static void unlockMutex(CMPI_MUTEX_TYPE m)
{
#if defined(CMPI_PLATFORM_LINUX_GENERIC_GNU)
   pthread_mutex_unlock((pthread_mutex_t *) m);
#else
#error Platform not yet supported
#endif
}



static CMPI_COND_TYPE newCondition(int opt)
{
#if defined(CMPI_PLATFORM_LINUX_GENERIC_GNU)
   pthread_cond_t tmpl = PTHREAD_COND_INITIALIZER;
   pthread_cond_t *c = calloc(1, sizeof(pthread_cond_t));
   *c = tmpl;
   return c;
#else
#error Platform not yet supported
#endif
}

static void destroyCondition(CMPI_COND_TYPE c)
{
#if defined(CMPI_PLATFORM_LINUX_GENERIC_GNU)
   free(c);
#else
#error Platform not yet supported
#endif
}

static int timedCondWait(CMPI_COND_TYPE c, CMPI_MUTEX_TYPE m,
                         struct timespec *wait)
{
#if defined(CMPI_PLATFORM_LINUX_GENERIC_GNU)
   return pthread_cond_timedwait((pthread_cond_t *) c,
                                 (pthread_mutex_t *) m, wait);
#else
#error Platform not yet supported
#endif
}

static int condWait(CMPI_COND_TYPE c, CMPI_MUTEX_TYPE m)
{
#if defined(CMPI_PLATFORM_LINUX_GENERIC_GNU)
   return pthread_cond_wait((pthread_cond_t *) c, (pthread_mutex_t *) m);
#else
#error Platform not yet supported
#endif
}



CMPIBrokerExtFT brokerExt_FT = {
   CMPICurrentVersion,
   resolveFileName,
   newThread,
   NULL,                        // Join not implemented yet
   NULL,                        // exit not implemented yet
   NULL,                        // cancel not implemented yet
   NULL,                        // sleep not implemented yet
   threadOnce,

   createThreadKey,
   destroyThreadKey,
   getThreadSpecific,
   setThreadSpecific,

   newMutex,
   destroyMutex,
   lockMutex,
   unlockMutex,

   newCondition,
   destroyCondition,
   condWait,
   timedCondWait,
   NULL                         // Signal not supported yet
};

CMPIBrokerExtFT *CMPI_BrokerExt_Ftab = &brokerExt_FT;
