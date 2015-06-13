#include <uuid/uuid.h>
#include <stdio.h>
#include <stdlib.h>

char *get_id(void) {
  char *id;
  uuid_t uuid;
  id = malloc(128);
  uuid_generate(uuid);
  uuid_unparse(uuid, id);
  return id;
}

/*
int main() {
  char *id = get_id();
  printf("Hello %s.\n", id);
  return 0;
}
*/
