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

#include <search.h>
#include "hfunctions.h"

typedef struct _ENTRY
{
  unsigned int used;
  ENTRY entry;
}
  _ENTRY;

int hiter_data_r(struct hsearch_data *htab, data_handler cmb) {
  int length = htab->size;
  struct _ENTRY *j;
  j = htab->table;
  int i;
  for (i = 0; i < length; ++i) {
    struct _ENTRY k;
    k = j[i];
    if (k.used) {
      ENTRY l;
      l = k.entry;
      cmb(l.data);
    }
  }
  return 0;
}

int hiter_items_r(struct hsearch_data *htab, entry_handler cmb) {
  int length = htab->size;
  struct _ENTRY *j;
  j = htab->table;
  int i;
  for (i = 0; i < length; ++i) {
    struct _ENTRY k;
    k = j[i];
    if (k.used) {
      ENTRY l;
      l = k.entry;
      ENTRY *k;
      k = &l;
      cmb(k);
    }
  }
  return 0;
}
