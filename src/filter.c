#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <uuid/uuid.h>
#include <json-c/json.h>

#include "jsonschema.h"
#include "jsonhelpers.h"
#include "filter.h"

char *get_id(void) {
  char *id;
  uuid_t uuid;
  id = malloc(128);
  uuid_generate(uuid);
  uuid_unparse(uuid, id);
  return id;
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

  /** Avoid path traversal, keep data within data dir. */
  char *absolute_path;
  absolute_path = malloc(BUFSIZ);
  char *existing_path = realpath(*path, absolute_path);
  printf("AOur full path is %s.\n", absolute_path);

  if (existing_path == NULL) {
    if (errno == ENOENT) {
      printf("Path does not exist yet %d\n", errno);
    } else {
      int errsv = errno;
      char *message_buffer;
      message_buffer = malloc(BUFSIZ);
      if (message_buffer == NULL) {
        /** Out of memory, end the program */
      }
      *error_message = strerror_r(errsv, message_buffer, BUFSIZ);
      printf("Something else happened. %s\n", message_buffer);
      return errsv;
    }
  }
  printf("Datadir is %s\n", *datadir);
  /* Now we check that path beings with datadir */
  int check = strncmp(absolute_path, *datadir, strlen(*datadir));
  if (check != 0) {
    *error_message = "Path traversal error.";
    return JSON_SCHEMA_ERROR_INVALID_PARAMS;
  }
  /* TODO: Hand on the absolute path rather than raw */
  /* rewrite above slightly */


  free(absolute_path);
  return 0;
}