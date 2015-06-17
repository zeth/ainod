#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#include "handleerror.h"
#include "worker.h"
#include "mutex.h"
#include "methods.h"

#define BACKLOG 10
#define MAX_EPOLL_FD 4096
#define MAXEVENTS 64
#define PAGE_SIZE 4096

int long_read(int *cfd, char *buf, int current_length) {
  ssize_t length_read = recv(*cfd,
                             buf + current_length,
                             PAGE_SIZE,
                             MSG_DONTWAIT);
  return length_read;
}


/* child_worker is the program that the children each run.

   The child process learns its own worker number through the worker
   argument.
*/

void child_worker(int worker,
                  pthread_mutex_t *mp,
                  char *datadir,
                  int incoming,
                  bool silentnote,
                  char *req_id_format) {
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
          int current_length = PAGE_SIZE;
          while (length_read == PAGE_SIZE) {
            buf = realloc(buf, current_length + PAGE_SIZE);
            length_read = long_read(&cfd, buf, current_length);
            current_length += PAGE_SIZE;
          }
          printf("End length. %d\n", current_length);

          /* Buffer processing needs to be moved to after the mutex is
             released, possibly using epoll to help. */
          const char *response = process_buffer(buf,
                                                silentnote,
                                                req_id_format);
          int response_success = send(cfd,
                                      response,
                                      strlen(response),
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
