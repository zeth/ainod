#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([ainod], [0.1], [theology@gmail.com])
: ${CFLAGS=-Werror -O3}
AC_CONFIG_SRCDIR([hfunctions.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_CC_C99
AC_PROG_INSTALL

AM_INIT_AUTOMAKE([-Wall -Werror])
# Checks for libraries.
AC_CHECK_LIB([systemd], [sd_listen_fds])

# Checks for header files.
AC_CHECK_HEADERS([limits.h stdlib.h string.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset socket strchr strcspn strdup strerror strndup strspn])

# Check for dependencies
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
