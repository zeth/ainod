#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <check.h>
#include "../src/daemon/configparser.c"



/** Test Suite **/
Suite * configparser_suite(void)
{
    Suite *test_suite;
    test_suite = suite_create("ConfigParser");
    //suite_add_tcase(test_suite, create_new_file_test_case());
    return test_suite;
}
