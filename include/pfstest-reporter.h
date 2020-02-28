#ifndef PFSTEST_REPORTER_H
#define PFSTEST_REPORTER_H

/** @file */

/** @dir src/reporters
 *
 * Output test results in human- and machine-readable formats
 *
 * @section Standard
 * - pfstest_reporter_standard_new()
 * - pfstest_reporter_verbose_new()
 *
 * @section XML
 * - pfstest_reporter_xml_new()
 */

#include "pfstest-platform.h"

typedef struct _pfstest_reporter_vtable_t pfstest_reporter_vtable_t;

typedef struct 
{
    const pfstest_pg_ptr pfstest_reporter_vtable_t *vtable;
    int (*char_writer)(int);
} pfstest_reporter_t;

struct _pfstest_reporter_vtable_t
{
    int (*print_char)(pfstest_reporter_t *reporter, int c);
    void (*run_started)(pfstest_reporter_t *reporter);
    void (*test_started)(pfstest_reporter_t *reporter,
                         const pfstest_pg_ptr char *test_name,
                         const pfstest_pg_ptr char *test_file);
    void (*test_ignored)(pfstest_reporter_t *reporter);
    void (*test_failed_message_start)(pfstest_reporter_t *reporter,
                                      const pfstest_pg_ptr char *file,
                                      int line);
    void (*test_failed_message_complete)(pfstest_reporter_t *reporter);
    void (*test_complete)(pfstest_reporter_t *reporter);
    void (*run_complete)(pfstest_reporter_t *reporter);
    int (*return_value)(pfstest_reporter_t *reporter);
};

int pfstest_reporter_print_char(pfstest_reporter_t *reporter, int c);
void pfstest_reporter_print_escaped_char(pfstest_reporter_t *reporter, int c);
void pfstest_reporter_print_pg_str(pfstest_reporter_t *reporter,
                                   const pfstest_pg_ptr char *s);
void pfstest_reporter_print_int(pfstest_reporter_t *reporter,
                                pfstest_intmax_t i);
void pfstest_reporter_print_uint(pfstest_reporter_t *reporter,
                                 pfstest_uintmax_t i,
                                 unsigned int base, int zpad);

void pfstest_reporter_run_started(pfstest_reporter_t *reporter);
void pfstest_reporter_test_started(
    pfstest_reporter_t *reporter,
    const pfstest_pg_ptr char *test_name,
    const pfstest_pg_ptr char *test_file);
void pfstest_reporter_test_ignored(pfstest_reporter_t *reporter);
void pfstest_reporter_test_failed_message_start(
    pfstest_reporter_t *reporter, const pfstest_pg_ptr char *file, int line);
void pfstest_reporter_test_failed_message_complete(
    pfstest_reporter_t *reporter);
void pfstest_reporter_test_complete(pfstest_reporter_t *reporter);
void pfstest_reporter_run_complete(pfstest_reporter_t *reporter);

int pfstest_reporter_return_value(pfstest_reporter_t *reporter);

#endif /* !PFSTEST_REPORTER_H */
