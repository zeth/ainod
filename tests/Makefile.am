# Makefile.am

AM_LDFLAGS=-lsystemd -lrt -luuid -ljson-c
AM_CFLAGS=-pthread -I/usr/include/json-c

lib_LIBRARIES= libainod.a
libainod_a_SOURCES= ../src/handleerror.h ../src/handleerror.c		\
../src/daemon/worker.h ../src/daemon/worker.c ../src/daemon/asocket.h	\
../src/daemon/asocket.c ../src/daemon/configparser.h			\
../src/daemon/configparser.c ../src/daemon/mutex.h			\
../src/daemon/mutex.c ../src/jsonschema.h ../src/jsonrpc.h		\
../src/jsonrpc.c ../src/jsonhelpers.h ../src/jsonhelpers.c		\
../src/filter.h ../src/filter.c ../src/helpers.h ../src/helpers.c	\
../src/methods.h ../src/methods.c ../src/daemon/parent.h		\
../src/daemon/parent.c

# Tell automake to build the testrunner on "make check"
check_PROGRAMS= testrunner
# Tell automake to run the testrunner on "make check"
TESTS= $(check_PROGRAMS)

# List all your test source code here
testrunner_SOURCES= test_runner.c
# Compile only test code with -g and the NovaProva flags
testrunner_CFLAGS= -g $(CHECK_CFLAGS)
# Link testrunner with the Code Under Test and the NovaProva library
testrunner_LDADD= libainod.a $(CHECK_LIBS)
