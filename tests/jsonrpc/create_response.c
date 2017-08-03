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


START_TEST(test_create_resource)
{
  /* Need to test success (true) and error (success is false)  */

  json_object *data = json_object_new_object();

  json_object *jstring = json_object_new_string("world");
  json_object_object_add(data,"hello", jstring);
  json_object *request_id = json_object_new_string("testresponse");

  const char *response = create_response(request_id,
                                         data,
                                         true);

  ck_assert_str_eq(response,
    "{ \"jsonrpc\": \"2.0\", \"id\": \"testresponse\", \"result\": { \"hello\": \"world\" } }");
  free((char*) response);
}
END_TEST

TCase *make_create_resource_test_case(void)
{
  TCase *test_case;
  test_case = tcase_create("CreateResource");
  tcase_add_test(test_case, test_create_resource);
  return test_case;
}
