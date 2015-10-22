/* Helpers for use of Json-c */

#ifndef AINOD_JSONHELPERS_H
#define AINOD_JSONHELPERS_H


int get_string(struct json_object *params,
               const char *key,
               const char **string);

int get_object_from_filename(char *filename,
                             struct json_object **data,
                             const char **error_message);


#endif /* AINOD_JSONHELPERS_H */
