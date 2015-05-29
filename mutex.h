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

#ifndef AINOD_MUTEX_H_   /* Include guard */
#define AINOD_MUTEX_H_

#include <pthread.h>
typedef pthread_mutex_t ainod_mutex;

int new_mutex(ainod_mutex *mtx);

int delete_mutex(ainod_mutex *mtx);

int lock_mutex(ainod_mutex *mtx);

int unlock_mutex(ainod_mutex *mtx);

#endif // AINOD_MUTEX_H_
