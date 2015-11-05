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

#ifndef AINOD_FILTER_H
#define AINOD_FILTER_H

#include <json-c/json.h>

#define AINOD_MISSING_COLLECTION "Missing collection parameter"\
  " (as required when the Path-format setting is set to 1)."

#define AINOD_MISSING_STORE "Missing store parameter"\
  " (as required when the Path-format setting is set to 0 or 2)."

char *get_id(void);

int get_path_from_filter(json_object **filter,
                         char **path,
                         int path_format,
                         const char **error_message,
                         char **datadir,
                         int create);


#endif /* AINOD_FILTER_H */
