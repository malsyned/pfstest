#include "pfstest-output.h"

#include "pfstest-alloc.h"
#include "pfstest-basename.h"

typedef struct
{
    int passed;
    int failed;
    int ignored;
} results_t;

typedef struct
{
    pfstest_output_formatter_t parent;
    results_t results;
    volatile bool test_failed;
    bool test_ignored;
    bool fresh_line;
    bool indent;
    const pfstest_nv_ptr char *test_name;
    const pfstest_nv_ptr char *test_file;
} builtin_formatter_t;

static int print_char(builtin_formatter_t *formatter, int c)
{
    pfstest_output_formatter_t *as_output_formatter =
        (pfstest_output_formatter_t *)formatter;
    int i, r;

    if (formatter->indent && formatter->fresh_line && c != '\n') {
        for (i = 0; i < 4; i++) {
            r = as_output_formatter->char_writer(' ');
            if (r != ' ')
                return r;
        }
    }
    formatter->fresh_line = (c == '\n');
    return as_output_formatter->char_writer(c);
}

static void print_nv_string(builtin_formatter_t *formatter,
                            const pfstest_nv_ptr char *s)
{
    pfstest_output_formatter_print_nv_string(
        (pfstest_output_formatter_t *)formatter, s);
}

static void get_fresh_line(builtin_formatter_t *formatter)
{
    if (!formatter->fresh_line) {
        print_char(formatter, '\n');
    }
}

static int builtin_print_char(pfstest_output_formatter_t *formatter,
                              int c)
{
    return print_char((builtin_formatter_t *)formatter, c);
}

static void print_int(builtin_formatter_t *formatter, intmax_t n)
{
    pfstest_output_formatter_print_int(
        (pfstest_output_formatter_t *)formatter, n);
}

static void print_context(builtin_formatter_t *formatter)
{
    get_fresh_line(formatter);
    print_nv_string(formatter, pfstest_basename(formatter->test_file));
    print_nv_string(formatter, pfstest_nv_string(":"));
    print_nv_string(formatter, formatter->test_name);
    print_nv_string(formatter, pfstest_nv_string(" "));
}

static void run_started_bookkeeping(builtin_formatter_t *formatter)
{
    formatter->results.passed = 0;
    formatter->results.failed = 0;
    formatter->results.ignored = 0;
}

static void run_started(pfstest_output_formatter_t *formatter)
{
    builtin_formatter_t *as_builtin_formatter =
        (builtin_formatter_t *)formatter;

    run_started_bookkeeping(as_builtin_formatter);
    print_nv_string(as_builtin_formatter,
                    pfstest_nv_string("PFSTest 0.2\n"));
    print_nv_string(as_builtin_formatter,
                    pfstest_nv_string("===========\n"));
}

static void test_started_bookkeeping(builtin_formatter_t *formatter,
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
    builtin_formatter_t *as_builtin_formatter =
        (builtin_formatter_t *)formatter;

    test_started_bookkeeping(as_builtin_formatter, test_name, test_file);
}

static void test_started_verbose(pfstest_output_formatter_t *formatter,
                                 const pfstest_nv_ptr char *test_name,
                                 const pfstest_nv_ptr char *test_file)
{
    builtin_formatter_t *as_builtin_formatter =
        (builtin_formatter_t *)formatter;

    test_started_bookkeeping(as_builtin_formatter, test_name, test_file);
    print_context(as_builtin_formatter);
}

static void test_ignored_bookkeeping(builtin_formatter_t *formatter)
{
    formatter->test_ignored = true;
    formatter->results.ignored++;
}

static void test_ignored_standard(pfstest_output_formatter_t *formatter)
{
    builtin_formatter_t *as_builtin_formatter =
        (builtin_formatter_t *)formatter;

    test_ignored_bookkeeping(as_builtin_formatter);
    print_char(as_builtin_formatter, 'I');
}

