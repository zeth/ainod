#define _GNU_SOURCE
#include <stdio.h>

/** check_for_highest_revision */

START_TEST(test_check_for_highest_revision_blank)
{
  int dirfd = AT_FDCWD;
  int revision = check_for_highest_revision(dirfd, fixture_directory_path);
  ck_assert_int_eq(revision, 0);
}
END_TEST


START_TEST(test_check_for_highest_revision_second)
{
  int dirfd = AT_FDCWD;
  test_create_numbered_file(0);
  test_create_numbered_file(1);
  test_create_numbered_file(2);
  int revision = check_for_highest_revision(dirfd, fixture_directory_path);
  ck_assert_int_eq(revision, 2);
  delete_numbered_file(0);
  delete_numbered_file(1);
  delete_numbered_file(2);
}
END_TEST


TCase *check_for_highest_revision_test_case(void)
{
  TCase *test_case;
  test_case = tcase_create("HighestRevision");
  tcase_add_checked_fixture(test_case,
                            create_fixture_directory,
                            delete_fixture_directory);
  tcase_add_test(test_case, test_check_for_highest_revision_blank);
  tcase_add_test(test_case, test_check_for_highest_revision_second);
  return test_case;
}
