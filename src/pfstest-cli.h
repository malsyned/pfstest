#ifndef PFSTEST_CLI_H
#define PFSTEST_CLI_H

#include "pfstest-list.h"
#include "pfstest-platform.h"

typedef struct 
{
    char *program_name;
    char *filter_file;
    char *filter_name;
    bool xml;
    bool verbose;
    bool color;
    bool print_register_commands;
} pfstest_arguments_t;

bool pfstest_arguments_parse(pfstest_arguments_t *args,
                             int argc, char *argv[]);
void pfstest_print_register_commands(int (*print_char)(int),
                                     pfstest_list_t *before,
                                     pfstest_list_t *after,
                                     pfstest_list_t *plugins,
                                     pfstest_list_t *suite);
void pfstest_print_usage(int (*print_char)(int), char *program_name);
int pfstest_main(int argc, char *argv[]);

#endif /* !defined(PFSTEST_CLI_H) */
