#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <check.h>
#include "../src/helpers.c"

/** Test Cases */
#include "helpers/get_revision_from_filename.c"


/** Test Suite **/
Suite * helpers_suite(void)
{
    Suite *s;
    TCase *get_revision;

    s = suite_create("Helpers");
    get_revision = get_revision_test_case();
    suite_add_tcase(s, get_revision);
    return s;
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
