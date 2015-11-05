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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <systemd/sd-daemon.h>

#include "asocket.h"

char *ABSTRACT_SOCKET_NAME = "ainod";

int get_socket()
{
  int fd, n;
  size_t name_length;
  socklen_t new_length;
  name_length = strlen(ABSTRACT_SOCKET_NAME) + 1;

  /* Check for file descriptors passed by the system manager */
  n = sd_listen_fds(0);

  if (n < 0)  {
    /* Below 0 is an error code */
    fprintf(stderr, "sd_listen_fds(): %s\n", strerror(-n));
    exit(EXIT_FAILURE);
  } else if (n > 1) {
    /* Only need one socket */
    fprintf(stderr, "Too many file descriptors received.\n");
    exit(EXIT_FAILURE);
  } else if (n == 1) {
    /* Got one socket */
    fd = SD_LISTEN_FDS_START + 0;
    /* Check it is correct type */
    if (sd_is_socket_unix(fd, SOCK_STREAM, -1, "\0", name_length) <= 0) {
      fprintf(stderr, "Invalid socket passed.\n");
      exit(EXIT_FAILURE);
    }
  } else {
    /* No socket from system manager. Lets make our own then */

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));  /* Clear address structure */
    addr.sun_family = AF_UNIX;                     /* UNIX domain address */

    /* addr.sun_path[0] has already been set to 0 by memset() */

    strncpy(&addr.sun_path[1], ABSTRACT_SOCKET_NAME, strlen(ABSTRACT_SOCKET_NAME));

    fd = socket(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (fd == -1) {
      printf("socket error");
      exit(EXIT_FAILURE);
    }

    new_length = (socklen_t) sizeof(sa_family_t) + (socklen_t) name_length;
    if (bind(fd, (struct sockaddr *) &addr, new_length) == -1) {
      printf("socket binding error");
      exit(EXIT_FAILURE);
    }
  }
  return fd;
}
