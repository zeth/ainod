#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <check.h>

/** Code under test */
#include "../../src/jsonrpc.c"

/** Test Cases */
#include "init_error_object.c"

/** Test Suite **/
Suite * jsonrpc_suite(void)
{
    Suite *test_suite;
    test_suite = suite_create("JSONRPC");
    suite_add_tcase(test_suite, make_init_error_object_test_case());
    return test_suite;
}
