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
#include "../src/methods.h"

/** By simple we mean that everything complicated is mocked. */

int mock_get_path_from_filter(json_object **filter,
                              char **path,
                              int path_format,
                              const char **error_message,
                              char **datadir,
                              int create) {
    asprintf(path, "/tmp/nothing.json");
    return 0;
}

int mock_get_object_from_filename(char *filename,
                                  struct json_object **data,
                                  const char **error_message) {
  *data = json_object_new_object();
  json_object* valuestring = json_object_new_string("World");
  json_object_object_add(*data,
                         "Hello",
                         valuestring);
  return 0;
}


static void test_get_simple(void)
{

  json_object *params;
  json_object *filter;
  const char *error_message;
  json_object *data;
  params = json_object_new_object();
  filter = json_object_new_object();
  json_object_object_add(params,
                         "filter",
                         filter);
  int result = get(params, &data, &error_message, 0, "/tmp/");
  NP_ASSERT_EQUAL(result, 0);
  const char* datastring = json_object_to_json_string(data);
  NP_ASSERT_STR_EQUAL("{ \"Hello\": \"World\" }", datastring);
  json_object_put(data);
  json_object_put(params);
}
