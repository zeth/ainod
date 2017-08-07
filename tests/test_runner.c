#define _GNU_SOURCE
#include <stdlib.h>
#include <check.h>

#include "helpers/suite_helpers.c"
#include "configparser/suite_configparser.c"
#include "handlerror/suite_handleerror.c"
#include "filter/suite_filter.c"
#include "jsonrpc/suite_jsonrpc.c"
#include "methods/suite_methods.c"

/** Default suite **/
Suite *make_master_suite (void)
{
  Suite *suite;

  suite = suite_create("Master");
  return suite;
}


/** Test Runner **/
int main(void)
{
  int number_failed;
  SRunner *srunner;

  srunner = srunner_create(make_master_suite());
  srunner_add_suite(srunner, helpers_suite());
  srunner_add_suite(srunner, configparser_suite());
  srunner_add_suite(srunner, handlerror_suite());
  srunner_add_suite(srunner, filter_suite());
  srunner_add_suite(srunner, jsonrpc_suite());
  srunner_add_suite(srunner, methods_suite());

  srunner_run_all(srunner, CK_NORMAL);
  number_failed = srunner_ntests_failed(srunner);
  srunner_free(srunner);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
