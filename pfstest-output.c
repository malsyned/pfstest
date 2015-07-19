#include "pfstest-output.h"

#include "pfstest-alloc.h"

static int print_char(pfstest_output_formatter_t *formatter, int c)
{
        formatter->fresh_line = (c == '\n');
        return formatter->print_char(c);
}

static void print_nv_string(pfstest_output_formatter_t *formatter,
                            const pfstest_nv_ptr char *s)
{
    char c;

    while (pfstest_memcpy_nv(&c, s, sizeof(c)), c) {
        print_char(formatter, c);
        s++;
    }
}

static void get_fresh_line(pfstest_output_formatter_t *formatter)
{
    if (!formatter->fresh_line) {
        print_char(formatter, '\n');
    }
}

static int message_print_char(pfstest_output_formatter_t *formatter,
                              int c)
{
    if (formatter->fresh_line && c != '\n') {
        print_nv_string(formatter, pfstest_nv_string("    "));
    }
    return print_char(formatter, c);
}

static const pfstest_nv char digits[] = "0123456789abcdef";

static char digit_char(int digit)
{
    char d;

    pfstest_memcpy_nv(&d, &digits[digit], sizeof(d));

    return d;
}

static void print_uint(pfstest_output_formatter_t *formatter,
                       uintmax_t n, int base, int zpad)
{
    uintmax_t d = 1;
    int digits = 1;

    while (n / d > (base - 1)) {
        d *= base;
        digits++;
    }

    while (zpad-- > digits) {
        message_print_char(formatter, '0');
    }

    do {
        message_print_char(formatter, digit_char(n / d % base));
        d /= base;
    } while (d > 0);
}

static void print_int(pfstest_output_formatter_t *formatter, intmax_t n)
{
    uintmax_t nabs;

    if (n < 0) {
        message_print_char(formatter, '-');
        /* Casting a negative number to an unsigned type is guaranteed
         * to result in that number, modulo MAX+1. (C Standard
         * 6.3.1.3.2) */
        nabs = UINTMAX_MAX - (uintmax_t)n + 1;
    } else {
        nabs = n;
    }

    print_uint(formatter, nabs, 10, 0);
}

static void print_context(pfstest_output_formatter_t *formatter)
{
    get_fresh_line(formatter);
    print_nv_string(formatter, formatter->test_file);
    print_nv_string(formatter, pfstest_nv_string(":"));
    print_nv_string(formatter, formatter->test_name);
    print_nv_string(formatter, pfstest_nv_string(" "));
}

static void run_started_bookkeeping(pfstest_output_formatter_t *formatter)
{
    formatter->results.passed = 0;
    formatter->results.failed = 0;
    formatter->results.ignored = 0;
}

static void run_started(pfstest_output_formatter_t *formatter)
{
    run_started_bookkeeping(formatter);
    print_nv_string(formatter, pfstest_nv_string("PFSTest 0.2\n"));
    print_nv_string(formatter, pfstest_nv_string("===========\n"));
}

static void test_started_bookkeeping(pfstest_output_formatter_t *formatter,
                                     const pfstest_nv_ptr char *test_name,
                                     const pfstest_nv_ptr char *test_file)
{
    formatter->test_failed = false;
    formatter->test_ignored = false;
    formatter->test_name = test_name;
    formatter->test_file = test_file;
}

static void test_started_standard(pfstest_output_formatter_t *formatter,
                                  const pfstest_nv_ptr char *test_name,
                                  const pfstest_nv_ptr char *test_file)
{
    test_started_bookkeeping(formatter, test_name, test_file);
}

static void test_started_verbose(pfstest_output_formatter_t *formatter,
                                 const pfstest_nv_ptr char *test_name,
                                 const pfstest_nv_ptr char *test_file)
{
    test_started_bookkeeping(formatter, test_name, test_file);
    print_context(formatter);
}

static void test_ignored_bookkeeping(pfstest_output_formatter_t *formatter)
{
    formatter->test_ignored = true;
    formatter->results.ignored++;
}

static void test_ignored_standard(pfstest_output_formatter_t *formatter)
{
    test_ignored_bookkeeping(formatter);
    print_char(formatter, 'I');
}

static void test_ignored_verbose(pfstest_output_formatter_t *formatter)
{
    test_ignored_bookkeeping(formatter);
    print_nv_string(formatter, pfstest_nv_string("IGNORED\n"));
}

static void test_failed_message_start_common(
    pfstest_output_formatter_t *formatter,
    const pfstest_nv_ptr char *file, int line)
{
    print_nv_string(formatter, pfstest_nv_string("FAIL"));
    print_nv_string(formatter, pfstest_nv_string("\n"));
    print_nv_string(formatter, pfstest_nv_string("    Location: "));
    print_nv_string(formatter, file);
    print_nv_string(formatter, pfstest_nv_string(":"));
    print_int(formatter, line);
    print_nv_string(formatter, pfstest_nv_string("\n"));
}

