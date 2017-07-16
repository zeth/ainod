#define _GNU_SOURCE
#include <stdio.h>
#include <uuid/uuid.h>

char *fixture_directory_path;


char *get_unique_path_id(void) {
  char *id;
  uuid_t uuid;
  id = malloc(128);
  uuid_generate(uuid);
  uuid_unparse(uuid, id);
  return id;
}


void create_fixture_directory(void)
{
  const char *id;
  id = get_unique_path_id();
  asprintf(&fixture_directory_path, "/dev/shm/ainod_test_fixture_%s", id);
  int creation_error = mkdir(fixture_directory_path, S_IRWXU|S_IRWXG|S_IROTH);
  ck_assert_int_eq(creation_error, 0);
  free((char*) id);
}


void delete_fixture_directory(void)
{
  int removal_error = rmdir(fixture_directory_path);
  ck_assert_int_eq(removal_error, 0);
  free(fixture_directory_path);
}

void test_delete_directory(const char *directory)
{
  char *delete_directory;
  asprintf(&delete_directory, "%s/%s/", fixture_directory_path, directory);
  int removal_error = rmdir(delete_directory);
  ck_assert_int_eq(removal_error, 0);
  free(delete_directory);
}

void test_check_directory_existence(const char *directory)
{
  char *check_directory;
  asprintf(&check_directory, "%s/%s/", fixture_directory_path, directory);
  struct stat file_status;
  int existence_err = stat(check_directory, &file_status);
  ck_assert_int_eq(existence_err, 0);
  free(check_directory);
}
