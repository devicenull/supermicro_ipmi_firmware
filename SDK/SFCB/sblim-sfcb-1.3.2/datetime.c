
/*
 * datetime.c
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
 * Author:        Frank Scheffler
 * Contributions: Adrian Schuur <schuur@de.ibm.com>
 *
 * Description:
 *
 * CMPIDateTime implementation.
 *
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include "native.h"

/*!
  This structure stores the information needed to represent time for
  CMPI providers.
 */
struct native_datetime {
   CMPIDateTime dt;             /*!< the inheriting data structure  */
   int refCount;
   int mem_state;               /*!< states, whether this object is
                                   registered within the memory mangagement or
                                   represents a cloned object */
   char cimDt[26];
};


static struct native_datetime *__new_datetime(int state, const char* cimDt, CMPIStatus *rc);

/****************************************************************************/

//! Releases a previously cloned CMPIDateTime object from memory.
/*!
  To achieve this, the object is simply added to the thread-based memory
  management to be freed later.

  \param dt the object to be released

  \return CMPI_RC_OK.
 */
static CMPIStatus __dtft_release(CMPIDateTime * dt)
{
   struct native_datetime *ndt = (struct native_datetime *) dt;

   if (ndt->mem_state && ndt->mem_state != MEM_RELEASED) {
      memUnlinkEncObj(ndt->mem_state);
      ndt->mem_state = MEM_RELEASED;
      free(ndt);
      CMReturn(CMPI_RC_OK);
   }
    
   CMReturn(CMPI_RC_ERR_FAILED);
}


//! Clones an existing CMPIDateTime object.
/*!
  The function simply calls __new_datetime() with the data fields
  extracted from dt.

  \param dt the object to be cloned
  \param rc return code pointer

  \return a copy of the given CMPIDateTime object that won't be freed
  from memory before calling __dtft_release().
 */
static CMPIDateTime *__dtft_clone(const CMPIDateTime * dt, CMPIStatus * rc)
{
   struct native_datetime *ndt = (struct native_datetime *) dt;
   struct native_datetime *new = __new_datetime(MEM_NOT_TRACKED, ndt->cimDt,rc);
   return (CMPIDateTime *) new;
}


CMPIUint64 chars2bin(const char *string, CMPIStatus * rc)
{
   CMPIUint64 msecs,secs;
   CMPIBoolean interval;
   char *str;
   int offset=0;

   str = strdup(string);
   interval = (str[21] == ':');
   if ((str[21] == '+') ||  (str[21] == '-')) {
      // If we found an offset in the timestamp
      // convert it to seconds, and save it
      offset= atoi(str+21) * 60ULL;
   }

// 0000000000111111111122222  
// 0123456789012345678901234
// yyyymmddhhmmss mmmmmmsutc 
// 20050503104354.000000:000
// 20080813104354.000000+500
   
   str[21] = 0;
   msecs = strtoull(str+15,NULL,10);
   
   str[14] = 0;
   secs = strtoull(str+12,NULL,10);
   str[12] = 0;
   secs += strtoull(str+10,NULL,10) * 60ULL;
   str[10] = 0;
   secs += strtoull(str+8,NULL,10) * 60ULL * 60ULL;
   str[8] = 0;

   if (interval) {
      secs += strtoull(str,NULL,10) * 60ULL * 60ULL * 24ULL;
      msecs=msecs+(secs*1000000ULL);
   }
   
   else {
      struct tm tmp;

      memset(&tmp, 0, sizeof(struct tm));
      tzset();

      tmp.tm_gmtoff = timezone;
      tmp.tm_isdst = daylight;
      tmp.tm_mday = atoi(str + 6);
      str[6] = 0;
      tmp.tm_mon = atoi(str + 4) - 1;
      str[4] = 0;
      tmp.tm_year = atoi(str) - 1900;

      msecs=msecs+(secs*1000000ULL);
      msecs += (CMPIUint64) mktime(&tmp) * 1000000ULL;
      // Add in the offset
      msecs -= offset * 1000000ULL;
   }

   free(str);

   return msecs;
}