static void test_failed_message_start_standard(
    pfstest_output_formatter_t *formatter,
    const pfstest_nv_ptr char *file, int line)
{
    print_context(formatter);
    test_failed_message_start_common(formatter, file, line);
}

static void test_failed_message_start_verbose(
    pfstest_output_formatter_t *formatter,
    const pfstest_nv_ptr char *file, int line)
{
    if (formatter->test_failed) {
        print_context(formatter);
    }

    test_failed_message_start_common(formatter, file, line);
}

static void test_failed_message_complete(
    pfstest_output_formatter_t *formatter)
{
    if (!formatter->test_failed) {
        formatter->results.failed++;
        formatter->test_failed = true;
    }
    print_nv_string(formatter, pfstest_nv_string("\n"));
}

static bool test_passed(pfstest_output_formatter_t *formatter)
{
    return (!formatter->test_ignored && !formatter->test_failed);
}

static void test_passed_bookkeeping(pfstest_output_formatter_t *formatter)
{
    formatter->results.passed++;
}

static void test_complete_standard(pfstest_output_formatter_t *formatter)
{
    if (test_passed(formatter)) {
        test_passed_bookkeeping(formatter);
        print_char(formatter, '.');
    }
}

static void test_complete_verbose(pfstest_output_formatter_t *formatter)
{
    if (test_passed(formatter)) {
        test_passed_bookkeeping(formatter);
        print_nv_string(formatter, pfstest_nv_string("PASS\n"));
    }
}

static void run_complete(pfstest_output_formatter_t *formatter)
{
    print_nv_string(formatter, pfstest_nv_string("\nRun complete. "));
    print_int(formatter, formatter->results.passed);
    print_nv_string(formatter, pfstest_nv_string(" passed, "));
    print_int(formatter, formatter->results.failed);
    print_nv_string(formatter, pfstest_nv_string(" failed, "));
    print_int(formatter, formatter->results.ignored);
    print_nv_string(formatter, pfstest_nv_string(" ignored\n"));
}

static int return_value(pfstest_output_formatter_t *formatter)
{
    return formatter->results.failed;
}

static const pfstest_nv pfstest_output_formatter_vtable_t standard_vtable = {
    message_print_char,
    run_started,
    test_started_standard,
    test_ignored_standard,
    test_failed_message_start_standard,
    test_failed_message_complete,
    test_complete_standard,
    run_complete,
    return_value
};

static const pfstest_nv pfstest_output_formatter_vtable_t verbose_vtable = {
    message_print_char,
    run_started,
    test_started_verbose,
    test_ignored_verbose,
    test_failed_message_start_verbose,
    test_failed_message_complete,
    test_complete_verbose,
    run_complete,
    return_value
};

static void test_failed_message_complete_message_spy(
    pfstest_output_formatter_t *formatter)
{
    formatter->results.failed++;
}

static void test_started_null(pfstest_output_formatter_t *formatter,
                              const pfstest_nv_ptr char *test_name,
                              const pfstest_nv_ptr char *test_file)
{
}

static void test_ignored_null(pfstest_output_formatter_t *formatter)
{
}

static void test_failed_message_start_null(
    pfstest_output_formatter_t *formatter,
    const pfstest_nv_ptr char *file,
    int line)
{
}

static void test_complete_null(pfstest_output_formatter_t *formatter)
{
}

static void run_complete_null(pfstest_output_formatter_t *formatter)
{
}

static int message_print_char_message_spy(
    pfstest_output_formatter_t *formatter, int c)
{
    return print_char(formatter, c);
}

static const
pfstest_nv pfstest_output_formatter_vtable_t message_spy_vtable = {
    message_print_char_message_spy,
    run_started_bookkeeping,
    test_started_null,
    test_ignored_null,
    test_failed_message_start_null,
    test_failed_message_complete_message_spy,
    test_complete_null,
    run_complete_null,
    return_value,
};

static void bookkeeping_init(pfstest_output_formatter_t *formatter,
                             int (*print_char)(int))
{
    formatter->print_char = print_char;
    formatter->fresh_line = true;
}


pfstest_output_formatter_t *pfstest_output_formatter_standard_new(
    int (*print_char)(int))
{
    pfstest_output_formatter_t *formatter =
        pfstest_alloc(sizeof(*formatter));

    bookkeeping_init(formatter, print_char);
    formatter->vtable = &standard_vtable;

    return formatter;
}

pfstest_output_formatter_t *pfstest_output_formatter_verbose_new(
    int (*print_char)(int))
{
    pfstest_output_formatter_t *formatter =
        pfstest_alloc(sizeof(*formatter));

    bookkeeping_init(formatter, print_char);
    formatter->vtable = &verbose_vtable;

    return formatter;
}

