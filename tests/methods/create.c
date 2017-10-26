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


/**

int create(json_object *params,
           json_object **data,
           const char **error_message,
           int path_format,
           char *datadir)

*/

START_TEST(test_create_missing_document)
{
  json_object *params;
  json_object *filter;
  const char *error_message;
  json_object *data;
  params = json_object_new_object();
  filter = json_object_new_object();
  json_object *store = json_object_new_string("timemachine");
  json_object_object_add(filter, "store", store);
  json_object *schema = json_object_new_string("morlocks");
  json_object_object_add(filter, "schema", schema);
  json_object *identifier = json_object_new_string("nebogipfel");
  json_object_object_add(filter, "id", identifier);
  json_object_object_add(params,
                         "filter",
                         filter);
  int result = create(params, &data, &error_message, 0, fixture_directory_path);
  ck_assert_int_eq(result, -20000);
  const char* datastring = json_object_to_json_string(data);
  ck_assert_str_eq("The create method requires a 'document' object inside 'params'.", error_message);
  json_object_put(data);
  json_object_put(params);
  json_object_put(filter);
}
END_TEST


/** It is already created by the fixture so we test its existence **/
START_TEST(test_create)
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

TCase *make_create_test_case(void)
{
  TCase *test_case;
  test_case = tcase_create("CreateMethod");
  tcase_add_checked_fixture(test_case,
                            test_create_test_file,
                            test_delete_test_file);
  tcase_add_test(test_case, test_create);
  tcase_add_test(test_case, test_create_missing_document);
  return test_case;
}
