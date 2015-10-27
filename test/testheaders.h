#ifndef AINOD_TEST_H
#define AINOD_TEST_H

const char *create_response(json_object *request_id,
                            json_object *data,
                            bool success);

int init_error_object(json_object **error_object,
                      int error_code,
                      const char *message);


#endif /* AINOD_TEST_H */
