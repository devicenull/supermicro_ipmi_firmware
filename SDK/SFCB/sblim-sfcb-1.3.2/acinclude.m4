dnl
dnl Helper functions
dnl
AC_DEFUN([_CHECK_SFCC],
    [
    AC_MSG_CHECKING($1)
    #AC_CHECK_HEADERS([cmci.h],[have_SFCC=yes],[have_SFCC=no])
    AC_LINK_IFELSE(
    [
		#include <cmci.h>
		int main (void)
		{
		    CMPIStatus status;
		    CMSetStatus(&status, CMPI_RC_OK);
		    return 0;
		}    
    ],
    [
    	have_SFCC=yes
    ],
    [
    	have_SFCC=no
    ])


])

AC_DEFUN([CHECK_SFCC],
    [
    AC_MSG_CHECKING(for SFCC headers)
        SFCC_CPP_FLAGS="$CPPFLAGS"
    dnl Check just with the standard include paths
    _CHECK_SFCC(standard)
    if test "$have_SFCC" == "yes"; then
        dnl The standard include paths worked.
        AC_MSG_RESULT(yes)
    else
      _DIRS_="/usr/local/include \
      		/usr/local/include/CimClientLib \
            /usr/include/CimClientLib"               
      for _DIR_ in $_DIRS_
      do
         _cppflags=$CPPFLAGS
         _include_SFCC="$_DIR_"
         CPPFLAGS="$CPPFLAGS -I$_include_SFCC "
         _CHECK_SFCC($_DIR_)
         if test "$have_SFCC" == "yes"; then
            dnl Found it
            AC_MSG_RESULT(yes)
            dnl Save the new -I parameter
            SFCC_CPP_FLAGS="$CPPFLAGS"
            break
         fi
             CPPFLAGS=$_cppflags
      done
    fi
    CPPFLAGS="$SFCC_CPP_FLAGS"
    if test "$have_SFCC" == "no"; then
        AC_MSG_ERROR(no. Sorry cannot find SFCC headers files.)
    fi
    ]
)
