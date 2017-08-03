/***
  This file is part of ainod.

  Copyright 2017 Zeth

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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <check.h>

#include "../../src/handleerror.c"

/** Test Cases */
#include "handle_error.c"

/** Test Suite **/
Suite * handlerror_suite(void)
{
    Suite *test_suite;
    test_suite = suite_create("HandleError");
    suite_add_tcase(test_suite, make_handle_error_test_case());
    return test_suite;
}
