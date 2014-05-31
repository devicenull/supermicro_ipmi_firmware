#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(sblim-sfcc, 2.1.0, sblim-devel@lists.sourceforge.net)
AC_CONFIG_SRCDIR([cimc/cimcclient.c])
AC_CONFIG_HEADER([config.h])

AM_INIT_AUTOMAKE

AC_ARG_ENABLE(http-chunking,
	[AC_HELP_STRING([--disable-http-chunking],
		[disable http chunking support. May be necessary for older CURL versions.])],
	[enable_http_chunking=$enableval],
	[enable_http_chunking="yes"]
	)
		
AC_ARG_ENABLE(large_volume_support,
	[AC_HELP_STRING([--enable-large_volume_support],
		[large_volume_support uses http chunk for enumclasses,enumclassnames,enuminstances,enuminstancenames.])],
	[large_volume_support=$enableval],
	[large_volume_support="no"]
	)	
	
# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_LIBTOOL

# Checks for libraries.

# 
if test "$large_volume_support" == "yes" ; then
   echo "enabled to compile for large volume support"
   CPPFLAGS="-DLARGE_VOL_SUPPORT $CPPFLAGS"
   LIBS="$LIBS -lrt"
fi

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([malloc.h stddef.h stdlib.h string.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_STRUCT_TM

# Checks for libraries
AC_CHECK_LIB(curl,curl_easy_init,[LIBCURL=-lcurl],[AC_MSG_ERROR([Could not find required libcurl])])
AC_CHECK_LIB(pthread,main)

# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_MKTIME
AC_FUNC_REALLOC
AC_FUNC_STRFTIME
AC_CHECK_FUNCS([gettimeofday localtime_r memset strcasecmp strdup strncasecmp strstr tzset])

# MELUNIT
AM_CONDITIONAL(HAVE_MELUNIT_CXX,[test 0 = 1 ])

if test $enable_http_chunking != yes 
then
   AC_DEFINE(HAVE_HTTP_CHUNKING,1,[Indication support enabled.])
fi

AC_CONFIG_FILES([Makefile sfcc.spec TEST/Makefile ])
AC_OUTPUT
