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

START_TEST(test_get_path_from_filter_pathformat_one)
{
  json_object *filter_object;
  char *reference;
  char *path;
  const char **error_message;
  char *datadir = "/var/lib/ainod";
  filter_object = json_object_new_object();
  json_object* storestring = json_object_new_string("catalog");
  json_object_object_add(filter_object,
                         "store",
                         storestring);
  json_object* collectionstring = json_object_new_string("product");
  json_object_object_add(filter_object,
                         "collection",
                         collectionstring);
  json_object* idstring = json_object_new_string("704e418e-682d-4ade-99be-710f2208102e");
  json_object_object_add(filter_object,
                         "id",
                         idstring);
  int result = get_path_from_filter(&filter_object,
                                    &reference,
                                    &path,
                                    1,
                                    error_message,
                                    &datadir,
                                    0);
  ck_assert_int_eq(result, 0);
  json_object_put(filter_object);
  ck_assert_str_eq(path,
                   "/var/lib/ainod/catalog/product/704e418e-682d-4ade-99be-710f2208102e");
  free(reference);
  free(path);

}
END_TEST

TCase *make_get_path_from_filter_pathformat_one_test_case(void)
{
  TCase *test_case;
  test_case = tcase_create("GetPathFromFilterPathformatOne");
  tcase_add_test(test_case, test_get_path_from_filter_pathformat_one);
  return test_case;
}
