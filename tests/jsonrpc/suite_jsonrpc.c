#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <check.h>
#include <json-c/json.h>

/** Code under test */
#include "../../src/jsonrpc.c"

/** Test Cases */
#include "init_error_object.c"
#include "create_response.c"
#include "create_response_error.c"
#include "get_method_name.c"

/** Test Suite **/
Suite * jsonrpc_suite(void)
{
    Suite *test_suite;
    test_suite = suite_create("JSONRPC");
    suite_add_tcase(test_suite, make_init_error_object_test_case());
    suite_add_tcase(test_suite, make_create_resource_test_case());
    suite_add_tcase(test_suite, make_create_resource_error_test_case());
    suite_add_tcase(test_suite, make_get_method_name_test_case());
    return test_suite;
}
