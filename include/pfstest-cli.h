#ifndef PFSTEST_CLI_H
#define PFSTEST_CLI_H

/** @file */

#include "pfstest-list.h"
#include "pfstest-platform.h"

typedef struct 
{
    char *program_name;
    char *filter_file;
    char *filter_name;
    pfstest_bool xml;
    pfstest_bool verbose;
    pfstest_bool color;
    pfstest_bool print_register_commands;
} pfstest_arguments_t;

pfstest_bool pfstest_arguments_parse(pfstest_arguments_t *args,
                                     int argc, char *argv[]);
void pfstest_print_register_commands(int (*print_char)(int),
                                     pfstest_list_t *plugins,
                                     pfstest_list_t *suite);
void pfstest_print_usage(int (*print_char)(int), char *program_name);

/**
 Command-line driver to start the PFSTest harness and cause all
 registered tests to run.

 Call from main() after registering tests with register_test(),
 registering plugins with register_plugin(), and doing any other
 project-specific initialization.

 With no command line arguments, creates a standard reporter and calls
 pfstest_run_registered_tests(). Command line arguments can modify
 output format or request an operation other than running tests.
*/
int pfstest_main(int argc, char *argv[]);

#endif /* !defined(PFSTEST_CLI_H) */
