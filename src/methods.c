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

/** Note to support multiple results, we will need to stream back the documents */
int get(json_object *params,
        json_object **data,
        const char **error_message,
        int path_format,
        char *datadir) {

  char *path;
  json_object *filter_object;
  int fsuccess;

  /* get filter */
  json_bool filter_exists = json_object_object_get_ex(params, "filter", &filter_object);
  if (filter_exists == true) {
    fsuccess = get_path_from_filter(&filter_object, &path, path_format, error_message, &datadir, 0);
  } else {
    printf("No filter found\n");
  }

  if (fsuccess != 0) {
    return fsuccess;
  }

  char *current;
  const char *tail = "/current.json";
  asprintf(&current, "%s%s", path, tail);

  /** We have the path */

  int gsuccess = get_object_from_filename(current,
                                          data,
                                          error_message);

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

  char *path;
  json_object *filter_object;
  int fsuccess;

  json_bool filter_exists = json_object_object_get_ex(params, "filter", &filter_object);
  if (filter_exists == true) {
    fsuccess = get_path_from_filter(&filter_object, &path, path_format, error_message, &datadir, 1);
  } else {
    printf("No filter found\n");
  }

  if (fsuccess != 0) {
    return fsuccess;
  }

  printf("We got this path %s\n", path);

  /** Now look at filesstuff.c */

  free(path);
  return 0;
}

int save() {
  printf("Save.\n");
}

int delete() {
  /* Set the symlink to 0.json */
  /* Does get need to check for the 0? */
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
