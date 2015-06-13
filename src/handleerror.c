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
#include <unistd.h>
#include <stdarg.h>
#include <systemd/sd-journal.h>
#include "handleerror.h"

/* Implement the next line properly later */
#define IS_A_DAEMON 0

/** `message` is the error message, with optional format
    arguments. **/

int handle_error(char *message, ...) {
  // Start by formatting the message with any optional args
  char *formatted_message = NULL;

  va_list ap;
  va_start (ap, message);
  vasprintf(&formatted_message, message, ap);
  va_end (ap);

  /** In daemon mode log to the journal */
  if (IS_A_DAEMON) {
    sd_journal_send("MESSAGE=%s", formatted_message,
                    "PRIORITY=5",
                    "HOME=%s", getenv("HOME"),
                    "TERM=%s", getenv("TERM"),
                    "PAGE_SIZE=%li", sysconf(_SC_PAGESIZE),
                    "N_CPUS=%li", sysconf(_SC_NPROCESSORS_ONLN),
                    NULL);
  } else {
    /** In terminal, log to the terminal */
    printf("Error: %s\n", formatted_message);
  }
  free(formatted_message);
  exit (EXIT_FAILURE);
  return 0;
}

//int main(int argc, char *argv[]) {
//  handle_error("There are %d Monkeys.", 33);
//  return 0;
//}
