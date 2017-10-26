#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <check.h>
#include <json-c/json.h>

/** Code under test */
#include "../../src/methods.c"

/** Test Cases */
#include "get_simple.c"
#include "create.c"

/** Test Suite **/
Suite * methods_suite(void)
{
    Suite *test_suite;
    test_suite = suite_create("Methods");
    suite_add_tcase(test_suite, make_get_simple_test_case());
    suite_add_tcase(test_suite, make_create_test_case());
    return test_suite;
}
