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

START_TEST(test_get_id)
{
  char *newid = get_id();
  int newid_length = strlen(newid);
  free(newid);
  ck_assert_int_eq(newid_length, 36);
}
END_TEST


TCase *make_get_id_test_case(void)
{
  TCase *test_case;
  test_case = tcase_create("GetId");
  tcase_add_test(test_case, test_get_id);
  return test_case;
}
