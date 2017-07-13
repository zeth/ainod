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
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>
#include <limits.h>
#include <json-c/json.h>
#include "helpers.h"

int create_document_dir(int dirfd, char *path) {
  int success = mkdirat(dirfd, path, S_IRWXU|S_IRWXG|S_IROTH);
  if (success == 0) {
    return 0;
  } else {
    int errsv = errno;
    return errsv;
  }
};

/** Go through the whole path and make sure each directory exists **/
int make_parents(int dirfd, char *directory) {
  char *remainder = directory;
  int error;
  while (*remainder == '/')
    remainder++; /* Gets rid of leading slash */
  for (;;) {
    strsep(&remainder, "/");
    if (remainder == NULL) {
      /* We are finished */
      error = 0;
      break;
      }
    while (*remainder == '/')
      remainder++;
    int fd;
    if ((fd = mkdirat(dirfd, directory, S_IRWXU|S_IRWXG|S_IROTH)) < 0 && errno != EEXIST) {
      error = errno;
      printf("Problem! %d\n", errno);
      break;
    }
    if ((fd = openat(dirfd, directory, O_DIRECTORY)) < 0) {
      printf("Problem! %d\n", errno);
      error = errno;
      break;
    }
    if (dirfd != AT_FDCWD) {
      close(dirfd);
    }
    dirfd = fd;
    directory = remainder;
  }
  free(remainder);
  return error;
}

/** When creating a new object, current is not allowed to exist. */
int check_for_no_current(int dirfd, char *path) {
  char *current;
  const char *tail = "/current.json";
  asprintf(&current, "%s%s", path, tail);
  char buff[255]; /* We are not going to use it anyway, so don't care if path is truncated */
  ssize_t length = readlinkat(dirfd, current, buff, sizeof(buff)-1);
  free(current);
  if (length == -1) {
    return 0;
  } else {
    return -1;
  }
}

/** Convert the filename into a revision number, or -1 if not possible.  */
int get_revision_from_filename(char *name)
{
  if(name[0] == '\0')
    /** Blank input (i.e. just the null terminator) */
    {
      return -1;
    }
  if (strchr(name, '.') == NULL)
    {
    /** No extension separator */
    return -1;
    }

  char *extension = name;
  char *endptr;
  strsep(&extension, ".");
  if (strcmp(extension, "json") != 0) {
    return -1;
  }
  errno = 0;
  long revision = strtol(name, &endptr, 10);

  if ((errno == ERANGE && (revision == LONG_MAX || revision == LONG_MIN))
      || (errno != 0 && revision == 0)) {
    /** Strtol function crapped out. */
    return -1;
  }
  if (endptr == name) {
    /** No numbers were found in filename */
    return -1;
  }
  if (*endptr != '\0') {
    /* Extra junk after number */
    return -1;
  }
  return revision;

}

/** Check for the highest revision number in a document directory */
int check_for_highest_revision(int dirfd, char *path) {
  int highest_revision = 0;
  int fd = openat(dirfd, path, O_DIRECTORY);
  DIR *current_dir = fdopendir(fd);
  struct dirent *result;

  /* Loop through the directory stream until empty */
  for (;;) {
    result = readdir(current_dir);
    if (result == NULL) {
      /* Got nothing left in stream */
      break;
    }
    char *filename = result->d_name;
    int current_revision = get_revision_from_filename(filename);
    if (current_revision > highest_revision) {
      highest_revision = current_revision;
    }
  }
  closedir(current_dir);
  return highest_revision;
}

int create_file(int dirfd,
                char *path,
                int revision,
                json_object *document,
                const char **error_message) {
  char *filename;
  char *linkpath;
  asprintf(&filename, "%s/%d.json", path, revision);
  asprintf(&linkpath, "%s/current.json", path);
  printf("Creating file %s\n", filename);
  /* TODO pass dirfd to json file creation and use openat() to help
      avoid race conditions */
  int result = json_object_to_file_ext(filename,
                                       document,
                                       JSON_C_TO_STRING_PRETTY|JSON_C_TO_STRING_SPACED|JSON_C_TO_STRING_NOZERO);
  if (result != 0) {
    /* TODO replace this with the real errno. */
    *error_message = "Could not save document. File error. Check permissions and configuration.";
    free(linkpath);
    return -32000;
  }
  int symresult = symlinkat(filename, dirfd, linkpath);
  if (symresult != 0) {
    int errsv = errno;
    char *message_buffer;
    message_buffer = malloc(BUFSIZ);
    *error_message = strerror_r(errsv, message_buffer, BUFSIZ);
    free(message_buffer);
    free(linkpath);
    return errsv;
  }
  free(linkpath);
  free(filename);
  return 0;
}

int create_new_file(char *path,
                    json_object *document,
                    const char **error_message) {
  int dirfd = AT_FDCWD;
  int error = create_document_dir(dirfd, path);
  /* All worked, make the file */
  if (error == 0) {
    int another = create_file(dirfd, path, 1, document, error_message);
    return another;
  }

  /* Missing parent dir(s), make them then make the file. */
  if (error == ENOENT) {
    printf("Making parents.\n");
    char *directory = strdup(path);
    int parents = make_parents(dirfd, directory);
    if (parents == 0) {
      free(directory);
      int another = create_file(dirfd, path, 1, document, error_message);
      return another;
    } else {
      error = parents;
    }
  }

  /* Dir already exists, look inside it */
  if (error == EEXIST) {
    printf("Directory already exists.\n");
    if (check_for_no_current(dirfd, path) == -1) {
      /* Crap out with duplicate key error */
      *error_message = "Duplicate Key Error on create method.";
      return -11000; /** MongoDB joke */
    }
    printf("Current.json does not exist.\n");
    int highest_revision = check_for_highest_revision(dirfd, path);
    printf("Highest revision was %d.\n", highest_revision);
    int another = create_file(dirfd, path, highest_revision+1, document, error_message);
    if (another != 0) {
      *error_message = "Error creating file.";
      return -1;
    }
    return 0;
  }

  /* Other problem, crap out. */
  char *message_buffer;
  message_buffer = malloc(BUFSIZ);
  *error_message = strerror_r(error, message_buffer, BUFSIZ);
  free(message_buffer);
  return error;
}


//int main() {
//  char *path = strdup("/home/zeth/c/store/dog/product/0ccc6671-c598-42b9-9761-a9e0776abc8ef");
//  int success = perform_create(path);
//  free(path);
//}
