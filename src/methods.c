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
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include <json-c/json.h>

#include "jsonschema.h"
#include "methods.h"
#include "jsonhelpers.h"
#include "filter.h"
#include "helpers.h"
#include "jsonrpc.h"

/** Note to support multiple results, we will need to stream back the documents */
int get(json_object *params,
        json_object **data,
        const char **error_message,
        int path_format,
        char *datadir) {

  char *path;
  char *reference;
  json_object *filter_object;

  /* get filter */
  json_bool filter_exists = json_object_object_get_ex(params, "filter", &filter_object);
  if (filter_exists == true) {
    int fsuccess = get_path_from_filter(&filter_object, &reference, &path, path_format, error_message, &datadir, 0);
    if (fsuccess != 0) {
      return fsuccess;
    }
  } else {
    printf("No filter found\n");
  }


  char *current;
  const char *tail = "/current.json";
  asprintf(&current, "%s%s", path, tail);

  /** We have the path */

  int gsuccess = get_object_from_filename(current,
                                          data,
                                          error_message);
  if (filter_exists == true) {
    free(reference);
  }
  free(path);
  free(current);

  if (gsuccess != 0) {
    return gsuccess;
  }

  return 0;
}

int create(json_object *params,
           json_object **data,
           const char **error_message,
           int path_format,
           char *datadir) {
  printf("Create.\n");

  char *reference;
  char *path;
  json_object *filter_object;

  json_bool filter_exists = json_object_object_get_ex(params, "filter", &filter_object);
  if (filter_exists == true) {
    int fsuccess = get_path_from_filter(&filter_object, &reference, &path, path_format, error_message, &datadir, 1);
    if (fsuccess != 0) {
      return fsuccess;
    }
  } else {
    printf("No filter found\n");
  }

  json_object *document_object;
  json_bool document_exists = json_object_object_get_ex(params, "document", &document_object);


  if (document_exists == false) {
      *error_message = AINOD_CREATE_MISSING_DOCUMENT;
      return AINOD_METHOD_CREATE_ERROR;
  }

  /** TODO: go off for validaton here. **/

  int result = create_new_file(path, document_object, error_message);
  free(path);
  if (result == 0) {
    *data = json_object_new_object();
    json_object *created = json_object_new_string(reference);
    json_object_object_add(*data, "created", created);
    free(reference);
  }

  return result;
}

int save() {
  printf("Save.\n");
}

int delete() {
  /* Set the symlink to e.g. 4.deletedmarker.json */
  printf("Delete.\n");
}

int update() {
  printf("Update.\n");
}

int replace() {
  printf("Replace.\n");
}

int reindex() {
  printf("Index.\n");
}
