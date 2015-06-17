bin_PROGRAMS=ainod
ainod_SOURCES=hfunctions.h hfunctions.c handleerror.h handleerror.c	\
worker.h worker.c asocket.h asocket.c configparser.h configparser.c	\
mutex.h mutex.c jsonschema.h jsonrpc.h jsonrpc.c ainod.c
AM_LDFLAGS=-lsystemd -lrt -luuid -ljson-c
AM_CFLAGS=-pthread -I/usr/include/json-c
