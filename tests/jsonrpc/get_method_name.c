/***
  This file is part of ainod.

  Copyright 2017 Zeth

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


START_TEST(test_get_method_name)
{
  /* Test the get method name function to get the JSON-RPC method name
     from the Json object */

  json_object *root_object = json_object_new_object();

  json_object *jstring = json_object_new_string("get");
  json_object_object_add(root_object, "method", jstring);

  const char *method_name;
  int method_result = get_method_name(root_object, &method_name);

  ck_assert_int_eq(method_result, 0);
  ck_assert_str_eq(method_name, "get");
  free((char*) method_name);
  json_object_put(root_object);
}
END_TEST

START_TEST(test_get_method_name_create)
{
  /* Test the get method name function to get the JSON-RPC method name
     from the Json object */

  json_object *root_object = json_object_new_object();

  json_object *jstring = json_object_new_string("create");
  json_object_object_add(root_object, "method", jstring);

  const char *method_name;
  int method_result = get_method_name(root_object, &method_name);

  ck_assert_int_eq(method_result, 0);
  ck_assert_str_eq(method_name, "create");
  free((char*) method_name);
  json_object_put(root_object);
}
END_TEST

START_TEST(test_get_method_name_missing)
{
  /* Test the get method name function to get the JSON-RPC method name
     from the Json object */

  json_object *root_object = json_object_new_object();

  const char *method_name;
  int method_result = get_method_name(root_object, &method_name);

  ck_assert_int_eq(method_result, -32600);
  ck_assert_str_eq(method_name, "");
  json_object_put(root_object);
}
END_TEST


TCase *make_get_method_name_test_case(void)
{
  TCase *test_case;
  test_case = tcase_create("GetMethodName");
  tcase_add_test(test_case, test_get_method_name);
  tcase_add_test(test_case, test_get_method_name_create);
  tcase_add_test(test_case, test_get_method_name_missing);
  return test_case;
}
