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

/* Helpers for use of Json-c */

#ifndef AINOD_JSONHELPERS_H
#define AINOD_JSONHELPERS_H


int get_string(struct json_object *params,
               const char *key,
               const char **string);

int get_object_from_filename(char *filename,
                             struct json_object **data,
                             const char **error_message);


#endif /* AINOD_JSONHELPERS_H */
