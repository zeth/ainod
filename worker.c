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

#include <string.h>


#define BACKLOG 10
#define MAX_EPOLL_FD 4096
#define MAXEVENTS 64
#define MAX_BUF 1000

static void setnonblocking(int fd)
{
  int opts;
  opts = fcntl(fd, F_GETFL);
  if (opts < 0) {
    handle_error("fcntl failed\n");
    return;
  }
  opts = opts | O_NONBLOCK;
  if (fcntl(fd, F_SETFL, opts) < 0) {
    handle_error("fcntl failed\n");
    return;
  }
  return;
}

/* child_worker is the program that the children each run.

   The child process learns its own worker number through the worker
   argument.
*/

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

  /*Should the event object be on the heap? */
  struct epoll_event event;
  event.data.fd = incoming;
  event.events = EPOLLIN | EPOLLET;
  if (epoll_ctl (epoll_fd, EPOLL_CTL_ADD, incoming, &event) == -1) {
    handle_error("Could not add socket to epoll.");
  }

  struct epoll_event *events;
  events = calloc (MAXEVENTS, sizeof event);
  while (1) {
    int number_of_events = epoll_wait (epoll_fd, events, MAXEVENTS, -1);
    /* printf("CHILD %d, try to get mutex\n", worker); */
    int first = lock_mutex(mp);
    if (first == 0) {
      /* printf("CHILD %d, I won the mutex! %d events \n", worker, number_of_events); */
      int i;
      for (i = 0; i < number_of_events; i++) {
        /* What to do here now ? */
        if (events[i].events & EPOLLIN) {
          /* Connection file descriptor */
          int cfd;
          /* Accept the connection */
          cfd = accept(events[i].data.fd, NULL, NULL);
          if (cfd == -1) {
            handle_error("Error in accepting connection");
          }
          char buf[100];
          ssize_t numRead;
          char *response = "{Result: OK}\n";
          int response_success = send(cfd,
                                      response,
                                      13,
                                      MSG_DONTWAIT);
          if (response_success == -1) {
            handle_error("Could not send to client socket.");
          }
          /* It is the while loop here that sucks. */
          while ((numRead = read(cfd, buf, 1000)) > 0) {
            if (write(STDOUT_FILENO, buf, numRead) != numRead) {
              handle_error("partial/failed write");
            }
          }
          close(cfd);
        } else {
          printf("Some other kind of event.");
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
