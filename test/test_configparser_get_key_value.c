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

#include <np.h>	    /* NovaProva library */
#include <string.h>
#include <stdlib.h>

int get_key_value(char *str, char **key, char **value);

static void test_get_key_value(void)
{
  char *line = "Workers=4";
  char *key;
  char *value;
  get_key_value(line, &key, &value);
  NP_ASSERT_STR_EQUAL(key, "Workers");
  NP_ASSERT_STR_EQUAL(value, "4");
  free(key);
  free(value);
}
