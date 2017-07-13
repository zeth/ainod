#define _GNU_SOURCE
#include <stdio.h>

#include <uuid/uuid.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/* src/helpers.c */
/* create_document_dir */

char *get_id(void) {
  char *id;
  uuid_t uuid;
  id = malloc(128);
  uuid_generate(uuid);
  uuid_unparse(uuid, id);
  return id;
}


char *pathname;
char *test_dir;

void create_fixture_directory(void)
{
  const char *id;
  id = get_id();
  asprintf(&pathname, "/dev/shm/ainod_test_fixture_%s", id);
  int creation_error = mkdir(pathname, S_IRWXU|S_IRWXG|S_IROTH);
  ck_assert_int_eq(creation_error, 0);
  free((char*) id);
}

void delete_fixture_directory(void)
{
  int removal_error = rmdir(pathname);
  ck_assert_int_eq(removal_error, 0);
  free(pathname);
}


START_TEST(test_get_create_document_dir)
{
  int dirfd = AT_FDCWD;
  asprintf(&test_dir, "%s/test_create_document_dir", pathname);
  int creation_error = create_document_dir(dirfd, test_dir);
  ck_assert_int_eq(creation_error, 0);
  struct stat s;
  int existence_err = stat(test_dir, &s);
  ck_assert_int_eq(existence_err, 0);
  int removal_error = rmdir(test_dir);
  ck_assert_int_eq(removal_error, 0);
  free(test_dir);
}
END_TEST



TCase *create_document_dir_test_case(void)
{
  TCase *test_case;
  test_case = tcase_create("CreateDocumentDir");
  tcase_add_checked_fixture(test_case, create_fixture_directory, delete_fixture_directory);
  tcase_add_test(test_case, test_get_create_document_dir);
  return test_case;
}
