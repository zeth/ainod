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

#ifndef H_FUNCTIONS_H_   /* Include guard */
#define H_FUNCTIONS_H_

typedef int (*data_handler) (char *data);
typedef int (*entry_handler) (ENTRY *entry);

/** hiterdata_r goes through all the data values and runs the
    callback handler on each.

    For example, imagine the data values are all pointers, then one
    can use this callback as a tidy up function:

    int free_it_all(char *data) {
      free(data);
      return 0;
    } 

*/
int hiter_data_r(struct hsearch_data *htab, data_handler cmb);

/** hiter_items_r goes through all the keys and values and runs the
    callback handler on each.

    For example, imagine the key and data values are all pointers,
    then one can use this callback as a tidy up function:

    int free_it_all(ENTRY *entry) {
      free(entry->key);
      free(entry->data);
    return 0;
    } 

*/
int hiter_items_r(struct hsearch_data *htab, entry_handler cmb);

#endif // H_FUNCTIONS_H_
