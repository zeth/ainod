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
