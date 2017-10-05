#ifndef PFSTEST_REPORTER_H
#define PFSTEST_REPORTER_H

#include "pfstest-platform.h"

typedef struct _pfstest_report_colorizer_t pfstest_report_colorizer_t;
extern pfstest_report_colorizer_t *pfstest_report_colorizer_null;
extern pfstest_report_colorizer_t *pfstest_report_colorizer_ansi;

typedef struct _pfstest_reporter_vtable_t pfstest_reporter_vtable_t;

typedef struct 
{
    const pfstest_nv_ptr pfstest_reporter_vtable_t *vtable;
    int (*char_writer)(int);
} pfstest_reporter_t;

struct _pfstest_reporter_vtable_t
{
    int (*print_char)(pfstest_reporter_t *reporter, int c);
    void (*run_started)(pfstest_reporter_t *reporter);
    void (*test_started)(pfstest_reporter_t *reporter,
                         const pfstest_nv_ptr char *test_name,
                         const pfstest_nv_ptr char *test_file);
    void (*test_ignored)(pfstest_reporter_t *reporter);
    void (*test_failed_message_start)(pfstest_reporter_t *reporter,
                                      const pfstest_nv_ptr char *file,
                                      int line);
    void (*test_failed_message_complete)(pfstest_reporter_t *reporter);
    void (*test_complete)(pfstest_reporter_t *reporter);
    void (*run_complete)(pfstest_reporter_t *reporter);
    int (*return_value)(pfstest_reporter_t *reporter);
};

pfstest_reporter_t *pfstest_reporter_standard_new(
    int (*print_char)(int), pfstest_report_colorizer_t *colorizer);
pfstest_reporter_t *pfstest_reporter_verbose_new(
    int (*print_char)(int), pfstest_report_colorizer_t *colorizer);

int pfstest_reporter_print_char(pfstest_reporter_t *reporter, int c);
void pfstest_reporter_print_escaped_char(pfstest_reporter_t *reporter, int c);
void pfstest_reporter_print_nv_string(
    pfstest_reporter_t *reporter, const pfstest_nv_ptr char *s);
void pfstest_reporter_print_int(pfstest_reporter_t *reporter, intmax_t i);
void pfstest_reporter_print_uint(
    pfstest_reporter_t *reporter, uintmax_t i, unsigned int base, int zpad);

void pfstest_reporter_run_started(pfstest_reporter_t *reporter);
void pfstest_reporter_test_started(
    pfstest_reporter_t *reporter,
    const pfstest_nv_ptr char *test_name,
    const pfstest_nv_ptr char *test_file);
void pfstest_reporter_test_ignored(pfstest_reporter_t *reporter);
void pfstest_reporter_test_failed_message_start(
    pfstest_reporter_t *reporter, const pfstest_nv_ptr char *file, int line);
void pfstest_reporter_test_failed_message_complete(
    pfstest_reporter_t *reporter);
void pfstest_reporter_test_complete(pfstest_reporter_t *reporter);
void pfstest_reporter_run_complete(pfstest_reporter_t *reporter);

int pfstest_reporter_return_value(pfstest_reporter_t *reporter);

#endif /* !PFSTEST_REPORTER_H */
