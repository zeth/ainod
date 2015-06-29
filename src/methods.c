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

int get(json_object *params,
        json_object **data,
        const char **error_message,
        int path_format,
        char *datadir) {

  const char *schema;
  const char *ref;
  const char *store;
  const char *id;
  const char *collection;
  char *path;
  json_object *filter_object;
  json_object *loaded_data;

  /* Check for schema */
  json_bool schema_exists = get_string(params, "schema", &schema);
  if (schema_exists == true) {
    printf("Found the %s schema.\n", schema);
  }

  /* Check for ref */
  json_bool ref_exists = get_string(params, "ref", &ref);

  if (ref_exists == true) {
    path = strdup(ref);
    printf("Found the %s path.\n", path);
  } else {
    printf("No easy ref, continuing.\n");
    /* get store */
    json_bool store_exists = get_string(params, "store", &store);
    if (store_exists == true) {
      printf("Found the %s store.\n", store);
    }
    json_bool id_exists = get_string(params, "id", &id);
    if (id_exists == true) {
      printf("Found the %s id.\n", id);
    }
    /* get filter */
    json_bool filter_exists = json_object_object_get_ex(params, "filter", &filter_object);
    if (filter_exists == true) {
      printf("Found a filter.\n");
    } else {
      printf("No filter needed\n");
    }
    /* Get collection if needed */
    if (path_format == 1) {
      json_bool collection_exists = get_string(params, "collection", &collection);
      if (collection_exists == true) {
        printf("Found the %s collection.\n", collection);
      } else {
        *error_message = AINOD_MISSING_COLLECTION;
        return JSON_SCHEMA_ERROR_INVALID_PARAMS;
      }
    }

    /* Now assemble the path */
    const char *tail = "/current.json";
    switch(path_format) {
    case 0:
      asprintf(&path, "%s/%s/%s/%s%s", datadir, store, schema, id, tail);
      break;

    case 1:
      asprintf(&path, "%s/%s/%s/%s%s", datadir, store, collection, id, tail);
      break;

    case 2:
      asprintf(&path, "%s/%s/%s/%s%s", datadir, store, id, schema, tail);
      break;
    }
  }
  *data = json_object_from_file(path);
  free(path);
  return 0;
}

int create() {
  printf("Create.\n");
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
