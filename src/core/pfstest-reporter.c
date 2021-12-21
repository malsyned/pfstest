/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include "pfstest-reporter.h"

struct escape_char_map
{
    char ch;
    const pfstest_pg_ptr char *str;
};

static const pfstest_pg char esc_a[] = "\\a";
static const pfstest_pg char esc_b[] = "\\b";
static const pfstest_pg char esc_f[] = "\\f";
static const pfstest_pg char esc_n[] = "\\n";
static const pfstest_pg char esc_r[] = "\\r";
static const pfstest_pg char esc_t[] = "\\t";
static const pfstest_pg char esc_v[] = "\\v";
/* MPLAB C18 v3.03 was calling "\\\\" a syntax error */
static const pfstest_pg char esc_backslash[] = {'\\', '\\', '\0'};
static const pfstest_pg char esc_apostrophe[] = "\\'";
static const pfstest_pg char esc_quote[] = "\\\"";
static const pfstest_pg char esc_question[] = "\\?";

static const pfstest_pg struct escape_char_map escape_char_map[] =
{
    {'\a', esc_a},
    {'\b', esc_b},
    {'\f', esc_f},
    {'\n', esc_n},
    {'\r', esc_r},
    {'\t', esc_t},
    {'\v', esc_v},
    {'\\', esc_backslash},
    {'\'', esc_apostrophe},
    {'\"', esc_quote},
    {'\?', esc_question},
};

void pfstest_reporter_print_escaped_char(pfstest_reporter_t *reporter, int c)
{
    size_t i;
    char ch;
    const pfstest_pg_ptr char *str;

    for (i = 0; i < sizeof(escape_char_map)/sizeof(escape_char_map[0]); i++)
    {
        PFSTEST_READ_PG(ch, escape_char_map[i].ch);
        if (c == ch) {
            PFSTEST_READ_PG(str, escape_char_map[i].str);
            pfstest_reporter_print_pg_str(reporter, str);
            return;
        }
    }
    pfstest_reporter_print_char(reporter, c);
}

void pfstest_reporter_print_pg_str(pfstest_reporter_t *reporter,
                                   const pfstest_pg_ptr char *s)
{
    char c;

    while (PFSTEST_READ_RETURN_PG(c, *s++)) {
        pfstest_reporter_print_char(reporter, c);
    }
}

void pfstest_reporter_print_int(pfstest_reporter_t *reporter,
                                pfstest_intmax_t n)
{
    pfstest_uintmax_t nabs;

    if (n < 0) {
        pfstest_reporter_print_char(reporter, '-');
        /* Casting a negative number to an unsigned type is guaranteed
         * to result in that number, modulo MAX+1. (C Standard
         * 6.3.1.3.2) */
        nabs = PFSTEST_UINTMAX_MAX - (pfstest_uintmax_t)n + 1;
    } else {
        nabs = (pfstest_uintmax_t)n;
    }

    pfstest_reporter_print_uint(reporter, nabs, 10, 0);
}

static const pfstest_pg char digits[] = "0123456789abcdef";

static char digit_char(unsigned int digit)
{
    char d;

    pfstest_c_assert(digit <= 0xf);

    PFSTEST_READ_PG(d, digits[digit]);

    return d;
}

void pfstest_reporter_print_uint(pfstest_reporter_t *reporter,
                                 pfstest_uintmax_t n,
                                 unsigned int base, unsigned int zpad)
{
    pfstest_uintmax_t d = 1;
    unsigned int digits = 1;

    while (n / d > (base - 1)) {
        d *= base;
        digits++;
    }

    while (zpad-- > digits) {
        pfstest_reporter_print_char(reporter, '0');
    }

    do {
        pfstest_reporter_print_char(
            reporter, digit_char((unsigned int)(n / d % base)));
        d /= base;
    } while (d > 0);
}

/* VTable Dispatchers */

int pfstest_reporter_print_char(pfstest_reporter_t *reporter, int c)
{
    int (*print_char)(pfstest_reporter_t *, int);
    PFSTEST_READ_PG(print_char, reporter->vtable->print_char);

    return print_char(reporter, c);
}

void pfstest_reporter_run_started(pfstest_reporter_t *reporter)
{
    void (*run_started)(pfstest_reporter_t *);
    PFSTEST_READ_PG(run_started, reporter->vtable->run_started);

    run_started(reporter);
}

void pfstest_reporter_test_started(pfstest_reporter_t *reporter,
                                   const pfstest_pg_ptr char *test_name,
                                   const pfstest_pg_ptr char *test_file)
{
    void (*test_started)(pfstest_reporter_t *,
                         const pfstest_pg_ptr char *,
                         const pfstest_pg_ptr char *);
    PFSTEST_READ_PG(test_started, reporter->vtable->test_started);

    test_started(reporter, test_name, test_file);
}

void pfstest_reporter_test_ignored(pfstest_reporter_t *reporter)
{
    void (*test_ignored)(pfstest_reporter_t *);
    PFSTEST_READ_PG(test_ignored, reporter->vtable->test_ignored);

    test_ignored(reporter);
}

void pfstest_reporter_test_failed_message_start(
    pfstest_reporter_t *reporter, const pfstest_pg_ptr char *file, int line)
{
    void (*test_failed_message_start)(pfstest_reporter_t *,
                                      const pfstest_pg_ptr char *,
                                      int);
    PFSTEST_READ_PG(test_failed_message_start,
                    reporter->vtable->test_failed_message_start);
    
    test_failed_message_start(reporter, file, line);
}

void pfstest_reporter_test_failed_message_complete(
    pfstest_reporter_t *reporter)
{
    void (*test_failed_message_complete)(pfstest_reporter_t *);
    PFSTEST_READ_PG(test_failed_message_complete,
                    reporter->vtable->test_failed_message_complete);
        
    test_failed_message_complete(reporter);
}

void pfstest_reporter_test_complete(pfstest_reporter_t *reporter)
{
    void (*test_complete)(pfstest_reporter_t *);
    PFSTEST_READ_PG(test_complete, reporter->vtable->test_complete);
        
    test_complete(reporter);
}

void pfstest_reporter_run_complete(pfstest_reporter_t *reporter)
{
    void (*run_complete)(pfstest_reporter_t *);
    PFSTEST_READ_PG(run_complete, reporter->vtable->run_complete);
        
    run_complete(reporter);
}

int pfstest_reporter_return_value(pfstest_reporter_t *reporter)
{
    int (*return_value)(pfstest_reporter_t *);
    PFSTEST_READ_PG(return_value, reporter->vtable->return_value);
        
    return return_value(reporter);
}
