#define _GNU_SOURCE
#include <stdio.h>

#include <json-c/json.h>

/* Test case for function: create_file */
/* which is found in src/helpers.c */

START_TEST(test_create_file)
{
  // Create a little JSON object
  json_object * jobj = json_object_new_object();
  json_object *jstring = json_object_new_string("Casio F-91W");
  json_object_object_add(jobj, "Watch", jstring);
  json_object *jint = json_object_new_int(3);
  json_object_object_add(jobj, "Number of buttons", jint);

  // Use the function to create a file
  int dirfd = AT_FDCWD;
  const char *error_message;
  int result = create_file(dirfd,
                           fixture_directory_path,
                           1,
                           jobj,
                           &error_message);
  json_object_put(jobj);

  // Check the file has the correct contents
  check_contents_numbered_file(1,
                               "{\n   \"Watch\": \"Casio F-91W\",\n   \"Number of buttons\": 3\n }");

  // Clean up
  delete_numbered_file(1);
  delete_current_symlink();
}
END_TEST


TCase *create_file_test_case(void)
{
  TCase *test_case;
  test_case = tcase_create("CreateFile");
  tcase_add_checked_fixture(test_case,
                            create_fixture_directory,
                            delete_fixture_directory);
  tcase_add_test(test_case, test_create_file);
  return test_case;
}
