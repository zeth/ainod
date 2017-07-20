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


/* src/handleerror.c */
/* handle_error */

//* Mock do_exit function */
void do_exit()
{
  // This function should do nothing
  // (Instead of exiting)
};

//* Mock do_printf function */
void do_printf(char *formatted_message)
{
  ck_assert_str_eq("There are 33 Monkeys.", formatted_message);
};

START_TEST(test_handle_error)
{
  handle_error("There are %d Monkeys.", 33);
}
END_TEST

TCase *make_handle_error_test_case(void)
{
  TCase *test_case;
  test_case = tcase_create("HandleError");
  tcase_add_test(test_case, test_handle_error);
  return test_case;
}
