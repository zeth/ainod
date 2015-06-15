#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <uuid/uuid.h>
#include <json-c/json.h>
#include "jsonschema.h"
#include "methods.h"


int get() {
  printf("Get.\n");
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

int handle(const char *method_name) {
  switch(method_name[0]) {
  case 'g': //get
     get();
    break;
  case 'c': //Create
    create();
    break;
  case 'd': //Delete
    delete();
    break;
  case 'u': //Update
    update();
    break;
  case 'r': //Replace
    replace();
    break;
  case 'i': //Index
    reindex();
    break;
  }
  printf("The letter is %c.\n", method_name[0]);
}

const char *get_method_name(json_object *root_object) {
  json_object *method_object;
  json_bool answer;
  const char *method_name;
  answer = json_object_object_get_ex(root_object, "method", &method_object);
  if (answer == true) {
      method_name = json_object_get_string(method_object);
  } else {
    // Send back an error to the client
  }
  return method_name;
}

int process_buffer(char *buf,
                   bool silentnote) {
  json_object *root_object;
  root_object = json_tokener_parse(buf);
  const char *method_name = get_method_name(root_object);
  handle(method_name);
  // tidy up
  json_object_put(root_object);
  return 0;
}


char *get_id(void) {
  char *id;
  uuid_t uuid;
  id = malloc(128);
  uuid_generate(uuid);
  uuid_unparse(uuid, id);
  return id;
}