static void bin2chars(CMPIUint64 msecs, CMPIBoolean interval, CMPIStatus * rc, char *str_time)
{
   time_t secs = msecs / 1000000ULL;
   unsigned long usecs = msecs % 1000000ULL;

   if (interval) {

      unsigned long long useconds, seconds, mins, hrs, days;
      seconds= msecs / 1000000ULL;
      useconds = msecs % 1000000ULL;

      mins = seconds / 60ULL;
      seconds %= 60ULL;
      hrs = mins / 60ULL;
      mins %= 60ULL;
      days = hrs / 24ULL;
      hrs %= 24ULL;

      sprintf(str_time, "%8.8llu%2.2llu%2.2llu%2.2llu.%6.6llu:000",
              days, hrs, mins, seconds, useconds);
   }
   
   else {
      struct tm tm_time;
      char us_utc_time[11];

      if (localtime_r(&secs, &tm_time) == NULL) {

         if (rc)
            CMSetStatus(rc, CMPI_RC_ERR_FAILED);
         return;
      }

      tzset();

      snprintf(us_utc_time, 11, "%6.6ld%+4.3ld",
               usecs, (daylight != 0) * 60 - timezone / 60);

      strftime(str_time, 26, "%Y%m%d%H%M%S.", &tm_time);

      strcat(str_time, us_utc_time);
   }
}

//! Extracts the binary time from the encapsulated CMPIDateTime object.
/*!
  \param dt the native CMPIDateTime to be extracted.
  \param rc return code pointer

  \return an amount of microseconds.
 */
static CMPIUint64 __dtft_getBinaryFormat(const CMPIDateTime * dt, CMPIStatus * rc)
{
   struct native_datetime *ndt = (struct native_datetime *) dt;

   if (rc)  CMSetStatus(rc, CMPI_RC_OK);
   return chars2bin(ndt->cimDt,rc);
}

//! Gives back a string representation of the time object.
/*!
  \param dt the native CMPIDateTime to be converted.
  \param rc return code pointer

  \return a string that has one of the following formats:

  - yyyymmddhhmmss.mmmmmmsutc (for absolute times)
  - ddddddddhhmmss.mmmmmm:000 (for time intervals)
 */

void dateTime2chars(CMPIDateTime * dt, CMPIStatus * rc, char *str_time)
{
   struct native_datetime *ndt = (struct native_datetime *) dt;
   
   strcpy(str_time,ndt->cimDt);
   if (rc) CMSetStatus(rc, CMPI_RC_OK);
}

static CMPIString *__dtft_getStringFormat(const CMPIDateTime * dt, CMPIStatus * rc)
{
   struct native_datetime *ndt = (struct native_datetime *) dt;

   if (rc) CMSetStatus(rc, CMPI_RC_OK);
   return sfcb_native_new_CMPIString(ndt->cimDt, rc, 0);
}

static CMPIBoolean __dtft_isInterval(const CMPIDateTime * dt, CMPIStatus * rc)
{
   struct native_datetime *ndt = (struct native_datetime *) dt;

   if (rc) CMSetStatus(rc, CMPI_RC_OK);
   return ndt->cimDt[21]==':' ? 1 : 0;
}

static CMPIDateTimeFT dtft = {
   NATIVE_FT_VERSION,
   __dtft_release,
   __dtft_clone,
   __dtft_getBinaryFormat,
   __dtft_getStringFormat,
   __dtft_isInterval
};


//! Creates a new native_datetime object.
/*!
  The newly allocated object's function table is initialized to point
  to the native functions in this file.

  \param mm_add MEM_TRACKED for a regular object, MEM_NOT_TRACKED for
  cloned ones
  \param msecs the binary time to be stored
  \param interval the interval flag to be stored
  \param rc return code pointer

  \return a fully initialized native_datetime object pointer.
 */
static struct native_datetime *__new_datetime(int mm_add,
                                              const char *cimDt,
                                              CMPIStatus * rc)
{
   static CMPIDateTime dt = {
      "CMPIDateTime",
      &dtft
   };
   struct native_datetime ndt,*tNdt;
   int state;
   
   ndt.dt = dt;
   tNdt=memAddEncObj(mm_add, &ndt, sizeof(ndt), &state);
   tNdt->mem_state = state;   
   tNdt->refCount=0;   
   strcpy(tNdt->cimDt,cimDt);
   
   if (rc) CMSetStatus(rc, CMPI_RC_OK);
   return (struct native_datetime*)tNdt;

}


