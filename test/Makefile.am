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
../src/filter.h ../src/filter.c ../src/methods.h ../src/methods.c	\
../src/daemon/parent.h ../src/daemon/parent.c

# Tell automake to build the testrunner on "make check"
check_PROGRAMS= testrunner
# Tell automake to run the testrunner on "make check"
TESTS= $(check_PROGRAMS)

# List all your test source code here
testrunner_SOURCES= test_filter_get_id.c				\
test_jsonrpc_create_response.c test_jsonrpc_create_response_error.c	\
test_jsonrpc_init_error_object.c					\
test_configparser_string_is_in_array.c					\
test_configparser_get_key_value.c test_methods_get_simple.c		\
test_filter_get_path_from_filter.c					\
test_filter_get_path_from_filter_pathformat_one.c			\
test_filter_get_path_from_filter_pathformat_two.c			\
test_handlerror_handle_error.c
# Compile only test code with -g and the NovaProva flags
testrunner_CFLAGS= -g $(NOVAPROVA_CFLAGS)
# Link testrunner with the Code Under Test and the NovaProva library
testrunner_LDADD= libainod.a $(NOVAPROVA_LIBS)
