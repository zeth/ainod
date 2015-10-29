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

#define _GNU_SOURCE

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <search.h>
#include <stdbool.h>
#include "configparser.h"
#include "hfunctions.h"
#include "handleerror.h"


char *SECTIONS[] = {"Daemon", "Other"};
int SECTIONS_LENGTH = 2;

int string_is_in_array(char *string, char *array[], int length) {
  int i = 0;
  while(i < length) {
    if(strcmp(array[i], string) == 0) {
      return 0;
    }
    i++;
  }
  return -1;
}

int get_key_value(char *str, char **key, char **value)
{
  char *lstr = strdup(str);
  const char s[2] = "=";
  char *lkey;
  char *lvalue;
  lkey = strtok_r(lstr, s, &lvalue);
  *key = (char *)malloc(strlen(lkey)+1);
  *value = (char *)malloc(strlen(lvalue)+1);
  strcpy(*key, lkey);
  strcpy(*value, lvalue);
  free(lstr);
  return(0);
}

int fd_warn_permissions(const char *path, int fd) {
  struct stat st;

  if (fstat(fd, &st) < 0)
    return -errno;

  if (st.st_mode & 0111)
    handle_error("Configuration file %s is marked executable. Please remove executable permission bits. Proceeding anyway.", path);

  if (st.st_mode & 0002)
    handle_error("Configuration file %s is marked world-writable. Please remove world writability permission bits. Proceeding anyway.", path);

  if (getpid() == 1 && (st.st_mode & 0044) != 0044)
    handle_error("Configuration file %s is marked world-inaccessible. This has no effect as configuration data is accessible via APIs without restrictions. Proceeding anyway.", path);

  return 0;
}

char *truncate_nl(char *s) {
  assert(s);

  s[strcspn(s, NEWLINE)] = 0;
  return s;
}

char *strstrip(char *s) {
  char *e;
  /* Drops trailing whitespace. Modifies the string in
   * place. Returns pointer to first non-space character */

  s += strspn(s, WHITESPACE);

  for (e = strchr(s, 0); e > s; e --)
    if (!strchr(WHITESPACE, e[-1]))
      break;

  *e = 0;

  return s;
}

char *strnappend(const char *s, const char *suffix, size_t b) {
  size_t a;
  size_t total;
  if (!s && !suffix) return strdup("");

  if (!s) return strndup(suffix, b);

  if (!suffix) return strdup(s);

  assert(s);
  assert(suffix);
  a = strlen(s);
  if (b > ((size_t) -1) - a) return NULL;

  total = a + b + 1;
  char *r = (char *) malloc (total);
  if (!r) return NULL;

  memcpy(r, s, a);
  memcpy(r+a, suffix, b);
  r[a+b] = 0;

  return r;
}

char *strappend(const char *s, const char *suffix) {
  return strnappend(s, suffix, suffix ? strlen(suffix) : 0);
}

char *parse_line(char *line) {
  /* Put line parsing code here */
  line = strstrip(line);
  /** Ignore blank lines */
  if (!*line) {
    return 0;
  }
  /** Ignore comment lines */
  if (strchr(COMMENTS "\n", *line)) {
    return 0;
  }
  /** Section headers */
  if (*line == '[') {
    size_t length;
    length = strlen(line);
    /** Check for end char */
    if (line[length-1] != ']') {
      handle_error("Invalid section header '%s' in config file. Missing ].",
                   line);
    }
    /** Make a copy of section header without the square brackets */
    char *header;
    header = strndup(line+1, length-2);
    if (!header) {
      /* If we got here means we have ran out of memory. Running out
         of memory when we are parsing the config file is not such a
         good start, are you running this on an abacus?
      */
      handle_error("Out of memory");
    }
    /** Check is header is in allowed SECTIONS */
    int result = string_is_in_array(header, SECTIONS, SECTIONS_LENGTH);
    if (result==-1) {
      handle_error("Invalid section header %s in config file.",
                   header);
    }
    /** We should eventually do something with the header, such as
        call a fuction to handle it or return it to the parent. For
        now just bin it and push on. */
    free(header);
    return 0;
  }
  /** Now we are down to standard config lines, lets check we have an
      equals sign. */
  char *equals = strchr(line, '=');
  if (!equals) {
    handle_error("Missing '='. in config file declaration. Line reads: %s",
                 line);
  }
  /* Now should have a valid line. */
  return line;
}

