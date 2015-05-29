#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "worker.h"

/* child_worker is the program that the children each run.

   The child process learns its own worker number through the worker
   argument.
*/

int lock_mutex(pthread_mutex_t *mp) {
  int result;
  result = pthread_mutex_trylock (mp);
  return result;
}

void child_worker(int worker,
                  pthread_mutex_t *mp) {
  printf("CHILD %d : Exiting.\n", worker);
  int first = lock_mutex(mp);
  printf("First %d lock gives %d.\n", worker, first);
  int second = lock_mutex(mp);
  printf("Second %d lock gives %d.\n", worker, second);
  exit(0);
}
