#ifndef AINOD_FILTER_H
#define AINOD_FILTER_H

#include <json-c/json.h>

#define AINOD_MISSING_COLLECTION "Missing collection parameter"\
  " (as required when the Path-format setting is set to 1)."

#define AINOD_MISSING_STORE "Missing store parameter"\
  " (as required when the Path-format setting is set to 0 or 2)."

char *get_id(void);

int get_path_from_filter(json_object **filter,
                         char **path,
                         int path_format,
                         const char **error_message,
                         char **datadir,
                         int create);


#endif /* AINOD_FILTER_H */
