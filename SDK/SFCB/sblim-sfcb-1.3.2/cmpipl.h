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

#ifndef _CMPIPL_H_
#   define _CMPIPL_H_

#ifndef CMPI_PLATFORM_LINUX_GENERIC_GNU
#define CMPI_PLATFORM_LINUX_GENERIC_GNU
#endif


// There are the following list of platforms

#   if !defined(CMPI_PLATFORM_LINUX_GENERIC_GNU) && !defined(CMPI_PLATFORM_HPUX_ACC) && \
    !defined(CMPI_PLATFORM_WIN32_IX86_MSVC) && !defined(CMPI_PLATFORM_SOLARIS_SPARC_GNU) && \
    !defined(CMPI_PLATFORM_SOLARIS_SPARC_CC) && !defined(CMPI_PLATFORM_AIX_RS_IBMCXX) && \
    !defined(CMPI_PLATFORM_ZOS_ZSERIES_IBM) && !defined(CMPI_PLATFORM_TRU64_ALPHA_DECCXX) && \
    !defined(CMPI_PLATFORM_OS400_ISERIES_IBM) && !defined(CMPI_PLATFORM_DARWIN_PPC_GNU) && \
	!defined(CMPI_PLATFORM_VMS_ALPHA_DECCXX) && !defined(CMPI_PLATFORM_VMS_IA64_DECCXX)
#      error "You have not defined the right platform. The choices are:"
#      error "CMPI_PLATFORM_LINUX_GENERIC_GNU, CMPI_PLATFORM_HPUX_ACC,"
#      error "CMPI_PLATFORM_WIN32_IX86_MSVC, CMPI_PLATFORM_SOLARIS_SPARC_GNU,"
#      error "CMPI_PLATFORM_SOLARIS_SPARC_CC, CMPI_PLATFORM_AIX_RS_IBMCXX,"
#      error "CMPI_PLATFORM_ZOS_ZSERIES_IBM, CMPI_PLATFORM_TRU64_ALPHA_DECCXX,"
#      error "CMPI_PLATFORM_OS400_ISERIES_IBM, CMPI_PLATFORM_DARWIN_PPC_GNU,"
#	   error "CMPI_PLATFORM_VMS_ALPHA_DECCXX, CMPI_PLATFORM_VMS_IA64_DECCXX"
#   endif

#endif
