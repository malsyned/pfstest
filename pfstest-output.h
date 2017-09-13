#ifndef PFSTEST_OUTPUT_H
#define PFSTEST_OUTPUT_H

#include "pfstest-platform.h"

typedef
struct _pfstest_output_formatter_vtable_t
pfstest_output_formatter_vtable_t;

typedef struct 
{
    const pfstest_nv_ptr pfstest_output_formatter_vtable_t *vtable;
    int (*char_writer)(int);
} pfstest_output_formatter_t;

struct _pfstest_output_formatter_vtable_t
{
    int (*print_char)(pfstest_output_formatter_t *formatter, int c);
    void (*run_started)(pfstest_output_formatter_t *formatter);
    void (*test_started)(pfstest_output_formatter_t *formatter,
                         const pfstest_nv_ptr char *test_name,
                         const pfstest_nv_ptr char *test_file);
    void (*test_ignored)(pfstest_output_formatter_t *formatter);
    void (*test_failed_message_start)(pfstest_output_formatter_t *formatter,
                                      const pfstest_nv_ptr char *file,
                                      int line);
    void (*test_failed_message_complete)(
        pfstest_output_formatter_t *formatter);
    void (*test_complete)(pfstest_output_formatter_t *formatter);
    void (*run_complete)(pfstest_output_formatter_t *formatter);
    int (*return_value)(pfstest_output_formatter_t *formatter);
};

pfstest_output_formatter_t *pfstest_output_formatter_standard_new(
    int (*print_char)(int));
pfstest_output_formatter_t *pfstest_output_formatter_verbose_new(
    int (*print_char)(int));

int pfstest_output_formatter_print_char(
    pfstest_output_formatter_t *formatter, int c);
void pfstest_output_formatter_print_escaped_char(
    pfstest_output_formatter_t *formatter, int c);
void pfstest_output_formatter_print_nv_string(
    pfstest_output_formatter_t *formatter, const pfstest_nv_ptr char *s);
void pfstest_output_formatter_print_int(
    pfstest_output_formatter_t *formatter, intmax_t i);
void pfstest_output_formatter_print_uint(
    pfstest_output_formatter_t *formatter, uintmax_t i,
    unsigned int base, int zpad);

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
    const pfstest_nv_ptr char *file, int line);
void pfstest_output_formatter_test_failed_message_complete(
    pfstest_output_formatter_t *formatter);
void pfstest_output_formatter_test_complete(
    pfstest_output_formatter_t *formatter);
void pfstest_output_formatter_run_complete(
    pfstest_output_formatter_t *formatter);

int pfstest_output_formatter_return_value(
    pfstest_output_formatter_t *formatter);

#endif /* !PFSTEST_OUTPUT_H */
