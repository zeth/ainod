#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#include "handleerror.h"
#include "worker.h"
#include "mutex.h"

#define BACKLOG 10
#define MAX_EPOLL_FD 4096
#define MAXEVENTS 64
#define PAGE_SIZE 4096

int long_read(int *cfd, char *buf) {
  printf("Long Read\n");
  buf = realloc(buf, 8192);
  ssize_t length_read = read(*cfd, buf + PAGE_SIZE, PAGE_SIZE);
  printf("We read %d from the buffer.\n", buf[4196]);
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
          char *buf = malloc(PAGE_SIZE);
          ssize_t length_read = read(cfd, buf, PAGE_SIZE);
          //printf("We read %d from the buffer.\n", length_read);
          if (length_read == PAGE_SIZE) {
            long_read(&cfd, buf);
          }

          char *response = "{result: OK}\n";
          int response_success = send(cfd,
                                      response,
                                      13,
                                      MSG_DONTWAIT);
          if (response_success == -1) {
            handle_error("Could not send to client socket.");
          }
          close(cfd);
          free(buf);
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