pfstest_output_formatter_t *pfstest_output_formatter_message_spy_new(
    int (*print_char)(int))
{
    pfstest_output_formatter_t *formatter =
        pfstest_alloc(sizeof(*formatter));

    formatter->results.failed = 0;
    formatter->print_char = print_char;
    formatter->vtable = &message_spy_vtable;

    return formatter;
}

/* Message Output */

void pfstest_output_formatter_message_print_escaped_char(
    pfstest_output_formatter_t *formatter, int c)
{
    if (c == '\n') {
        pfstest_output_formatter_message_print_nv_string(
            formatter, pfstest_nv_string("\\n"));
    } else if (c == '\"') {
        pfstest_output_formatter_message_print_nv_string(
            formatter, pfstest_nv_string("\\\""));
    } else {
        pfstest_output_formatter_message_print_char(formatter, c);
    }
}

void pfstest_output_formatter_message_print_nv_string(
    pfstest_output_formatter_t *formatter, const pfstest_nv_ptr char *s)
{
    char c;

    while (pfstest_memcpy_nv(&c, s, sizeof(c)), c) {
        pfstest_output_formatter_message_print_char(formatter, c);
        s++;
    }
}

void pfstest_output_formatter_message_print_int(
    pfstest_output_formatter_t *formatter, intmax_t i)
{
    print_int(formatter, i);
}

void pfstest_output_formatter_message_print_uint(
    pfstest_output_formatter_t *formatter, uintmax_t i, int base, int zpad)
{
    print_uint(formatter, i, base, zpad);
}

/* VTable Dispatchers */

int pfstest_output_formatter_message_print_char(
    pfstest_output_formatter_t *formatter, int c)
{
    int (*message_print_char)(pfstest_output_formatter_t *, int);
    pfstest_memcpy_nv(&message_print_char,
                      &(formatter->vtable->message_print_char),
                      sizeof(message_print_char));
    return message_print_char(formatter, c);
}

void pfstest_output_formatter_run_started(
    pfstest_output_formatter_t *formatter)
{
    void (*run_started)(pfstest_output_formatter_t *);
    pfstest_memcpy_nv(&run_started, &(formatter->vtable->run_started),
                      sizeof(run_started));
    run_started(formatter);
}

void pfstest_output_formatter_test_started(
    pfstest_output_formatter_t *formatter,
    const pfstest_nv_ptr char *test_name,
    const pfstest_nv_ptr char *test_file)
{
    void (*test_started)(pfstest_output_formatter_t *,
                         const pfstest_nv_ptr char *,
                         const pfstest_nv_ptr char *);
    pfstest_memcpy_nv(&test_started, &formatter->vtable->test_started,
                      sizeof(test_started));
    test_started(formatter, test_name, test_file);
}

void pfstest_output_formatter_test_ignored(
    pfstest_output_formatter_t *formatter)
{
    void (*test_ignored)(pfstest_output_formatter_t *);
    pfstest_memcpy_nv(&test_ignored, &formatter->vtable->test_ignored,
                      sizeof(test_ignored));
    test_ignored(formatter);
}

void pfstest_output_formatter_test_failed_message_start(
    pfstest_output_formatter_t *formatter,
    const pfstest_nv_ptr char *file, int line)
{
    void (*test_failed_message_start)(pfstest_output_formatter_t *,
                                      const pfstest_nv_ptr char *,
                                      int);
    pfstest_memcpy_nv(&test_failed_message_start,
                      &formatter->vtable->test_failed_message_start,
                      sizeof(test_failed_message_start));
    test_failed_message_start(formatter, file, line);
}

void pfstest_output_formatter_test_failed_message_complete(
    pfstest_output_formatter_t *formatter)
{
    void (*test_failed_message_complete)(
        pfstest_output_formatter_t *);
    pfstest_memcpy_nv(&test_failed_message_complete,
                      &formatter->vtable->test_failed_message_complete,
                      sizeof(test_failed_message_complete));
    test_failed_message_complete(formatter);
}

void pfstest_output_formatter_test_complete(
    pfstest_output_formatter_t *formatter)
{
    void (*test_complete)(pfstest_output_formatter_t *);
    pfstest_memcpy_nv(&test_complete, &formatter->vtable->test_complete,
                      sizeof(test_complete));
    test_complete(formatter);
}

void pfstest_output_formatter_run_complete(
    pfstest_output_formatter_t *formatter)
{
    void (*run_complete)(pfstest_output_formatter_t *);
    pfstest_memcpy_nv(&run_complete, &formatter->vtable->run_complete,
                      sizeof(run_complete));
    run_complete(formatter);
}

int pfstest_output_formatter_return_value(
    pfstest_output_formatter_t *formatter)
{
    int (*return_value)(pfstest_output_formatter_t *);
    pfstest_memcpy_nv(&return_value, &formatter->vtable->return_value,
                      sizeof(return_value));
    return return_value(formatter);
}
