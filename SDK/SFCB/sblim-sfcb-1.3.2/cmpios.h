
/*
 * cmpimacs.h
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
 * Author:        Adrian Schuur <schuur@de.ibm.com>
 *
 * Description:
 *
 * CMPI os support macros.
 *
*/
/* ------------------------------------------------------------------------- */
/*                                                                           */
/* Copyright (c) 2006 The Open Group                                         */
/*                                                                           */
/* Permission is hereby granted, free of charge, to any person obtaining a   */
/* copy of this software (the "Software"), to deal in the Software without   */
/* restriction, including without limitation the rights to use, copy,        */
/* modify, merge, publish, distribute, sublicense, and/or sell copies of     */
/* the Software, and to permit persons to whom the Software is furnished     */
/* to do so, subject to the following conditions:                            */
/*                                                                           */
/* The above copyright notice and this permission notice shall be included   */
/* in all copies or substantial portions of the Software.                    */
/*                                                                           */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS   */
/* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF                */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.    */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY      */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT */
/* OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR  */
/* THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                */
/*                                                                           */
/* ------------------------------------------------------------------------- */

#ifndef _CMPIOS_H_
#   define _CMPIOS_H_

#   include "cmpipl.h"
#   include <stdlib.h>          // To get the size_t
#   define CMPI_THREAD_RETURN      void*
#   define CMPI_THREAD_TYPE        void*
#   define CMPI_MUTEX_TYPE         void*
#   define CMPI_COND_TYPE          void*

#   if defined(CMPI_PLATFORM_WIN32_IX86_MSVC)
#      define CMPI_THREAD_CDECL    __stdcall
#      define CMPI_THREAD_KEY_TYPE unsigned long int

#   ifndef HAVE_STRUCT_TIMESPEC
#       define HAVE_STRUCT_TIMESPEC
	    struct timespec
	    {
			long tv_sec;
			long tv_nsec;
	    };
#   endif /* HAVE_STRUCT_TIMESPEC */

#   elif defined(CMPI_PLATFORM_ZOS_ZSERIES_IBM)
#      ifndef __cplusplus
#      include <pthread.h>
#         define CMPI_THREAD_CDECL
#      else
#         define CMPI_THREAD_CDECL    __cdecl
#      endif
#      define CMPI_THREAD_KEY_TYPE  pthread_key_t
#   else
#      define CMPI_THREAD_CDECL
#      define CMPI_THREAD_KEY_TYPE unsigned long int
#   endif

/* Define CMPI_EXPORT */
#   if defined(CMPI_PLATFORM_WIN32_IX86_MSVC)
#      define CMPI_EXPORT __declspec(dllexport)
#   elif defined(CMPI_PLATFORM_LINUX_GENERIC_GNU) && (__GNUC__ >= 4)
#      define CMPI_EXPORT __attribute__((visibility("default")))
#   else
#      define CMPI_EXPORT /* empty */
#   endif

/* Define CMPI_EXTERN_C */
#   ifdef __cplusplus
#      define CMPI_EXTERN_C extern "C" CMPI_EXPORT
#   else
#      define CMPI_EXTERN_C CMPI_EXPORT
#   endif

#endif
