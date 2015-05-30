#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "worker.h"
#include "mutex.h"
#include <unistd.h>


/* child_worker is the program that the children each run.

   The child process learns its own worker number through the worker
   argument.
*/

void child_worker(int worker,
                  pthread_mutex_t *mp,
                  char *datadir) {
  printf("CHILD %d, try to get mutex\n", worker);
  int first = lock_mutex(mp);
  printf("CHILD %d lock gives %d.\n", worker, first);
  printf("CHILD %d has lock.\n", worker);
  sleep(2);
  int second = unlock_mutex(mp);
  printf("CHILD %d unlock gives %d.\n", worker, first);
  printf("CHILD %d : Exiting.\n", worker);
  free(datadir);
  exit(0);
}
