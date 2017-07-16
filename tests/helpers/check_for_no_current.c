#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/* src/helpers.c */
/* check_for_no_current */

/** With nothing in the way, the test should pass */
START_TEST(test_check_for_no_current)
{
  int dirfd = AT_FDCWD;
  int result = check_for_no_current(dirfd, fixture_directory_path);
  ck_assert_int_eq(result, 0);
}
END_TEST

/** With an existing symlink, the test should fail. */
START_TEST(test_check_for_existing_symlink)
{

  int dirfd = AT_FDCWD;

  /** Create a numbered file */
  char *numbered_file;
  asprintf(&numbered_file, "%s/1.json", fixture_directory_path);
  int fd = open(numbered_file, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  ck_assert_int_ne(fd, -1); 
  struct stat s;
  int existence_err = stat(numbered_file, &s);
  ck_assert_int_eq(existence_err, 0);

  /** Create a current.json symlink */
  char *current_file;
  asprintf(&current_file, "%s/current.json", fixture_directory_path);
  int symlink_error = symlink(numbered_file, current_file);
  ck_assert_int_eq(symlink_error, 0);
  struct stat sym_s;
  int symlink_existence_err = stat(current_file, &sym_s);
  ck_assert_int_eq(symlink_existence_err, 0);

  /** Key part: check that the function returns -1 */
  int result = check_for_no_current(dirfd, fixture_directory_path);
  ck_assert_int_eq(result, -1);

  /** Tidy up */
  int unlink_sym_err = unlink(current_file);
  int unlink_err = unlink(numbered_file);
  ck_assert_int_eq(unlink_err, 0);
  ck_assert_int_eq(unlink_sym_err, 0);
  free(current_file);
  free(numbered_file);
}
END_TEST

/** With a random blank file, the test should fail */
START_TEST(test_check_for_existing_current_blank_file)
{
  /** Create a fake current.json file */
  int dirfd = AT_FDCWD;
  char *current_file;
  asprintf(&current_file, "%s/current.json", fixture_directory_path);
  int fd = open(current_file, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  ck_assert_int_ne(fd, -1);
  struct stat s;
  int existence_err = stat(current_file, &s);
  ck_assert_int_eq(existence_err, 0);

  /** Key part: check that the function returns -1 */
  int result = check_for_no_current(dirfd, fixture_directory_path);
  ck_assert_int_eq(result, -1);

  /** Tidy up */
  int unlink_err = unlink(current_file);
  ck_assert_int_eq(unlink_err, 0);
  free(current_file);

}
END_TEST

TCase *check_for_no_current_test_case(void)
{
  TCase *test_case;
  test_case = tcase_create("CheckNoCurrent");
  tcase_add_checked_fixture(test_case,
                            create_fixture_directory,
                            delete_fixture_directory);
  tcase_add_test(test_case, test_check_for_no_current);
  tcase_add_test(test_case, test_check_for_existing_current_blank_file);
  tcase_add_test(test_case, test_check_for_existing_symlink);
  return test_case;
}
