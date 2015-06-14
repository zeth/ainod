#ifndef AINOD_METHODS_H
#define AINOD_METHODS_H


/** Invalid JSON was received by the server. **/
#define JSON_SCHEMA_ERROR_PARSE_ERROR -32700

/** The JSON sent is not a valid Request object. */
#define JSON_SCHEMA_ERROR_INVALID_REQUEST -32600

/** The method does not exist / is not available. **/
#define JSON_SCHEMA_ERROR_METHOD_NOT_FOUND -32601

/** Invalid method parameter(s). */
#define JSON_SCHEMA_ERROR_INVALID_PARAMS -32602

/** Internal JSON-RPC error. */
#define JSON_SCHEMA_ERROR_INTERNAL_ERROR -32603

/** Reserved for implementation-defined server-errors. */
#define JSON_SCHEMA_ERROR_SERVER_ERROR -32000

char *get_id(void);
int process_buffer(char *buf);

#endif /* AINOD_METHODS_H */
