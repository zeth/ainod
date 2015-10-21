#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <uuid/uuid.h>
#include <json-c/json.h>

#include "jsonschema.h"
#include "methods.h"



char *get_id(void) {
  char *id;
  uuid_t uuid;
  id = malloc(128);
  uuid_generate(uuid);
  uuid_unparse(uuid, id);
  return id;
}

int get_string(json_object *params,
               const char *key,
               const char **string) {
  json_object *string_object;
  json_bool object_exists = json_object_object_get_ex(params, key, &string_object);
  if (object_exists == true) {
    *string = json_object_get_string(string_object);
  }
  return object_exists;
}


int get_path_from_filter(json_object **filter,
                         char **path,
                         int path_format,
                         const char **error_message,
                         char **datadir,
                         int create) {

  const char *schema;
  const char *ref;
  const char *store;
  const char *id;
  const char *collection;

  /* Check for schema */
  json_bool schema_exists = get_string(*filter, "schema", &schema);
  if (schema_exists == true) {
    printf("Found the %s schema.\n", schema);
  }

  /* Check for ref */
  json_bool ref_exists = get_string(*filter, "ref", &ref);

  if (ref_exists == true) {
    asprintf(path, "%s/%s", *datadir, ref);
  } else {

    /* get store */
    json_bool store_exists = get_string(*filter, "store", &store);
    if (store_exists == true) {
      printf("Found the %s store.\n", store);
    } else {
      *error_message = AINOD_MISSING_STORE;
      return JSON_SCHEMA_ERROR_INVALID_PARAMS;
    }

    /* Get collection if needed */
    if (path_format == 1) {
      json_bool collection_exists = get_string(*filter, "collection", &collection);
      if (collection_exists == true) {
        printf("Found the %s collection.\n", collection);
      } else {
        *error_message = AINOD_MISSING_COLLECTION;
        return JSON_SCHEMA_ERROR_INVALID_PARAMS;
      }
    }

    json_bool id_exists = get_string(*filter, "id", &id);
    if (id_exists == true) {
      printf("Found the %s id.\n", id);
    } else {
      if (create == 1) {
        printf("Create ID\n");
        id = get_id();
        printf("We made %s\n", id);
      } else {
        printf("This is the end of the road, megatron");
          /** Handle this somehow */
      }
    }

    /* Now assemble the path */
    switch(path_format) {
    case 0:
      asprintf(path, "%s/%s/%s/%s", *datadir, store, schema, id);
      break;

    case 1:
      asprintf(path, "%s/%s/%s/%s", *datadir, store, collection, id);
      break;

    case 2:
      asprintf(path, "%s/%s/%s/%s", *datadir, store, id, schema);
      break;
    }
  }

  return 0;
}

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

  *data = json_object_from_file(current);
  /** Note to support multiple results, we will need to stream back the documents */

  free(path);
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

  free(path);
  return 0;
}

int save() {
  printf("Save.\n");
}

int delete() {
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
