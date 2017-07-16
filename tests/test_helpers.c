#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <check.h>
#include "../src/helpers.c"

/** Test Fixtures */
#include "fixtures.c"

/** Test Cases */
#include "helpers/check_for_highest_revision.c"
#include "helpers/check_for_no_current.c"
#include "helpers/create_document_dir.c"
#include "helpers/create_file.c"
#include "helpers/create_new_file.c"
#include "helpers/get_revision_from_filename.c"
#include "helpers/make_parents.c"


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
    return test_suite;
}

/** Test Runner **/
int main(void)
{
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = helpers_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
