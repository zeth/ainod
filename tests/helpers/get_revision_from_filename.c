#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <check.h>

/* src/helpers.c */
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
