#ifndef AINOD_METHODS_H
#define AINOD_METHODS_H

char *get_id(void);

int get(json_object *params,
        json_object **data,
        const char **error_message,
        int path_format,
        char *datadir);

int create(json_object *params,
        json_object **data,
        const char **error_message,
        int path_format,
        char *datadir);

int delete(void);

int update(void);

int replace(void);

int reindex(void);

int save(void);


#endif /* AINOD_METHODS_H */
