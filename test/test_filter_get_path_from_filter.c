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

#include <np.h>	    /* NovaProva library */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <json-c/json.h>
#include "../src/filter.h"


static void test_get_path_from_filter(void)
{
  json_object *filter_object;
  char *reference;
  char *path;
  const char **error_message;
  char *datadir = "/var/lib/ainodb";
  filter_object = json_object_new_object();
  json_object* storestring = json_object_new_string("catalog");
  json_object_object_add(filter_object,
                         "store",
                         storestring);
  json_object* schemastring = json_object_new_string("product");
  json_object_object_add(filter_object,
                         "schema",
                         schemastring);
  json_object* idstring = json_object_new_string("704e418e-682d-4ade-99be-710f2208102e");
  json_object_object_add(filter_object,
                         "id",
                         idstring);
  int result = get_path_from_filter(&filter_object,
                                    &reference,
                                    &path,
                                    0,
                                    error_message,
                                    &datadir,
                                    0);
  NP_ASSERT_EQUAL(result, 0);
  json_object_put(filter_object);
  NP_ASSERT_STR_EQUAL(path,
                      "/var/lib/ainodb/catalog/product/704e418e-682d-4ade-99be-710f2208102e");
  free(path);
  free(reference);
}
