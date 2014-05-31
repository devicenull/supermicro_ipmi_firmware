# $id: configure.ac,v 1.10 2006/01/23 15:35:09 a3schuur Exp $

AC_INIT(MOF Compiler, 0.8.2, sblim-devel@lists.sourceforge.net,mofc)
AC_CONFIG_SRCDIR([hash.c])
AC_CONFIG_HEADER([config.h])

AM_INIT_AUTOMAKE

AC_CANONICAL_HOST
case $host_os in
     openedition*) MOFC_CPPFLAGS=-W"c,SSCOMM" 
                MOFC_CMPI_PLATFORM="-D CMPI_PLATFORM_LINUX_GENERIC_GNU"
		MOFC_CPPFLAGS="-D_OPEN_SOURCE=3 -D_ENHANCED_ASCII_EXT=0xFFFFFFFF $MOFC_CMPI_PLATFORM $MOFC_CPPFLAGS"
		MOFC_CFLAGS=-W"c,ASCII,dll,expo,langlvl(extended),float $MOFC_CMPI_PLATFORM"
		MOFC_LDFLAGS=-W"l,XPLINK,dll,EDIT=NO"
		;;
     **)	MOFC_CFLAGS="-Wall -fsigned-char"
                MOFC_CMPI_PLATFORM="-D CMPI_PLATFORM_LINUX_GENERIC_GNU"
                MOFC_CFLAGS="$MOFC_CFLAGS $MOFC_CMPI_PLATFORM"
		;;
esac
AC_SUBST(MOFC_CPPFLAGS)
AC_SUBST(MOFC_CFLAGS)
AC_SUBST(MOFC_LDFLAGS)

AC_ARG_ENABLE(relax-mofsyntax,
	      [AC_HELP_STRING([--enable-relax-mofsyntax],
		              [be less strict when parsing MOFs.])])
AC_ARG_ENABLE(onepass,
	      [AC_HELP_STRING([--enable-onepass],
		              [create one-pass parser, disallows forward declarations.])])
AC_ARG_ENABLE(namespace-kludge,
	      [AC_HELP_STRING([--enable-namespace-kludge],
		              [allow to create __namespace class definition.])])
AC_ARG_ENABLE(testscanner,
	      [AC_HELP_STRING([--enable-testscanner],
		              [produce a standalone scanner for testing.])])
AC_ARG_VAR(SFCB_DIR,
      [set SFCB_DIR to the directory containing the SFCB sources, 
       if configure fails to find it.])

AC_ARG_WITH(sfcb,
	    [AC_HELP_STRING([--with-sfcb],
		              [use sfcb backend. In that case this package must
			       be in a sub-directory of the sfcb package.])])


if test x"$SFCB_DIR" != x && test "$with_sfcb" != yes ; then
   AC_WARN([SFCB_DIR has been specified ... assuming --with_sfcb])
   with_sfcb=yes
fi

unset MOFCNAME

if test "$with_sfcb" == yes; then
   if test x"$SFCB_DIR" = x; then
      SFCB_DIR=$srcdir/..
   fi
   MOFCNAME=sfcbmof
   BACKEND_STRING=sfcb
   BACKEND_OBJ='backend_sfcb.$(OBJEXT)'

   BACKEND_LIBS="-L ../.libs -lsfcBrokerCore -lsfcFileRepository -lsfcUtil"
   BACKEND_INCLUDES="-I $SFCB_DIR -I $SFCB_DIR/sfcUtil"
   AC_DEFINE(BACKEND_INFO,"SFCB Backend",[Backend Identification String])
else
   BACKEND_STRING=default
   BACKEND_OBJ='backend_print.$(OBJEXT)'
   BACKEND_LIBS=
   BACKEND_INCLUDES=
   AC_DEFINE(BACKEND_INFO,"Default Backend",[Backend Identification String])
fi

# Checks for programs.
AC_PROG_CC
AC_PROG_YACC
AC_PROG_LEX

if test -z "${YACC}" || test -z "${LEX}"
then
	AC_MSG_WARN([Lex and Yacc are required to build mofc from scratch or after grammar changes.])
fi

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stddef.h stdlib.h string.h unistd.h])
if test "$with_sfcb" = yes; then
   save_CPPFLAGS=$CPPFLAGS
   CPPFLAGS="-I $SFCB_DIR $MOFC_CMPI_PLATFORM $CPPFLAGS"
   AC_CHECK_HEADER(objectImpl.h,,
	[AC_MSG_ERROR([sorry, could not find SFCB include files. Hint: set SFCB_DIR environment variable.])])
   CPPFLAGS=$save_CPPFLAGS
fi;

# Checks for libraries
if test "$with_sfcb" = yes && test x"$sfcb_recursed" = x; then
   save_LDFLAGS=$LDFLAGS
   LDFLAGS="-L $../.libs $LDFLAGS"
   AC_CHECK_LIB(sfcBrokerCore,main,,
       [AC_MSG_ERROR([sorry, could not find SFCB libraries.])])
   LDFLAGS=$save_LDFLAGS
fi;

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([strcasecmp strchr strdup strrchr strstr])

# Defines
if test "$enable_relax_mofsyntax" = yes; then
  AC_DEFINE(RELAXED_MOF,,[Relaxed MOF Parsing])
fi

if test "$enable_namespace_kludge" = yes; then
  AC_DEFINE(NAMESPACE_KLUDGE,,[Allow __Namespace class name.])
fi

if test "$enable_onepass" = yes; then
  AC_DEFINE(ONEPASS,,[One-Pass Parsing])
fi

AM_CONDITIONAL(TESTSCANNER,[test "$enable_testscanner" == yes]) 
AM_CONDITIONAL(ALTERNATE_MOFCNAME,[test x"$MOFCNAME" != x]) 

AC_SUBST(MOFCNAME)
AC_SUBST(BACKEND_OBJ)
AC_SUBST(BACKEND_LIBS)
AC_SUBST(BACKEND_INCLUDES)

# Output control
echo =================================================================
echo mofc configuration complete
echo configured for $BACKEND_STRING backend
if test "$with_sfcb" == yes
then
echo SFCB_DIR=$SFCB_DIR
fi
echo =================================================================


AC_CONFIG_FILES([Makefile])
AC_OUTPUT
