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

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <json-c/json.h>
#include "jsonschema.h"
#include "jsonrpc.h"
#include "methods.h"
#include "constants.h"

/** Support of JSON RPC within Ainod */

const char *create_response(json_object *request_id,
                            json_object *data,
                            bool success) {
  /*Create a json object*/
  json_object *response = json_object_new_object();
  /*Add the protocol version*/
  json_object *protversion = json_object_new_string("2.0");
  json_object_object_add(response,"jsonrpc", protversion);
  json_object_object_add(response, "id", request_id);
  if (success == true) {
    json_object_object_add(response, "result", data);
  } else {
    json_object_object_add(response, "error", data);
  }

  const char *response_text;
  response_text = strdup(json_object_to_json_string(response));
  /*Now printing the json object*/
  if (DEBUG == true) {
    printf ("The json object created: %s \n",
            response_text);
  }
  json_object_put(response);
  return response_text;
}

int init_error_object(json_object **error_object,
                      int error_code,
                      const char *message) {
  /* Add the error code and message */
  json_object * error_code_obj = json_object_new_int(error_code);
  json_object * message_obj = json_object_new_string(message);
  json_object_object_add(*error_object, "code", error_code_obj);
  json_object_object_add(*error_object, "message", message_obj);
  return 0;
};

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


int get_request_id(json_object **identifier,
                   json_object *root_object,
                   bool silentnote,
                   char *req_id_format,
                   bool req_req_id) {
  //json_object *identifier;
  json_bool answer;
  // here
  const char *id_string;
  answer = json_object_object_get_ex(root_object, "id", identifier);
  if (answer == true) {
    /* We an have id, (but don't know the type yet) */
    if (strcmp(req_id_format, "default") == 0) {
      /* we don't care about the type, type testing is for wimps... */
      return 0;
    }
    /* ... or perhaps not! It seems you still do care about the
       type. */
    enum json_type id_type = json_object_get_type(*identifier);
    if (id_type == json_type_string) {
      if (strcmp(req_id_format, "string") == 0) {
        /* Client wanted a string and got one, Merry Christmas. */
        return 0;
      }
      if (strcmp(req_id_format, "rpc") == 0) {
        return 0;
      }
      /* if we got here then we wanted an int but got a string, throw
         an error back to the client! */
      return JSON_SCHEMA_ERROR_INVALID_REQUEST;
    }
    if (id_type == json_type_int) {
      if (strcmp(req_id_format, "int") == 0) {
        /* Client wanted an int and got one, Merry Christmas. */
        return 0;
      }
      if (strcmp(req_id_format, "rpc") == 0) {
        return 0;
      }
      /* if we got here then we wanted a string but got an int, throw
         error to client. */
      return JSON_SCHEMA_ERROR_INVALID_REQUEST;
    }
    if (id_type == json_type_double) {
      if (strcmp(req_id_format, "rpc") == 0) {
        return 0;
      }
      /* if we got here then we wanted a string or an int but got a
         number, throw error to client. */
      return JSON_SCHEMA_ERROR_INVALID_REQUEST;
    }
    if (id_type == json_type_null) {
      if (strcmp(req_id_format, "rpc") == 0) {
        if (silentnote) {
          /* We have a notification */
          return AINOD_RPC_SILENT_NOTIFICATION;
        }
        /* Needs to be more complicated here. */
        return 0;
      }
      /* if we got here then we wanted a string or an int but got a
         null, throw error to client. */
      return JSON_SCHEMA_ERROR_INVALID_REQUEST;
    }
    /* if we got here then the request id needs to be a particular
       type or set of types but is an array or an int so throw an error*/
    return JSON_SCHEMA_ERROR_INVALID_REQUEST;
  } else {
    /* No id found */
    if (silentnote) {
      /* We have a notification */
      return AINOD_RPC_SILENT_NOTIFICATION;
    } else {
      if (req_req_id) {
        /* Id is required but not present */
        return JSON_SCHEMA_ERROR_INVALID_REQUEST;
      }
      /* No request id at all but it is not required in this case. So
         lets set it to 0.*/
      *identifier = json_object_new_int(0);
      return 0;
    }

  }
  return 0;
}

const char *handle(const char *method_name,
                   json_object *request_id,
                   json_object *root_object,
                   int path_format,
                   char *datadir,
                   bool silent) {
  json_object *params;
  int error;
  const char *error_message;
  const char* response_text;
  json_object *data;
  json_bool params_exists;
  params_exists = json_object_object_get_ex(root_object, "params", &params);
  if (params_exists == true) {
    printf("Bingo\n");
  } else {
    printf("Game over\n");
  }

  /** Methods need to reply with data and with error code */
  switch(method_name[0]) {
  case 'g': //get
    error = get(params, &data, &error_message, path_format, datadir);
    break;
  case 'c': //Create
    error = create(params, &data, &error_message, path_format, datadir);
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
  case 's': //Save
    save();
    break;
  }

  if (silent) {
    //free(error_message);
    return "";
  }

  if (error == 0) {
    //data = json_object_new_string("Hello World!");
    response_text = create_response(request_id,
                                    data,
                                    true);
  } else {
    printf("error here jim;\n");
    json_object * error_object = json_object_new_object();
    init_error_object(&error_object,
                      error,
                      error_message);
    response_text = create_response(request_id,
                                    error_object,
                                    false);

  }
  //  free(error_message);
  return response_text;
}

const char *process_buffer(char *buf,
                           bool silentnote,
                           char *req_id_format,
                           bool req_req_id,
                           char *datadir,
                           int path_format) {
  json_object *root_object;
  const char *response_text;
  root_object = json_tokener_parse(buf);
  const char *method_name = get_method_name(root_object);
  json_object *identifier;
  int success = get_request_id(&identifier,
                               root_object,
                               silentnote,
                               req_id_format,
                               req_req_id);
  if (success == 0) {
    response_text = handle(method_name, identifier,
                           root_object, path_format,
                           datadir, false);
    /* Do param errors have to propegate all the way to here? I don't
       think so... */

  } else if (success == AINOD_RPC_SILENT_NOTIFICATION) {
    response_text = "";
    handle(method_name, identifier,
           root_object, path_format,
           datadir, true);
  } else if (success == JSON_SCHEMA_ERROR_INVALID_REQUEST) {
    json_object * error_object = json_object_new_object();
    init_error_object(&error_object,
                      JSON_SCHEMA_ERROR_INVALID_REQUEST,
                      AINOD_INVALID_REQ_ID);
    response_text = create_response(identifier,
                                    error_object,
                                    false);
    //json_object_put(error_object);
  }
  // tidy up before return
  json_object_put(root_object);
  return response_text;
}
