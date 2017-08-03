#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <check.h>
#include "../../src/daemon/configparser.c"

/** Test Cases */
#include "get_key_value.c"
#include "string_is_in_array.c"

/** Test Suite **/
Suite * configparser_suite(void)
{
    Suite *test_suite;
    test_suite = suite_create("ConfigParser");
    suite_add_tcase(test_suite, make_get_key_value_test_case());
    suite_add_tcase(test_suite, make_string_is_in_array_case());
    return test_suite;
}
