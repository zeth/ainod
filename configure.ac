#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([ainod], [0.1], [theology@gmail.com])
: ${CFLAGS=-Werror -O0 -g}
AC_CONFIG_SRCDIR([src/handleerror.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_CC_C99
AC_PROG_INSTALL
AC_PROG_RANLIB

AM_INIT_AUTOMAKE([subdir-objects serial-tests -Wall -Werror -Wno-portability])
# Checks for libraries.
AC_CHECK_LIB(pthread, pthread_attr_init)
AC_CHECK_LIB([systemd], [sd_listen_fds])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h limits.h stdlib.h string.h sys/socket.h unistd.h netdb.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_CHECK_FUNCS([ftruncate memset socket strchr strcspn strdup strerror strndup strspn])

# Check for dependencies
AC_CONFIG_FILES([
 Makefile
 src/Makefile
 test/Makefile
])

# Check for optional dependencies
AC_ARG_ENABLE([tests],
        AS_HELP_STRING([--enable-tests], [Enable unit tests]))

AS_IF([test "x$enable_tests" = "xyes"], [
  # Do the stuff needed for enabling the feature
  AC_MSG_NOTICE([Tests have been enabled!!!!!!!!!!])
  PKG_CHECK_EXISTS([novaprova],
          [
        AC_MSG_NOTICE([Found novaprova])
        PKG_CHECK_MODULES(NOVAPROVA, novaprova)
        ],
        [AC_MSG_NOTICE([Did not find novaprova])])
])
AM_CONDITIONAL([TESTS_ENABLED], [test "x$enable_tests" = "xyes"])

AC_OUTPUT
