#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <check.h>
#include "../src/helpers.c"

/* get_revision_from_filename */

START_TEST(test_get_revision_from_filename)
{
  char *filename;
  asprintf(&filename, "10.json");
  int current_revision = get_revision_from_filename(filename);
  ck_assert_int_eq(current_revision, 10);
  free(filename);
  
}
END_TEST

START_TEST(test_get_revision_from_filename_no_number)
{
  char *filename;
  asprintf(&filename, "Hello.json");
  int current_revision = get_revision_from_filename(filename);
  ck_assert_int_eq(current_revision, -1);
  free(filename);
  
}
END_TEST

START_TEST(test_get_revision_from_filename_junk)
{
  char *filename;
  asprintf(&filename, "1_extrajunk.json");
  int current_revision = get_revision_from_filename(filename);
  ck_assert_int_eq(current_revision, -1);
  free(filename);
  
}
END_TEST

START_TEST(test_get_revision_from_filename_blank)
{
  char *filename;
  asprintf(&filename, "");
  int current_revision = get_revision_from_filename(filename);
  ck_assert_int_eq(current_revision, -1);
  free(filename);
  
}
END_TEST

START_TEST(test_get_revision_from_filename_half_blank)
{
  char *filename;
  asprintf(&filename, ".json");
  int current_revision = get_revision_from_filename(filename);
  ck_assert_int_eq(current_revision, -1);
  free(filename);
  
}
END_TEST

START_TEST(test_get_revision_from_filename_no_extension)
{
  char *filename;
  asprintf(&filename, "1json");
  int current_revision = get_revision_from_filename(filename);
  ck_assert_int_eq(current_revision, -1);
  free(filename);
  
}
END_TEST


TCase *get_revision_test_case(void)
{
  TCase *get_revision;
  get_revision = tcase_create("GetRevision");
  tcase_add_test(get_revision, test_get_revision_from_filename);
  tcase_add_test(get_revision, test_get_revision_from_filename_no_number);
  tcase_add_test(get_revision, test_get_revision_from_filename_junk);
  tcase_add_test(get_revision, test_get_revision_from_filename_blank);
  tcase_add_test(get_revision, test_get_revision_from_filename_half_blank);
  tcase_add_test(get_revision, test_get_revision_from_filename_no_extension);
  return get_revision;
}


Suite * helpers_suite(void)
{
    Suite *s;
    TCase *get_revision;

    s = suite_create("Helpers");
    get_revision = get_revision_test_case();
    suite_add_tcase(s, get_revision);
    return s;
}

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
