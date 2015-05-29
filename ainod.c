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

#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <search.h>
#include <pthread.h>
#include <unistd.h>
#include "configparser.h"
#include "asocket.h"
#include "handleerror.h"

int parent(void) {
  /* Make a Hash Table for config information. */
  struct hsearch_data *store = new_store();

  /* Fill it from the config manager */
  parse_config(store, "ainod.conf");

  /* Get out the relevant settings */
  int number_of_workers = check_workers(store);
  char *datadir = check_data_dir(store);

  /* Get the incoming socket */
  int incoming = get_socket();

  /* Get a mutex for it */
  pthread_mutex_t mp;
  pthread_mutexattr_t attr;
  int i = pthread_mutexattr_init(&attr);
  int j = pthread_mutexattr_setpshared (&attr, PTHREAD_PROCESS_SHARED);
  int k = pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST);
  int l = pthread_mutex_init(&mp, &attr);
  if (!((i == 0) && (j == 0) && (k == 0) && (l == 0))) {
    handle_error("Error making mutex.");
  }

  /* Bin the config information */
  delete_store(store);

  /* Bin the mutex */
  pthread_mutex_destroy (&mp);
}


int main(){
  parent();
  printf("Hello World.\n");
}
