# Makefile.am

AM_LDFLAGS=-lsystemd -lrt -luuid -ljson-c
AM_CFLAGS=-pthread -I/usr/include/json-c

lib_LIBRARIES= libainod.a
libainod_a_SOURCES= ../src/hfunctions.h ../src/hfunctions.c		\
../src/constants.h ../src/handleerror.h ../src/handleerror.c		\
../src/worker.h ../src/worker.c ../src/asocket.h ../src/asocket.c	\
../src/configparser.h ../src/configparser.c ../src/mutex.h		\
../src/mutex.c ../src/jsonschema.h ../src/jsonrpc.h ../src/jsonrpc.c	\
../src/jsonhelpers.h ../src/jsonhelpers.c ../src/filter.h		\
../src/filter.c ../src/methods.h ../src/methods.c

# Tell automake to build the testrunner on "make check"
check_PROGRAMS= testrunner
# Tell automake to run the testrunner on "make check"
TESTS= $(check_PROGRAMS)

# List all your test source code here
testrunner_SOURCES= mytest.c
# Compile only test code with -g and the NovaProva flags
testrunner_CFLAGS= -g $(NOVAPROVA_CFLAGS)
# Link testrunner with the Code Under Test and the NovaProva library
testrunner_LDADD= libainod.a $(NOVAPROVA_LIBS)
