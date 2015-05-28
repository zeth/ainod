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

#include <stdio.h>
#include <search.h>
#include "configparser.h"
#include "asocket.h"

int parent(void) {
  struct hsearch_data *store = new_store();
  parse_config(store, "ainod.conf");
  search_store(store, "Workers");
  search_store(store, "Datadir");
  int incoming = get_socket();
  delete_store(store);
}


int main(){
  parent();
  printf("Hello World.\n");
}
