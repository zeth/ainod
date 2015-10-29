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

int string_is_in_array(char *string, char *array[], int length);

static void test_string_is_in_array(void)
{
  char *sections[] = {"Daemon", "Other"};
  int answer = string_is_in_array("Other", sections, 2);
  int wronganswer = string_is_in_array("Angel", sections, 2);
  NP_ASSERT_EQUAL(answer, 0);
  NP_ASSERT_EQUAL(wronganswer, -1);
}
