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
#include "jsonrpc.h"

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
                  char *req_id_format,
                  bool req_req_id) {
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
    /* Try to get the mutex */
    int first = lock_mutex(mp);
    if (first == 0) {
      /* We won the mutex!  */
      /* Make arrays to hold the connections and the character buffers */
      int connection[number_of_events];
      char *buffer_array[number_of_events];
      int i;
      for (i = 0; i < number_of_events; i++) {
        /* What to do here now ? */
        if (events[i].events & EPOLLIN) {
          /* Accept the connection */
          connection[i] = accept(events[i].data.fd, NULL, NULL);
          if (connection[i] == -1) {
            handle_error("Error in accepting connection.");
          }
          if ((buffer_array[i] = malloc(PAGE_SIZE)) == NULL) {
            handle_error("Error in allocating memory.");
          }
          ssize_t length_read = read(connection[i], buffer_array[i], PAGE_SIZE);
          int current_length = PAGE_SIZE;
          while (length_read == PAGE_SIZE) {
            buffer_array[i] = realloc(buffer_array[i], current_length + PAGE_SIZE);
            length_read = long_read(&connection[i], buffer_array[i], current_length);
            current_length += PAGE_SIZE;
          }
          printf("End length. %d\n", current_length);



        } else {
          printf("Some other kind of event.");
        }
      }
      /** Hand back the Mutex to let the other workers take incoming connections. */
      int second = unlock_mutex(mp);

      /** We had read all the data from the incoming socket, now process it */
      /** Possibly want to hand this back to epoll */
      for (i = 0; i < number_of_events; i++) {
        printf("We have got %s.\n", buffer_array[i]);
          const char *response = process_buffer(buffer_array[i],
                                                silentnote,
                                                req_id_format,
                                                req_req_id);
          if (response == 0 && silentnote == true) {
            /* We have a notification, don't send reply */
            printf("I am a notification");
          } else {
            /* We have a normal response */
            int response_success = send(connection[i],
                                        response,
                                        strlen(response),
                                        MSG_DONTWAIT);
            if (response_success == -1) {
              handle_error("Could not send to client socket.");
            }
          }
          free(buffer_array[i]);
          close(connection[i]);
      }
    } else {
      /** printf("CHILD %d, I lost the mutex!\n", worker, first); */
    }

  }

  close(epoll_fd);
  free(events);

  free(datadir);
  exit(0);
}
