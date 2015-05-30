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

/* Mutex support for ainod. */
#include <pthread.h>
#include "handleerror.h"
#include "mutex.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MUTEX "/ainod_mutex_lock"


pthread_mutex_t *setup_mutex(void) {
  pthread_mutex_t *mutex;
  int shared_mem;
  int mode = S_IRWXU | S_IRWXG;
  pthread_mutexattr_t mutex_attr;

  shared_mem = shm_open(MUTEX, O_CREAT | O_RDWR | O_TRUNC, mode);

  if (shared_mem < 0) {
    handle_error("Failed to create shared memory.");
  }
  if (ftruncate(shared_mem, sizeof(pthread_mutex_t)) == -1) {
    handle_error("Failed to set correct size of shared memory space.");
  }
  mutex = (pthread_mutex_t*) mmap(NULL, sizeof(pthread_mutex_t),
                                  PROT_READ | PROT_WRITE, MAP_SHARED, shared_mem, 0);
  if (mutex == MAP_FAILED ) {
    handle_error("Failed to map the mutex into shared memory.");
  }
  if (pthread_mutexattr_init(&mutex_attr) != 0) {
    handle_error("Failed to create mutex attribute");
  }
  if (pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED) != 0) {
    handle_error("Failed to share mutex.");
  }
  if (pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_ERRORCHECK) != 0) {
    handle_error("Failed to set mutex type.");
  }
  if (pthread_mutex_init(mutex, &mutex_attr) != 0) {
    handle_error("Failed to initialise mutex.");
  }
  if (pthread_mutexattr_destroy(&mutex_attr) != 0) {
    handle_error("Failed to delete mutex attribute.");
  }
  return mutex;
}


int delete_mutex(pthread_mutex_t *mutex) {
  pthread_mutex_destroy(mutex);
  shm_unlink(MUTEX);
  return 0;
}


int new_mutex(pthread_mutex_t *mtx) {
  pthread_mutexattr_t attr;
  if (pthread_mutexattr_init(&attr) != 0) {
    handle_error("Failed to create mutex attribute");
  }
  if (pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK) != 0) {
    handle_error("Failed to set mutex type");
  }
  if (pthread_mutex_init(mtx, &attr) != 0) {
    handle_error("Failed to initialise mutex.");
  }
  if (pthread_mutexattr_destroy(&attr) != 0) {
    handle_error("Failed to delete mutex attribute.");
  }
  return 0;
}

int delete_mutexb(pthread_mutex_t *mtx) {
  if (pthread_mutex_destroy(mtx) != 0) {
    handle_error("Failed to delete mutex.");
  }
  return 0;
}

int lock_mutex(pthread_mutex_t *mtx) {
  if (pthread_mutex_lock(mtx) == 0) {
    return 0;
  } else {
    return -1;
  }
}

int unlock_mutex(pthread_mutex_t *mtx) {
  if (pthread_mutex_unlock(mtx) == 0) {
    return 0;
  } else {
    return -1;
  }
}
