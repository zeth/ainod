#define _GNU_SOURCE
#include <stdio.h>
#include <uuid/uuid.h>

#include "../config.h"

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
  asprintf(&fixture_directory_path, "%s/ainod_test_fixture_%s", TEST_FIXTURE_STORAGE, id);
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

void test_create_numbered_file(int number)
{
  char *numbered_filename;
  asprintf(&numbered_filename, "%s/%d.json", fixture_directory_path, number);
  int fd = open(numbered_filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  ck_assert_int_ne(fd, -1);
  struct stat file_status;
  int existence_err = stat(numbered_filename, &file_status);
  ck_assert_int_eq(existence_err, 0);
}

void delete_numbered_file(int number)
{
  char *numbered_filename;
  asprintf(&numbered_filename, "%s/%d.json", fixture_directory_path, number);
  int unlink_err = unlink(numbered_filename);
  ck_assert_int_eq(unlink_err, 0);
  free(numbered_filename);
}

void delete_current_symlink(void)
{
  char *current_filename;
  asprintf(&current_filename, "%s/current.json", fixture_directory_path);
  int unlink_err = unlink(current_filename);
  ck_assert_int_eq(unlink_err, 0);
  free(current_filename);
}

int read_numbered_file(int number, char* buffer)
{
  char *fullpath;
  asprintf(&fullpath, "%s/%d.json", fixture_directory_path, number);
  int fd = open(fullpath, O_RDONLY);
  int num_read = read(fd, buffer, JSON_FILE_BUF_SIZE);
  buffer[num_read] = '\0';
  free(fullpath);
  return 0;
}

void check_contents_numbered_file(int number, char *contents)
// Check the file has the correct contents
// For better isolation, we will just read the file
// instead of using json-c api
{
  char *buffer = (char *) malloc(JSON_FILE_BUF_SIZE);
  int read_result = read_numbered_file(1, buffer);
  ck_assert_int_eq(read_result, 0);
  ck_assert_str_eq(buffer, contents);
  free(buffer);
}

void test_create_test_file(void)
{
  create_fixture_directory();
  // Make a store directory
  char *store_directory;
  asprintf(&store_directory, "%s/%s", fixture_directory_path, "timemachine");
  int creation_error = mkdir(store_directory, S_IRWXU|S_IRWXG|S_IROTH);
  ck_assert_int_eq(creation_error, 0);
  // Make schema directory
  char *schema_directory;
  asprintf(&schema_directory, "%s/%s", store_directory, "eloi");
  creation_error = mkdir(schema_directory, S_IRWXU|S_IRWXG|S_IROTH);
  ck_assert_int_eq(creation_error, 0);
  // Make object directory
  char *object_directory;
  asprintf(&object_directory, "%s/%s", schema_directory, "weena");
  creation_error = mkdir(object_directory, S_IRWXU|S_IRWXG|S_IROTH);
  ck_assert_int_eq(creation_error, 0);
  // Add a file
  char *numbered_filename;
  asprintf(&numbered_filename, "%s/1.json", object_directory);
  int fd = open(numbered_filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  int bytes_written = write(fd, "{ \"Hello\": \"World\" }", 40);
  ck_assert_int_eq(bytes_written, 40);
  // Make a symlink
  char *linkpath;
  asprintf(&linkpath, "%s/current.json", object_directory);
  int symerror = symlink(numbered_filename, linkpath);
  ck_assert_int_eq(symerror, 0);
  // Tidy Up
  free(linkpath);
  free(numbered_filename);
  free(object_directory);
  free(schema_directory);
  free(store_directory);
}

void test_delete_test_file(void)
{
  char *store_directory;
  char *schema_directory;
  char *object_directory;
  char *numbered_filename;
  char *linkpath;
  asprintf(&store_directory, "%s/%s", fixture_directory_path, "timemachine");
  asprintf(&schema_directory, "%s/%s", store_directory, "eloi");
  asprintf(&object_directory, "%s/%s", schema_directory, "weena");
  asprintf(&numbered_filename, "%s/1.json", object_directory);
  asprintf(&linkpath, "%s/current.json", object_directory);
  int unlink_err = unlink(linkpath);
  ck_assert_int_eq(unlink_err, 0);
  unlink_err = unlink(numbered_filename);
  ck_assert_int_eq(unlink_err, 0);
  rmdir(object_directory);
  rmdir(schema_directory);
  rmdir(store_directory);
  free(object_directory);
  free(schema_directory);
  free(store_directory);
  delete_fixture_directory();
}
