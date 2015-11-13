bin_PROGRAMS=ainod
ainod_SOURCES=handleerror.h handleerror.c daemon/worker.h	\
daemon/worker.c daemon/asocket.h daemon/asocket.c		\
daemon/configparser.h daemon/configparser.c daemon/mutex.h	\
daemon/mutex.c jsonschema.h jsonrpc.h jsonrpc.c jsonhelpers.h	\
jsonhelpers.c filter.h filter.c helpers.h helpers.c methods.h	\
methods.c daemon/parent.h daemon/parent.c ainod.c
AM_LDFLAGS=-lrt -luuid -ljson-c
AM_CFLAGS=-pthread -I/usr/include/json-c
