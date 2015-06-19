#ifndef PFSTEST_OUTPUT_H
#define PFSTEST_OUTPUT_H

#include "pfstest-platform.h"

typedef struct 
{
    int passed;
    int failed;
    int ignored;
} _pfstest_results_t;

typedef struct 
{
    _pfstest_results_t results;
    int (*print_char)(int);
    bool verbose;
    volatile bool test_failed;
    bool test_ignored;
    bool fresh_line;
    const pfstest_nv_ptr char *test_name;
    const pfstest_nv_ptr char *test_file;
} pfstest_output_formatter_t;

void pfstest_output_formatter_standard_init(
    pfstest_output_formatter_t *formatter, int (*print_char)(int));
void pfstest_output_formatter_verbose_init(
    pfstest_output_formatter_t *formatter, int (*print_char)(int));

void pfstest_output_formatter_print_nv_string(
    pfstest_output_formatter_t *formatter, const pfstest_nv_ptr char *s);

void pfstest_output_formatter_run_started(
    pfstest_output_formatter_t *formatter);
void pfstest_output_formatter_test_started(
    pfstest_output_formatter_t *formatter,
    const pfstest_nv_ptr char *test_name,
    const pfstest_nv_ptr char *test_file);
void pfstest_output_formatter_test_ignored(
    pfstest_output_formatter_t *formatter);
void pfstest_output_formatter_test_failed_message_start(
    pfstest_output_formatter_t *formatter,
    const pfstest_nv_ptr char *file, int line,
    /* FIXME: Hack for old core */
    bool fail_expected);
void pfstest_output_formatter_test_failed_message_complete(
    pfstest_output_formatter_t *formatter);
void pfstest_output_formatter_test_complete(
    pfstest_output_formatter_t *formatter);
void pfstest_output_formatter_run_complete(
    pfstest_output_formatter_t *formatter);

int pfstest_output_formatter_return_value(
    pfstest_output_formatter_t *formatter);

#endif /* !PFSTEST_OUTPUT_H */
