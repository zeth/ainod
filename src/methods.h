#ifndef AINOD_METHODS_H
#define AINOD_METHODS_H

char *get_id(void);

int get(json_object *params,
        json_object **data,
        const char **error_message,
        int path_format,
        char *datadir);

int create(void);

int delete(void);

int update(void);

int replace(void);

int reindex(void);

#define AINOD_MISSING_COLLECTION "Missing Collection parameter"\
  " (as required when the Path-format setting is set to 1)."


#endif /* AINOD_METHODS_H */
