#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include <json-c/json.h>
#include <json-c/printbuf.h>

/* Helpers for use of Json-c */

int get_string(struct json_object *params,
               const char *key,
               const char **string) {
  json_object *string_object;
  json_bool object_exists = json_object_object_get_ex(params, key, &string_object);
  if (object_exists == true) {
    *string = json_object_get_string(string_object);
  }
  return object_exists;
}


/** TODO Integrate this bit into the below,
especially get rid of JSON-C own specific
error handling.
*/
struct json_object* json_object_from_fd(int fd)
{
  struct printbuf *pb;
  struct json_object *obj;
  char buf[JSON_FILE_BUF_SIZE];
  int ret;

  if(!(pb = printbuf_new())) {
    MC_ERROR("json_object_from_file: printbuf_new failed\n");
    return NULL;
  }
  while((ret = read(fd, buf, JSON_FILE_BUF_SIZE)) > 0) {
    printbuf_memappend(pb, buf, ret);
  }
  if(ret < 0) {
    MC_ERROR("json_object_from_fd: error reading fd %d: %s\n", fd, strerror(errno));
    printbuf_free(pb);
    return NULL;
  }
  obj = json_tokener_parse(pb->buf);
  printbuf_free(pb);
  return obj;
}

int get_object_from_filename(char *filename,
                             struct json_object **data,
                             const char **error_message)
{
  printf("We have %s.\n", filename);
  int fd;
  if((fd = open(filename, O_RDONLY)) < 0) {
    char *message_buffer;
    message_buffer = malloc(BUFSIZ);
    if (message_buffer == NULL) {
      /** Out of memory, end the program */
    }
    *error_message = strerror_r(errno, message_buffer, BUFSIZ);
    free(message_buffer);
    return errno;
  }

  *data = json_object_from_fd(fd);


  close(fd);
  return 0;
}
