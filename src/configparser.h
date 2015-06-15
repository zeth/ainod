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

#ifndef AINOD_CONFIG_PARSER_H_   /* Include guard */
#define AINOD_CONFIG_PARSER_H_

#include <search.h>
#include <stdbool.h>

#define CONFIG_STORE_MAX 20

#define NEWLINE    "\n\r"
#define WHITESPACE " \t\n\r"
#define COMMENTS   "#;"

struct hsearch_data *new_store(void);

int delete_store(struct hsearch_data *store);

int parse_config(struct hsearch_data *store,
                 const char *filename);

char *search_store(struct hsearch_data *store,
                   char *key);

int check_workers(struct hsearch_data *store);

char *check_data_dir(struct hsearch_data *store);

bool check_boolean_setting(struct hsearch_data *store,
                            char *setting_name);

#endif // AINOD_CONFIG_PARSER_H_

