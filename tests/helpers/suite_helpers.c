#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <check.h>
#include "../../src/helpers.c"

/** Test Fixtures */
#include "../fixtures.c"

/** Test Cases */
#include "check_for_highest_revision.c"
#include "check_for_no_current.c"
#include "create_document_dir.c"
#include "create_file.c"
#include "create_new_file.c"
#include "get_revision_from_filename.c"
#include "make_parents.c"


/** Test Suite **/
Suite * helpers_suite(void)
{
    Suite *test_suite;
    test_suite = suite_create("Helpers");
    suite_add_tcase(test_suite, get_revision_test_case());
    suite_add_tcase(test_suite, create_document_dir_test_case());
    suite_add_tcase(test_suite, check_for_no_current_test_case());
    suite_add_tcase(test_suite, make_parents_test_case());
    suite_add_tcase(test_suite, check_for_highest_revision_test_case());
    suite_add_tcase(test_suite, create_file_test_case());
    suite_add_tcase(test_suite, create_new_file_test_case());
    return test_suite;
}
