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

int new_mutex(ainod_mutex *mtx) {
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

int delete_mutex(ainod_mutex *mtx) {
  if (pthread_mutex_destroy(mtx) != 0) {
    handle_error("Failed to delete mutex.");
  }
  return 0;
}

int lock_mutexb(ainod_mutex *mtx) {
  if (pthread_mutex_lock(mtx) == 0) {
    return 0;
  } else {
    return -1;
  }
}

int unlock_mutex(ainod_mutex *mtx) {
  if (pthread_mutex_unlock(mtx) == 0) {
    return 0;
  } else {
    return -1;
  }
}
