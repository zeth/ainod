/***
  This file is part of ainod.

  Copyright 2015 Zeth

  ainod is free software; you can redistribute it and/or modify it
  under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 2.1 of the License, or
  (at your option) any later version.

  ainod is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with ainod; If not, see <http://www.gnu.org/licenses/>.
***/
#define _GNU_SOURCE
#include <json-c/json.h>

START_TEST(test_init_error_object)
{
  /* Need to test success (true) and error (success is false)  */
  json_object *error_object = json_object_new_object();
  const char *message = "Pretend Error";
  int error_code = 12345;
  int success = init_error_object(&error_object, error_code, message);
  ck_assert_int_eq(success, 0);
  const char* result = json_object_to_json_string(error_object);

  ck_assert_str_eq(result,
                   "{ \"code\": 12345, \"message\": \"Pretend Error\" }");
  json_object_put(error_object);
}
END_TEST

TCase *make_init_error_object_test_case(void)
{
  TCase *test_case;
  test_case = tcase_create("InitErrorObject");
  tcase_add_test(test_case, test_init_error_object);
  return test_case;
}
