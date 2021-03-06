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


START_TEST(test_get_simple)
{
  json_object *params;
  json_object *filter;
  const char *error_message;
  json_object *data;
  params = json_object_new_object();
  filter = json_object_new_object();
  json_object *store = json_object_new_string("timemachine");
  json_object_object_add(filter, "store", store);
  json_object *schema = json_object_new_string("eloi");
  json_object_object_add(filter, "schema", schema);
  json_object *identifier = json_object_new_string("weena");
  json_object_object_add(filter, "id", identifier);
  json_object_object_add(params,
                         "filter",
                         filter);
  int result = get(params, &data, &error_message, 0, fixture_directory_path);
  ck_assert_int_eq(result, 0);
  const char* datastring = json_object_to_json_string(data);
  ck_assert_str_eq("{ \"Hello\": \"World\" }", datastring);
  json_object_put(data);
  json_object_put(params);
  json_object_put(filter);
}
END_TEST

TCase *make_get_simple_test_case(void)
{
  TCase *test_case;
  test_case = tcase_create("GetSimple");
  tcase_add_checked_fixture(test_case,
                            test_create_test_file,
                            test_delete_test_file);
  tcase_add_test(test_case, test_get_simple);
  return test_case;
}
