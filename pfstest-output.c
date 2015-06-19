#include "pfstest-output.h"

static void print_char(pfstest_output_formatter_t *formatter, int c)
{
        formatter->fresh_line = (c == '\n');
        formatter->print_char(c);
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

static void message_print_nv_string(pfstest_output_formatter_t *formatter,
                                    const pfstest_nv_ptr char *s)
{
    char c;

    while (pfstest_memcpy_nv(&c, s, sizeof(c)), c) {
        if (formatter->fresh_line && c != '\n') {
            print_nv_string(formatter, pfstest_nv_string("    "));
        }
        print_char(formatter, c);
        s++;
    }
}

static void print_int(pfstest_output_formatter_t *formatter, int n)
{
    int d = 1;
    int ntemp = n;

    /* FIXME: Doesn't handle INT_MIN properly (neither does Unity) */
    if (n < 0) {
        print_char(formatter, '-');
        n = -n;
    }

    while (ntemp > 9) {
        ntemp /= 10;
        d *= 10;
    }

    do {
        print_char(formatter, '0' + n / d % 10);
        d /= 10;
    } while (d > 0);
}

static void print_context(pfstest_output_formatter_t *formatter)
{
    get_fresh_line(formatter);
    print_nv_string(formatter, formatter->test_file);
    print_nv_string(formatter, pfstest_nv_string(":"));
    print_nv_string(formatter, formatter->test_name);
    print_nv_string(formatter, pfstest_nv_string(" "));
}

void pfstest_output_formatter_standard_init(
    pfstest_output_formatter_t *formatter, int (*print_char)(int))
{
    formatter->print_char = print_char;
    formatter->fresh_line = true;

    formatter->verbose = false;
}

void pfstest_output_formatter_verbose_init(
    pfstest_output_formatter_t *formatter, int (*print_char)(int))
{
    formatter->print_char = print_char;
    formatter->fresh_line = true;

    formatter->verbose = true;
}

void pfstest_output_formatter_print_nv_string(
    pfstest_output_formatter_t *formatter, const pfstest_nv_ptr char *s)
{
    message_print_nv_string(formatter, s);
}

void pfstest_output_formatter_run_started(
    pfstest_output_formatter_t *formatter)
{
    formatter->results.passed = 0;
    formatter->results.failed = 0;
    formatter->results.ignored = 0;
}

void pfstest_output_formatter_test_started(
    pfstest_output_formatter_t *formatter,
    const pfstest_nv_ptr char *test_name,
    const pfstest_nv_ptr char *test_file)
{
    formatter->test_failed = false;
    formatter->test_ignored = false;
    formatter->test_name = test_name;
    formatter->test_file = test_file;

    if (formatter->verbose)
        print_context(formatter);
}

void pfstest_output_formatter_test_ignored(
    pfstest_output_formatter_t *formatter)
{
    formatter->test_ignored = true;
    formatter->results.ignored++;
    if (formatter->verbose) {
        print_nv_string(formatter, pfstest_nv_string("IGNORED\n"));
    } else {
        print_char(formatter, 'I');
    }
}

void pfstest_output_formatter_test_failed_message_start(
    pfstest_output_formatter_t *formatter,
    const pfstest_nv_ptr char *file, int line,
    /* FIXME: Hack for old core */
    bool fail_expected)
{
    if (!formatter->verbose || formatter->test_failed) {
        print_context(formatter);
    }
    if (!fail_expected) {
        if (!formatter->test_failed) {
            formatter->results.failed++;
            formatter->test_failed = true;
        }
    }
    print_nv_string(formatter, pfstest_nv_string("FAIL"));
    if (fail_expected) {
        print_nv_string(formatter, pfstest_nv_string(" (expected)"));
    }
    print_nv_string(formatter, pfstest_nv_string("\n"));
    print_nv_string(formatter, pfstest_nv_string("    Location: "));
    print_nv_string(formatter, file);
    print_nv_string(formatter, pfstest_nv_string(":"));
    print_int(formatter, line);
    print_nv_string(formatter, pfstest_nv_string("\n"));
}

void pfstest_output_formatter_test_failed_message_complete(
    pfstest_output_formatter_t *formatter)
{
    print_nv_string(formatter, pfstest_nv_string("\n"));
}

void pfstest_output_formatter_test_complete(
    pfstest_output_formatter_t *formatter)
{
    if (!formatter->test_ignored && !formatter->test_failed) {
        formatter->results.passed++;
        if (formatter->verbose) {
            message_print_nv_string(formatter, pfstest_nv_string("PASS\n"));
        } else {
            print_char(formatter, '.');
        }
    }
}

void pfstest_output_formatter_run_complete(
    pfstest_output_formatter_t *formatter)
{
    print_nv_string(formatter, pfstest_nv_string("\nRun complete. "));
    print_int(formatter, formatter->results.passed);
    print_nv_string(formatter, pfstest_nv_string(" passed, "));
    print_int(formatter, formatter->results.failed);
    print_nv_string(formatter, pfstest_nv_string(" failed, "));
    print_int(formatter, formatter->results.ignored);
    print_nv_string(formatter, pfstest_nv_string(" ignored\n"));
}

int pfstest_output_formatter_return_value(
    pfstest_output_formatter_t *formatter)
{
    return formatter->results.failed;
}
