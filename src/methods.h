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

#ifndef AINOD_METHODS_H
#define AINOD_METHODS_H

char *get_id(void);

int get(json_object *params,
        json_object **data,
        const char **error_message,
        int path_format,
        char *datadir);

int create(json_object *params,
        json_object **data,
        const char **error_message,
        int path_format,
        char *datadir);

int delete(void);

int update(void);

int replace(void);

int reindex(void);

int save(void);


#endif /* AINOD_METHODS_H */
