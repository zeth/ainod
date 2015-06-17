#include <stdlib.h>
#include <stdio.h>
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


int create_error();

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


const char *get_method_name(json_object *root_object) {
  json_object *method_object;
  json_bool answer;
  const char *method_name;
  answer = json_object_object_get_ex(root_object, "method", &method_object);
  if (answer == true) {
    method_name = json_object_get_string(method_object);
  } else {
    // Send back an error to the client
    printf("1. Send an error to client please.\n");
  }
  return method_name;
}

json_object *get_request_id(json_object *root_object,
                            bool silentnote,
                            char *req_id_format) {
  json_object *identifier;
  json_bool answer;
  // here
  const char *id_string;
  answer = json_object_object_get_ex(root_object, "id", &identifier);
  if (answer == true) {
    /* We an have id, (but don't know the type yet) */
    if (strcmp(req_id_format, "default") == 0) {
      /* we don't care about the type, type testing is for wimps... */
      return identifier;
    }
    /* ... or perhaps not! It seems you still do care about the
       type. */
    enum json_type id_type = json_object_get_type(identifier);
    printf("id_type is %d.\n", id_type);
    printf("My format is now %s.\n", req_id_format);
    if (id_type == json_type_string) {
      if (strcmp(req_id_format, "string") == 0) {
        /* Client wanted a string and got one, Merry Christmas. */
        return identifier;
      }
      if (strcmp(req_id_format, "rpc") == 0) {
        return identifier;
      }
      /* if we got here then we wanted an int but got a string, throw
         an error back to the client! TODO! */
      printf("2. Send an error to client please.\n");
    }
    if (id_type == json_type_int) {
      if (strcmp(req_id_format, "int") == 0) {
        /* Client wanted an int and got one, Merry Christmas. */
        return identifier;
      }
      if (strcmp(req_id_format, "rpc") == 0) {
        return identifier;
      }
      /* if we got here then we wanted a string but got an int, throw
         error to client. */
      printf("3. Send an error to client please.\n");
    }
    if (id_type == json_type_double) {
      if (strcmp(req_id_format, "rpc") == 0) {
        return identifier;
      }
      /* if we got here then we wanted a string or an int but got a
         number, throw error to client. */
      printf("4. Send an error to client please.\n");
    }
    if (id_type == json_type_null) {
      if (strcmp(req_id_format, "rpc") == 0) {
        /* Needs to be more complicated here. */
        return identifier;
      }
      /* if we got here then we wanted a string or an int but got a
         null, throw error to client. */
      printf("5. Send an error to client please.\n");
    }
    /* if we got here then the request id needs to be a particular
       type or set of types but is an array or an int so throw an error*/
    printf("6. Send an error to client please.");
  } else {
    /* No id found */
    if (silentnote) {
      printf("Must be a notification.");
    } else {
      printf("Missing id altogether.");
    }

  }
  return identifier;
}

const char *create_response(json_object *request_id) {
  /*Create a json object*/
  json_object * response = json_object_new_object();
  /*Add the protocol version*/
  json_object *protversion = json_object_new_string("2.0");
  json_object_object_add(response,"jsonrpc", protversion);
  json_object_object_add(response, "id", request_id);

  const char *response_text;
  response_text = json_object_to_json_string(response);
  /*Now printing the json object*/
  printf ("The json object created: %s \n",
          response_text);
  return response_text;
  //json_object_put(response);
}

const char *handle(const char *method_name,
                   json_object *request_id) {
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
  const char* response_text;
  response_text = create_response(request_id);
  return response_text;
}

const char *process_buffer(char *buf,
                           bool silentnote,
                           char *req_id_format) {
  json_object *root_object;
  root_object = json_tokener_parse(buf);
  const char *method_name = get_method_name(root_object);
  json_object *request_id = get_request_id(root_object,
                                           silentnote,
                                           req_id_format);
  const char *response_text = handle(method_name, request_id);
  return response_text;
  // tidy up
  //json_object_put(root_object);
  //return 0;
}
