#define _GNU_SOURCE
#include <stdio.h>

/* src/helpers.c */
/* make_parents */

START_TEST(test_make_parents)
{
  int dirfd = AT_FDCWD;

  // Add the directories
  char *creation_directory;
  asprintf(&creation_directory,
           "%s/test_make_parents_top_dir/middle_dir/bottom_dir/",
           fixture_directory_path);
  int creation_result = make_parents(dirfd, creation_directory);
  ck_assert_int_eq(creation_result, 0);
  free(creation_directory);

  // Check the bottom dir exists
  test_check_directory_existence("test_make_parents_top_dir/middle_dir/bottom_dir");

  // Delete the directories
  test_delete_directory("test_make_parents_top_dir/middle_dir/bottom_dir");
  test_delete_directory("test_make_parents_top_dir/middle_dir");
  test_delete_directory("test_make_parents_top_dir");
}
END_TEST



TCase *make_parents_test_case(void)
{
  TCase *test_case;
  test_case = tcase_create("MakeParents");
  tcase_add_checked_fixture(test_case,
                            create_fixture_directory,
                            delete_fixture_directory);
  tcase_add_test(test_case, test_make_parents);
  return test_case;
}
