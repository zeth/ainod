//int create_new_file(char *path,
//                    json_object *document,
//                   const char **error_message)

#define _GNU_SOURCE
#include <stdio.h>

#include <json-c/json.h>

/* Test case for function: create_new_file */
/* which is found in src/helpers.c */

START_TEST(test_create_new_file)
{

  // Create a little JSON object
  json_object * document = json_object_new_object();
  json_object *jstring = json_object_new_string("Amstrad CPC");
  json_object_object_add(document, "Computer", jstring);
  json_object *jint = json_object_new_int(6128);
  json_object_object_add(document, "Model", jint);

  // Use the function to create a file
  const char *error_message;
  int result = create_new_file(fixture_directory_path,
                               document,
                               &error_message);
  ck_assert_int_eq(result, 0);
  json_object_put(document);

  // Check the file has the correct contents
  check_contents_numbered_file(1,
                               "{\n   \"Computer\": \"Amstrad CPC\",\n   \"Model\": 6128\n }");

  // Clean up
  delete_numbered_file(1);
  delete_current_symlink();
}
END_TEST


TCase *create_new_file_test_case(void)
{
  TCase *test_case;
  test_case = tcase_create("CreateFile");
  tcase_add_checked_fixture(test_case,
                            create_fixture_directory,
                            delete_fixture_directory);
  tcase_add_test(test_case, test_create_new_file);
  return test_case;
}