static void test_ignored_verbose(pfstest_output_formatter_t *formatter)
{
    builtin_formatter_t *as_builtin_formatter =
        (builtin_formatter_t *)formatter;

    test_ignored_bookkeeping(as_builtin_formatter);
    print_nv_string(as_builtin_formatter, pfstest_nv_string("IGNORED\n"));
}

static void test_failed_message_start_common(
    builtin_formatter_t *formatter,
    const pfstest_nv_ptr char *file, int line)
{
    print_nv_string(formatter, pfstest_nv_string("FAIL"));
    print_nv_string(formatter, pfstest_nv_string("\n"));
    print_nv_string(formatter, pfstest_nv_string("    Location: "));
    print_nv_string(formatter, pfstest_basename(file));
    print_nv_string(formatter, pfstest_nv_string(":"));
    print_int(formatter, line);
    print_nv_string(formatter, pfstest_nv_string("\n"));

    formatter->indent = true;
}

static void test_failed_message_start_standard(
    pfstest_output_formatter_t *formatter,
    const pfstest_nv_ptr char *file, int line)
{
    builtin_formatter_t *as_builtin_formatter =
        (builtin_formatter_t *)formatter;

    print_context(as_builtin_formatter);
    test_failed_message_start_common(as_builtin_formatter, file, line);
}

static void test_failed_message_start_verbose(
    pfstest_output_formatter_t *formatter,
    const pfstest_nv_ptr char *file, int line)
{
    builtin_formatter_t *as_builtin_formatter =
        (builtin_formatter_t *)formatter;

    if (as_builtin_formatter->test_failed) {
        print_context(as_builtin_formatter);
    }

    test_failed_message_start_common(as_builtin_formatter, file, line);
}

static void test_failed_message_complete(
    pfstest_output_formatter_t *formatter)
{
    builtin_formatter_t *as_builtin_formatter =
        (builtin_formatter_t *)formatter;

    if (!as_builtin_formatter->test_failed) {
        as_builtin_formatter->results.failed++;
        as_builtin_formatter->test_failed = true;
    }
    print_nv_string(as_builtin_formatter, pfstest_nv_string("\n"));
    as_builtin_formatter->indent = false;
}

static bool test_passed(builtin_formatter_t *formatter)
{
    return (!formatter->test_ignored && !formatter->test_failed);
}

static void test_passed_bookkeeping(builtin_formatter_t *formatter)
{
    formatter->results.passed++;
}

static void test_complete_standard(pfstest_output_formatter_t *formatter)
{
    builtin_formatter_t *as_builtin_formatter =
        (builtin_formatter_t *)formatter;

    if (test_passed(as_builtin_formatter)) {
        test_passed_bookkeeping(as_builtin_formatter);
        print_char(as_builtin_formatter, '.');
    }
}

static void test_complete_verbose(pfstest_output_formatter_t *formatter)
{
    builtin_formatter_t *as_builtin_formatter =
        (builtin_formatter_t *)formatter;

    if (test_passed(as_builtin_formatter)) {
        test_passed_bookkeeping(as_builtin_formatter);
        print_nv_string(as_builtin_formatter, pfstest_nv_string("PASS\n"));
    }
}

static void run_complete(pfstest_output_formatter_t *formatter)
{
    builtin_formatter_t *as_builtin_formatter =
        (builtin_formatter_t *)formatter;

    print_nv_string(as_builtin_formatter,
                    pfstest_nv_string("\nRun complete. "));
    print_int(as_builtin_formatter, as_builtin_formatter->results.passed);
    print_nv_string(as_builtin_formatter, pfstest_nv_string(" passed, "));
    print_int(as_builtin_formatter, as_builtin_formatter->results.failed);
    print_nv_string(as_builtin_formatter, pfstest_nv_string(" failed, "));
    print_int(as_builtin_formatter, as_builtin_formatter->results.ignored);
    print_nv_string(as_builtin_formatter, pfstest_nv_string(" ignored\n"));
}

