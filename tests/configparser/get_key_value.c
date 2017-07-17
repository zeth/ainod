#define _GNU_SOURCE
#include <stdio.h>

/* src/daemon/configparser.c */
/* get_key_value */

START_TEST(test_get_key_value)
{
  char *line = "Workers=4";
  char *key;
  char *value;
  get_key_value(line, &key, &value);
  ck_assert_str_eq(key, "Workers");
  ck_assert_str_eq(value, "4");
  free(key);
  free(value);
  printf("\nHello\n\n");
}
END_TEST



TCase *make_get_key_value_test_case(void)
{
  TCase *test_case;
  test_case = tcase_create("GetKeyValue");
  tcase_add_test(test_case, test_get_key_value);
  return test_case;
}