//! Creates a native CMPIDateTime representing the current time.
/*!
  This function calculates the current time and stores it within
  a new native_datetime object.
  
  \param rc return code pointer

  \return a pointer to a native CMPIDateTime.
 */
static CMPIDateTime *_new_CMPIDateTime(CMPIStatus * rc, int mm_add)
{
   struct timeval tv;
   struct timezone tz;
   CMPIUint64 msecs;
   char cimDt[26];

   gettimeofday(&tv, &tz);

   msecs = (CMPIUint64) 1000000 *(CMPIUint64) tv.tv_sec
       + (CMPIUint64) tv.tv_usec;
   
   bin2chars(msecs, 0, rc, cimDt);

   return (CMPIDateTime *) __new_datetime(mm_add, cimDt, rc);
}

CMPIDateTime *sfcb_native_new_CMPIDateTime(CMPIStatus * rc)
{
   return _new_CMPIDateTime(rc, MEM_TRACKED);;
}

CMPIDateTime *NewCMPIDateTime(CMPIStatus * rc)
{
   return _new_CMPIDateTime(rc, MEM_NOT_TRACKED);;
}


//! Creates a native CMPIDateTime given a fixed binary time.
/*!
  This calls is simply passed on to __new_datetime().

  \param time fixed time-stamp in microseconds
  \param interval states, if the time-stamp is to be treated as interval
  \param rc return code pointer

  \return a pointer to a native CMPIDateTime.
  
  \sa __dtft_getBinaryFormat()
 */
static CMPIDateTime *_new_CMPIDateTime_fromBinary(CMPIUint64 msecs,
                                                 CMPIBoolean interval,
                                                 CMPIStatus * rc, int mm_add)
{
   char cimDt[26];
   bin2chars(msecs, interval, rc, cimDt);
   return (CMPIDateTime*) __new_datetime(mm_add, cimDt, rc);
}

CMPIDateTime *sfcb_native_new_CMPIDateTime_fromBinary(CMPIUint64 msecs,
                                                 CMPIBoolean interval,
                                                 CMPIStatus * rc)
{
   return _new_CMPIDateTime_fromBinary(msecs,interval,rc,MEM_TRACKED);
}

CMPIDateTime *NewCMPIDateTimeFromBinary(CMPIUint64 msecs,
                                                 CMPIBoolean interval,
                                                 CMPIStatus * rc)
{
   return _new_CMPIDateTime_fromBinary(msecs,interval,rc,MEM_NOT_TRACKED);
}


//! Creates a native CMPIDateTime given a fixed time in string representation.
/*!
  This function assumes the given string to have one of the following formats:
  
  - for absolute times: yyyymmddhhmmss.mmmmmmsutc
  - for time intervals: ddddddddhhmmss.mmmmmm:000

  \param string the time to be converted into internal representation
  \param rc return code pointer #include <limits.h>


  \return a pointer to a native CMPIDateTime.

  \sa __dtft_getStringFormat()
 */
static CMPIDateTime *_new_CMPIDateTime_fromChars(const char *string,
                                                CMPIStatus * rc, int mm_add)
{
   if (string == NULL || strlen(string)!=25 ||
       (string[21]!='-' && string[21]!='+' && string[21]!=':')) {
      if (rc) CMSetStatus(rc, CMPI_RC_ERR_INVALID_PARAMETER);
      return NULL;
   }

   return (CMPIDateTime *) __new_datetime(mm_add, string, rc);
}

CMPIDateTime *sfcb_native_new_CMPIDateTime_fromChars(const char *string,
                                                CMPIStatus * rc)
{
   return  _new_CMPIDateTime_fromChars(string,rc,MEM_TRACKED);
}

CMPIDateTime *NewCMPIDateTimeFromChars(const char *string, CMPIStatus * rc)
{
   return  _new_CMPIDateTime_fromChars(string,rc,MEM_NOT_TRACKED);
}

/****************************************************************************/

/*** Local Variables:  ***/
/*** mode: C           ***/
/*** c-basic-offset: 8 ***/
/*** End:              ***/