static int return_value(pfstest_output_formatter_t *formatter)
{
    builtin_formatter_t *as_builtin_formatter =
        (builtin_formatter_t *)formatter;

    return as_builtin_formatter->results.failed;
}

static const pfstest_nv pfstest_output_formatter_vtable_t standard_vtable = {
    builtin_print_char,
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
    builtin_print_char,
    run_started,
    test_started_verbose,
    test_ignored_verbose,
    test_failed_message_start_verbose,
    test_failed_message_complete,
    test_complete_verbose,
    run_complete,
    return_value
};

static void bookkeeping_init(builtin_formatter_t *formatter)
{
    formatter->fresh_line = true;
    formatter->indent = false;
}


pfstest_output_formatter_t *pfstest_output_formatter_standard_new(
    int (*char_writer)(int))
{
    builtin_formatter_t *formatter =
        pfstest_alloc(sizeof(*formatter));

    formatter->parent.vtable = &standard_vtable;
    formatter->parent.char_writer = char_writer;
    bookkeeping_init(formatter);

    return (pfstest_output_formatter_t *)formatter;
}

pfstest_output_formatter_t *pfstest_output_formatter_verbose_new(
    int (*char_writer)(int))
{
    builtin_formatter_t *formatter =
        pfstest_alloc(sizeof(*formatter));

    formatter->parent.vtable = &verbose_vtable;
    formatter->parent.char_writer = char_writer;
    bookkeeping_init(formatter);

    return (pfstest_output_formatter_t *)formatter;
}

/* Message Output */

void pfstest_output_formatter_print_escaped_char(
    pfstest_output_formatter_t *formatter, int c)
{
    if (c == '\n') {
        pfstest_output_formatter_print_nv_string(
            formatter, pfstest_nv_string("\\n"));
    } else if (c == '\"') {
        pfstest_output_formatter_print_nv_string(
            formatter, pfstest_nv_string("\\\""));
    } else {
        pfstest_output_formatter_print_char(formatter, c);
    }
}

void pfstest_output_formatter_print_nv_string(
    pfstest_output_formatter_t *formatter, const pfstest_nv_ptr char *s)
{
    char c;

    while (pfstest_memcpy_nv(&c, s, sizeof(c)), c) {
        pfstest_output_formatter_print_char(formatter, c);
        s++;
    }
}

void pfstest_output_formatter_print_int(
    pfstest_output_formatter_t *formatter, intmax_t n)
{
    uintmax_t nabs;

    if (n < 0) {
        pfstest_output_formatter_print_char(formatter, '-');
        /* Casting a negative number to an unsigned type is guaranteed
         * to result in that number, modulo MAX+1. (C Standard
         * 6.3.1.3.2) */
        nabs = UINTMAX_MAX - (uintmax_t)n + 1;
    } else {
        nabs = n;
    }

    pfstest_output_formatter_print_uint(formatter, nabs, 10, 0);
}

static const pfstest_nv char digits[] = "0123456789abcdef";

static char digit_char(int digit)
{
    char d;

    pfstest_memcpy_nv(&d, &digits[digit], sizeof(d));

    return d;
}

void pfstest_output_formatter_print_uint(
    pfstest_output_formatter_t *formatter, uintmax_t n,
    unsigned int base, int zpad)
{
    uintmax_t d = 1;
    int digits = 1;

    while (n / d > (base - 1)) {
        d *= base;
        digits++;
    }

    while (zpad-- > digits) {
        pfstest_output_formatter_print_char(formatter, '0');
    }

    do {
        pfstest_output_formatter_print_char(
            formatter, digit_char(n / d % base));
        d /= base;
    } while (d > 0);
}

/* VTable Dispatchers */

int pfstest_output_formatter_print_char(
    pfstest_output_formatter_t *formatter, int c)
{
    int (*print_char)(pfstest_output_formatter_t *, int);
    pfstest_memcpy_nv(&print_char, &(formatter->vtable->print_char),
                      sizeof(print_char));
    return print_char(formatter, c);
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
