#define _GNU_SOURCE
#include <stdio.h>


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/* src/helpers.c */
/* create_document_dir */

START_TEST(test_get_create_document_dir)
{
  char *test_dir;
  int dirfd = AT_FDCWD;
  asprintf(&test_dir,
           "%s/test_create_document_dir",
           fixture_directory_path);
  int creation_error = create_document_dir(dirfd, test_dir);
  ck_assert_int_eq(creation_error, 0);

  test_check_directory_existence("test_create_document_dir");
  test_delete_directory("test_create_document_dir");
}
END_TEST



TCase *create_document_dir_test_case(void)
{
  TCase *test_case;
  test_case = tcase_create("CreateDocumentDir");
  tcase_add_checked_fixture(test_case,
                            create_fixture_directory,
                            delete_fixture_directory);
  tcase_add_test(test_case, test_get_create_document_dir);
  return test_case;
}
