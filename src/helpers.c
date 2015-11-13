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

/**

0. Try to create the directory.

mkdirat

Success = 0 --> go to 4.
Error = -1

errno:
ENOENT - parent does not exist --> goto 1.

EEXIST - pathname already exists --> goto 2.

All other errors - give up.

1. Make all parents. --> Goto 0.

2. Try to find current.json

readlinkat

ENOENT - link does not exist. --> goto 3

Link exists, throw duplicatekey error

3. Look for other files in directory, find the one with highest number in name
-->goto 5 with arg of number+1

4. --> Goto 5 with arg of 1.

5. Create {n}.json, fill it with the data and make current.json symlink

**/

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
  int errsv;
  int success = mkdirat(dirfd, path, S_IRWXU|S_IRWXG|S_IROTH);
  if (success == 0) {
    printf("Happy\n");
    return 0;
  } else {
    errsv = errno;
    return errsv;
  }
};

/** Go through the whole path and make sure each directory exists **/
int make_parents(int dirfd, char *directory) {
  char *remainder = directory;
  int fd;
  while (*remainder == '/')
    remainder++; /* Gets rid of leading slash */
  for (;;) {
    strsep(&remainder, "/");
    if (remainder == NULL) {
      /* We are finished */
      break;
      }
    while (*remainder == '/')
      remainder++;
    if ((fd = mkdirat(dirfd, directory, S_IRWXU|S_IRWXG|S_IROTH)) < 0 && errno != EEXIST) {
      printf("Problem! %d\n", errno);
      break;
    }
    if ((fd = openat(dirfd, directory, O_DIRECTORY)) < 0) {
      printf("Problem! %d\n", errno);
      break;
    }
    if (dirfd != AT_FDCWD) {
      close(dirfd);
    }
    dirfd = fd;
    directory = remainder;
  }
  free(remainder);
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
  int current_revision;
  int fd = openat(dirfd, path, O_DIRECTORY);
  DIR *current_dir = fdopendir(fd);
  struct dirent entry;
  struct dirent *result;

  /* Loop through the directory stream until empty */
  for (;;) {
    int error = readdir_r(current_dir, &entry, &result);
    if (result == NULL) {
      /* Got nothing left in stream */
      break;
    }
    char *filename = result->d_name;
    current_revision = get_revision_from_filename(filename);
    if (current_revision > highest_revision) {
      highest_revision = current_revision;
    }
  }
  closedir(current_dir);
  return highest_revision;
}

int create_file(int dirfd, char *path, int revision, json_object *document) {
  char *filename;
  char *linkpath;
  asprintf(&filename, "%s/%d.json", path, revision);
  asprintf(&linkpath, "%s/current.json", path);
  printf("Creating file %s\n", filename);
  /** TODO pass dirfd to json file creation and use openat() to help
      avoid race conditions */
  int result = json_object_to_file_ext(filename,
                                       document,
                                       JSON_C_TO_STRING_PRETTY|JSON_C_TO_STRING_SPACED|JSON_C_TO_STRING_NOZERO);
  if (result != 0) {
    printf("Houston we have a problem\n");
  }
  int symresult = symlinkat(filename, dirfd, linkpath);
  if (symresult != 0) {
    printf("Houston we still have a problem\n");
  }
  free(linkpath);
  free(filename);
  return result;
}

int create_new_file(char *path, json_object *document) {
  int dirfd = AT_FDCWD;
  int error = create_document_dir(dirfd, path);
  /* All worked, make the file */
  if (error == 0) {
    int another = create_file(dirfd, path, 1, document);
    return 0;
  }

  /* Missing parent dir(s), make them then make the file. */
  if (error == ENOENT) {
    printf("Making parents.\n");
    char *directory = strdup(path);
    make_parents(dirfd, directory);
    free(directory);
    int another = create_file(dirfd, path, 1, document);
    return 0;
  }

  /* Dir already exists, look inside it */
  if (error == EEXIST) {
    printf("Directory already exists.\n");
    if (check_for_no_current(dirfd, path) == -1) {
      /* Crap out with duplicate key error */
      printf("Duplicate key error!!!!.\n");
      return -200;
    }
    printf("Current.json does not exist.\n");
    int highest_revision = check_for_highest_revision(dirfd, path);
    printf("Highest revision was %d.\n", highest_revision);
    int another = create_file(dirfd, path, highest_revision+1, document);
    return 0;
  }

  printf("Other problem, crap out.\n");
  return error;
}


//int main() {
//  char *path = strdup("/home/zeth/c/store/dog/product/0ccc6671-c598-42b9-9761-a9e0776abc8ef");
//  int success = perform_create(path);
//  free(path);
//}
