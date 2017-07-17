#define _GNU_SOURCE
#include <stdio.h>

/* src/daemon/configparser.c */
/* string_is_in_array */

START_TEST(test_string_is_in_array)
{
  char *sections[] = {"Daemon", "Other"};
  int answer = string_is_in_array("Other", sections, 2);
  int wronganswer = string_is_in_array("Angel", sections, 2);
  ck_assert_int_eq(answer, 0);
  ck_assert_int_eq(wronganswer, -1);
}
END_TEST



TCase *make_string_is_in_array_case(void)
{
  TCase *test_case;
  test_case = tcase_create("GetKeyValue");
  tcase_add_test(test_case, test_string_is_in_array);
  return test_case;
}