int free_it_all(ENTRY *entry) {
  free(entry->key);
  free(entry->data);
  return 0;
}

int delete_store(struct hsearch_data *store) {
  hiter_items_r(store, free_it_all);
  hdestroy_r(store);
  free(store);
  return 0;
}

struct hsearch_data *new_store(void) {
  struct hsearch_data *store;
  store = calloc(1, sizeof(struct hsearch_data));
  hcreate_r(CONFIG_STORE_MAX, store);
  return store;
}

int parse_config(struct hsearch_data *store,
                 const char *filename) {
  FILE *f = NULL;
  f = fopen(filename, "re");
  if (!f) {
    handle_error("Failed to open configuration file '%s': %m", filename);
  }
  fd_warn_permissions(filename, fileno(f));

  while (!feof(f)) {
    char l[LINE_MAX];

    /* Get a line  */
    if (!fgets(l, sizeof(l), f)) {
      if (feof(f))
        break;
      handle_error("Failed to read configuration file '%s'", filename);
    }

    /* Remove newline from end of line */
    truncate_nl(l);

    /* Parse the line */
    char *line = parse_line(l);

    /* Split line into key and value */
    if (line) {
      char *key;
      char *value;
      get_key_value(line, &key, &value);
      ENTRY new_entry;
      ENTRY *return_val;
      new_entry.key = strdup(key);
      new_entry.data = strdup(value);
      hsearch_r(new_entry, ENTER, &return_val, store);
      free(key);
      free(value);
    }
  }

  fclose(f);

  return 0;
}

char *search_store(struct hsearch_data *store,
                   char *key) {
    ENTRY search, *search_result;
    search.key = key;
    hsearch_r(search, FIND, &search_result, store);
    if (search_result == NULL) {
      return NULL;
    } else {
      return (char *)search_result->data;
    }
    return 0;
}

/** Check Workers setting. If not set, then use number of CPUs. */
int check_workers(struct hsearch_data *store) {
  char *process = search_store(store, "Workers");
  int processors_int;
  if (process) {
    processors_int = atoi(process);
  } else {
    processors_int = (int)sysconf(_SC_NPROCESSORS_ONLN);
  }
  return processors_int;
}

/** Check for a string setting, if not set use `default_setting` */
char *check_string_setting(struct hsearch_data *store,
                           char *key,
                           char *default_setting) {
  char *setting = search_store(store, key);
  if (!setting) {
    setting = default_setting;
  }
  return setting;
}

/** Check Datadir setting, if not set use /var/lib/ainod */
char *check_data_dir(struct hsearch_data *store) {
  char *datadir = search_store(store, "Datadir");
  if (!datadir) {
    datadir = "/var/lib/ainod";
  }
  return datadir;
}

/** Check boolean setting, if not set defaults to false */
bool check_boolean_setting(struct hsearch_data *store,
                            char *setting_name) {
  char *setting = search_store(store, setting_name);
  if (!setting) {
    return false;
  }
  if (strcmp(setting, "true") == 0) {
    return true;
  } else {
    return false;
  }
}

/** Check integer setting, if not defaults to 0 */
int check_int_setting(struct hsearch_data *store,
                      char *setting_name) {
  char *setting = search_store(store, setting_name);
  if (!setting) {
    return 0;
  } else {
    return strtol(setting, (char **)NULL, 10);
  }
}


//int main(void) {
//  struct hsearch_data *store = new_store();
//  parse_config(store, "ainod.conf");
//  search_store(store, "Workers");
//  search_store(store, "Datadir");
//  delete_store(store);
//}
