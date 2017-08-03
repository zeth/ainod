#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <check.h>
#include "../../src/filter.c"

/** Test Cases */
#include "get_id.c"
#include "get_path_from_filter.c"


/** Test Suite **/
Suite * filter_suite(void)
{
    Suite *test_suite;
    test_suite = suite_create("Filter");
    suite_add_tcase(test_suite, make_get_id_test_case());
    suite_add_tcase(test_suite, make_get_path_from_filter_test_case());
    return test_suite;
}
