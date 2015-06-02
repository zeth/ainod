#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <errno.h>
#include <netdb.h>
#include <fcntl.h>

#include "handleerror.h"
#include "worker.h"
#include "mutex.h"

#define BACKLOG 10
#define MAX_EPOLL_FD 4096
#define MAXEVENTS 64
#define MAX_BUF 1000
char buf[MAX_BUF];

/* child_worker is the program that the children each run.

   The child process learns its own worker number through the worker
   argument.
*/

static int make_socket_non_blocking (int sfd)
{
  int flags, s;

  flags = fcntl (sfd, F_GETFL, 0);
  if (flags == -1)
    {
      perror ("fcntl");
      return -1;
    }

  flags |= O_NONBLOCK;
  s = fcntl (sfd, F_SETFL, flags);
  if (s == -1)
    {
      handle_error("fcntl");
      return -1;
    }

  return 0;
}


void child_worker(int worker,
                  pthread_mutex_t *mp,
                  char *datadir,
                  int incoming) {
  /** Listen to socket */

  if (listen(incoming, BACKLOG) == -1) {
    handle_error("Worker could not listen on socket");
  }
  int epoll_fd;
  /** Create epoll instance */
  epoll_fd = epoll_create(MAX_EPOLL_FD);
  if (epoll_fd == -1) {
    handle_error("Worker could not create epoll.");
  }

  struct epoll_event event;
  event.data.fd = incoming;
  event.events = EPOLLIN | EPOLLET;
  if (epoll_ctl (epoll_fd, EPOLL_CTL_ADD, incoming, &event) == -1) {
    handle_error("Could add socket to epoll.");
  }

  struct epoll_event *events;
  events = calloc (MAXEVENTS, sizeof event);

  while (1) {
    int number_of_events = epoll_wait (epoll_fd, events, MAXEVENTS, -1);
    printf("CHILD %d, try to get mutex\n", worker);
    int first = lock_mutex(mp);
    if (first == 0) {
      printf("CHILD %d, I won the mutex! %d events \n", worker, number_of_events);
      int i;
      for (i = 0; i < number_of_events; i++) {
        /* What to do here now ? */
        if (events[i].events & EPOLLIN) {
          int s = read(events[i].data.fd, buf, MAX_BUF);
          if (s == -1)
            handle_error("read");
          printf("    read %d bytes: %.*s\n", s, s, buf);
        }
      }
      int second = unlock_mutex(mp);

    } else {
      /** printf("CHILD %d, I lost the mutex!\n", worker, first); */
    }
  }

  close(epoll_fd);
  free(events);

  free(datadir);
  exit(0);
}
