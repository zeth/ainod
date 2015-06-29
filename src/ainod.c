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
#include <string.h>
#include <sys/wait.h>
#include "configparser.h"
#include "asocket.h"
#include "handleerror.h"
#include "worker.h"
#include "mutex.h"

void sig_handler(int signo)
{
  if (signo == SIGINT)
    printf("received SIGINT\n");
  exit(0);
}


int parent(void) {
  /* Setup signal */
  if (signal(SIGINT, sig_handler) == SIG_ERR) {
    printf("\ncan't catch SIGINT\n");
  }

  /* Make a Hash Table for config information. */
  struct hsearch_data *store = new_store();

  /* Fill it from the config manager */
  parse_config(store, "ainod.conf");

  /* Get out the relevant settings */
  int number_of_workers = check_workers(store);
  char *datadir = strdup(check_data_dir(store));
  bool silentnote = check_boolean_setting(store, "Silent-notifications");
  bool req_prot = check_boolean_setting(store, "Require-protocol-version");
  bool req_req_id = check_boolean_setting(store, "Require-request-id");
  bool remote_schemas = check_boolean_setting(store, "Remote-schemas");
  bool eof_marker = check_boolean_setting(store, "EOF-marker");
  char *req_id_format = strdup(check_string_setting(store,
                                                    "Req-id-format",
                                                    "default"));
  int path_format = check_int_setting(store,
                                      "Path-format");
  /* Bin the config information */
  delete_store(store);

  /* Get the incoming socket */
  int incoming = get_socket();

  /* Get the mutex */
  pthread_mutex_t *mtx = setup_mutex();
  int i;
  /* Create child worker processes */
  for(i=0;i<number_of_workers;i++){
    if (!fork()) {
      child_worker(i, mtx, datadir,
                   incoming, silentnote,
                   req_id_format, req_req_id,
                   path_format);
    }
  }
  /* Cleanup below */

  /* Make sure children are all finished. */
  for(i=0;i<number_of_workers;i++){
    (void) wait(NULL);
  }

  /* Bin the config information */
  //delete_store(store);
  free(datadir);
  /* Bin the mutex */
  delete_mutex(mtx);
}


int main(){
  parent();
  printf("Hello World.\n");
}
