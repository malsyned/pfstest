#include "pfstest-reporters-standard.h"

#include <stdlib.h>

#include "pfstest-version.h"
#include "pfstest-alloc.h"

typedef enum {
    REPORT_COLOR_GREEN,
    REPORT_COLOR_YELLOW,
    REPORT_COLOR_RED,
    REPORT_COLOR_BOLD,
    REPORT_COLOR_VERY_RED
} report_color_t;

static void report_colorizer_start(pfstest_report_colorizer_t *this,
                                   int (*char_writer)(int),
                                   report_color_t color);
static void report_colorizer_reset(pfstest_report_colorizer_t *this,
                                   int (*char_writer)(int));

typedef struct
{
    int passed;
    int failed;
    int ignored;
} results_t;

typedef struct
{
    pfstest_reporter_t parent;
    pfstest_report_colorizer_t *colorizer;
    results_t results;
    volatile pfstest_bool test_failed;
    pfstest_bool test_ignored;
    pfstest_bool fresh_line;
    pfstest_bool indent;
    const pfstest_pg_ptr char *test_name;
    const pfstest_pg_ptr char *test_file;
} builtin_reporter_t;

static int print_char(builtin_reporter_t *breporter, int c)
{
    pfstest_reporter_t *reporter = (pfstest_reporter_t *)breporter;
    int i, r;

    if (breporter->indent && breporter->fresh_line && c != '\n') {
        for (i = 0; i < 4; i++) {
            r = reporter->char_writer(' ');
            if (r != ' ')
                return r;
        }
    }
    breporter->fresh_line = (c == '\n');
    return reporter->char_writer(c);
}

static void print_pg_str(builtin_reporter_t *breporter,
                         const pfstest_pg_ptr char *s)
{
    pfstest_reporter_print_pg_str((pfstest_reporter_t *)breporter, s);
}

static void get_fresh_line(builtin_reporter_t *breporter)
{
    if (!breporter->fresh_line) {
        print_char(breporter, '\n');
    }
}

static int builtin_print_char(pfstest_reporter_t *reporter, int c)
{
    return print_char((builtin_reporter_t *)reporter, c);
}

static void print_int(builtin_reporter_t *breporter, pfstest_intmax_t n)
{
    pfstest_reporter_print_int((pfstest_reporter_t *)breporter, n);
}

static void print_context(builtin_reporter_t *breporter)
{
    get_fresh_line(breporter);
    print_pg_str(breporter, breporter->test_file);
    print_pg_str(breporter, pfstest_pg_str(":"));
    print_pg_str(breporter, breporter->test_name);
    print_pg_str(breporter, pfstest_pg_str(" "));
}

static void run_started_bookkeeping(builtin_reporter_t *breporter)
{
    breporter->results.passed = 0;
    breporter->results.failed = 0;
    breporter->results.ignored = 0;
}

static void run_started(pfstest_reporter_t *reporter)
{
    builtin_reporter_t *breporter = (builtin_reporter_t *)reporter;

    run_started_bookkeeping(breporter);
    print_pg_str(breporter,
                 pfstest_pg_str("PFSTest " PFSTEST_VERSION_STRING "\n"));
    print_pg_str(breporter, pfstest_pg_str("===========\n"));
}

static void test_started_bookkeeping(builtin_reporter_t *breporter,
                                     const pfstest_pg_ptr char *test_name,
                                     const pfstest_pg_ptr char *test_file)
{
    breporter->test_failed = pfstest_false;
    breporter->test_ignored = pfstest_false;
    breporter->test_name = test_name;
    breporter->test_file = test_file;
}

static void test_started_standard(pfstest_reporter_t *reporter,
                                  const pfstest_pg_ptr char *test_name,
                                  const pfstest_pg_ptr char *test_file)
{
    builtin_reporter_t *breporter = (builtin_reporter_t *)reporter;

    test_started_bookkeeping(breporter, test_name, test_file);
}

static void test_started_verbose(pfstest_reporter_t *reporter,
                                 const pfstest_pg_ptr char *test_name,
                                 const pfstest_pg_ptr char *test_file)
{
    builtin_reporter_t *breporter = (builtin_reporter_t *)reporter;

    test_started_bookkeeping(breporter, test_name, test_file);
    print_context(breporter);
}

static void test_ignored_bookkeeping(builtin_reporter_t *breporter)
{
    breporter->test_ignored = pfstest_true;
    breporter->results.ignored++;
}

static void test_ignored_standard(pfstest_reporter_t *reporter)
{
    builtin_reporter_t *breporter = (builtin_reporter_t *)reporter;

    test_ignored_bookkeeping(breporter);
    print_char(breporter, 'I');
}

static void test_ignored_verbose(pfstest_reporter_t *reporter)
{
    builtin_reporter_t *breporter = (builtin_reporter_t *)reporter;
    pfstest_report_colorizer_t *colorizer = breporter->colorizer;
    int (*char_writer)(int) = reporter->char_writer;

    test_ignored_bookkeeping(breporter);
    report_colorizer_start(colorizer, char_writer, REPORT_COLOR_YELLOW);
    print_pg_str(breporter, pfstest_pg_str("IGNORED"));
    report_colorizer_reset(colorizer, char_writer);
    print_pg_str(breporter, pfstest_pg_str("\n"));
}

static void test_failed_message_start_common(
    builtin_reporter_t *breporter, const pfstest_pg_ptr char *file, int line)
{
    pfstest_reporter_t *reporter = (pfstest_reporter_t *)breporter;
    pfstest_report_colorizer_t *colorizer = breporter->colorizer;
    int(*char_writer)(int) = reporter->char_writer;

    report_colorizer_start(colorizer, char_writer, REPORT_COLOR_RED);
    print_pg_str(breporter, pfstest_pg_str("FAIL"));
    report_colorizer_reset(colorizer, char_writer);

    print_pg_str(breporter, pfstest_pg_str("\n"));
    print_pg_str(breporter, pfstest_pg_str("    Location: "));

    report_colorizer_start(colorizer, char_writer, REPORT_COLOR_BOLD);
    print_pg_str(breporter, file);
    print_pg_str(breporter, pfstest_pg_str(":"));
    print_int(breporter, line);
    report_colorizer_reset(colorizer, char_writer);

    print_pg_str(breporter, pfstest_pg_str("\n"));

    breporter->indent = pfstest_true;
}

static void test_failed_message_start_standard(
    pfstest_reporter_t *reporter, const pfstest_pg_ptr char *file, int line)
{
    builtin_reporter_t *breporter = (builtin_reporter_t *)reporter;

    print_context(breporter);
    test_failed_message_start_common(breporter, file, line);
}

static void test_failed_message_start_verbose(
    pfstest_reporter_t *reporter, const pfstest_pg_ptr char *file, int line)
{
    builtin_reporter_t *breporter = (builtin_reporter_t *)reporter;

    if (breporter->test_failed)
        print_context(breporter);

    test_failed_message_start_common(breporter, file, line);
}

static void test_failed_message_complete(pfstest_reporter_t *reporter)
{

    builtin_reporter_t *breporter = (builtin_reporter_t *)reporter;

    if (!breporter->test_failed) {
        breporter->results.failed++;
        breporter->test_failed = pfstest_true;
    }
    print_pg_str(breporter, pfstest_pg_str("\n"));
    breporter->indent = pfstest_false;
}

static pfstest_bool test_passed(builtin_reporter_t *breporter)
{
    return (!breporter->test_ignored && !breporter->test_failed);
}

static void test_passed_bookkeeping(builtin_reporter_t *breporter)
{
    breporter->results.passed++;
}

static void test_complete_standard(pfstest_reporter_t *reporter)
{
    builtin_reporter_t *breporter = (builtin_reporter_t *)reporter;

    if (test_passed(breporter)) {
        test_passed_bookkeeping(breporter);
        print_char(breporter, '.');
    }
}

static void test_complete_verbose(pfstest_reporter_t *reporter)
{
    builtin_reporter_t *breporter = (builtin_reporter_t *)reporter;
    pfstest_report_colorizer_t *colorizer = breporter->colorizer;
    int (*char_writer)(int) = reporter->char_writer;

    if (test_passed(breporter)) {
        test_passed_bookkeeping(breporter);
        report_colorizer_start(colorizer, char_writer, REPORT_COLOR_GREEN);
        print_pg_str(breporter, pfstest_pg_str("PASS"));
        report_colorizer_reset(colorizer, char_writer);
        print_pg_str(breporter, pfstest_pg_str("\n"));
    }
}

static void run_complete(pfstest_reporter_t *reporter)
{
    builtin_reporter_t *breporter = (builtin_reporter_t *)reporter;
    pfstest_report_colorizer_t *colorizer = breporter->colorizer;
    pfstest_bool pass_green = (breporter->results.passed > 0
                               && breporter->results.failed == 0);
    pfstest_bool fail_red = (breporter->results.failed > 0);
    pfstest_bool ignore_yellow = (breporter->results.ignored > 0);

    print_pg_str(breporter, pfstest_pg_str("\nRun complete. "));

    if (pass_green)
    {
        report_colorizer_start(colorizer, reporter->char_writer,
                               REPORT_COLOR_GREEN);
    }
    print_int(breporter, breporter->results.passed);
    print_pg_str(breporter, pfstest_pg_str(" passed"));
    if (pass_green) {
        report_colorizer_reset(colorizer, reporter->char_writer);
    }

    print_pg_str(breporter, pfstest_pg_str(", "));
    if (fail_red) {
        report_colorizer_start(colorizer, reporter->char_writer,
                               REPORT_COLOR_VERY_RED);
    }
    print_int(breporter, breporter->results.failed);
    print_pg_str(breporter, pfstest_pg_str(" failed"));
    if (fail_red) {
        report_colorizer_reset(colorizer, reporter->char_writer);
    }
    print_pg_str(breporter, pfstest_pg_str(", "));

    if (ignore_yellow) {
        report_colorizer_start(colorizer, reporter->char_writer,
                               REPORT_COLOR_YELLOW);
    }
    print_int(breporter, breporter->results.ignored);
    print_pg_str(breporter, pfstest_pg_str(" ignored"));
    if (ignore_yellow) {
        report_colorizer_reset(colorizer, reporter->char_writer);
    }

    print_pg_str(breporter, pfstest_pg_str("\n"));
}

static int return_value(pfstest_reporter_t *reporter)
{
    builtin_reporter_t *breporter = (builtin_reporter_t *)reporter;

    if (breporter->results.failed == 0)
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}

static const pfstest_pg pfstest_reporter_vtable_t standard_vtable = {
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

static const pfstest_pg pfstest_reporter_vtable_t verbose_vtable = {
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

static void bookkeeping_init(builtin_reporter_t *breporter)
{
    breporter->fresh_line = pfstest_true;
    breporter->indent = pfstest_false;
}


pfstest_reporter_t *pfstest_reporter_standard_new(
    int (*char_writer)(int), pfstest_report_colorizer_t *colorizer)
{
    builtin_reporter_t *breporter = pfstest_alloc(sizeof(*breporter));

    breporter->parent.vtable = &standard_vtable;
    breporter->parent.char_writer = char_writer;
    breporter->colorizer = colorizer;
    bookkeeping_init(breporter);

    return (pfstest_reporter_t *)breporter;
}

pfstest_reporter_t *pfstest_reporter_verbose_new(
    int (*char_writer)(int), pfstest_report_colorizer_t *colorizer)
{
    builtin_reporter_t *breporter = pfstest_alloc(sizeof(*breporter));

    breporter->parent.vtable = &verbose_vtable;
    breporter->parent.char_writer = char_writer;
    breporter->colorizer = colorizer;
    bookkeeping_init(breporter);

    return (pfstest_reporter_t *)breporter;
}

struct _pfstest_report_colorizer_t
{
    void (*start)(int (*char_writer)(int), report_color_t color);
    void (*reset)(int (*char_writer)(int));
};

static void colorizer_null_start(int (*char_writer)(int),
                                 report_color_t color)
{
    (void)char_writer;
    (void)color;
}

static void colorizer_null_reset(int (*char_writer)(int))
{
    (void)char_writer;
}

static void start_color_esc(int (*char_writer)(int))
{
    char_writer('\033');
    char_writer('[');
}

static void finish_color_esc(int (*char_writer)(int))
{
    char_writer('m');
}

static void colorizer_ansi_start(int (*char_writer)(int),
                                 report_color_t color)
{
    (void)color;
    start_color_esc(char_writer);
    if (color == REPORT_COLOR_BOLD) {
        char_writer('1');
    } else if (color == REPORT_COLOR_RED) {
        char_writer('3'); char_writer('1');
    } else if (color == REPORT_COLOR_GREEN) {
        char_writer('3'); char_writer('2');
    } else if (color == REPORT_COLOR_YELLOW) {
        char_writer('3'); char_writer('3');
    } else if (color == REPORT_COLOR_VERY_RED) {
        char_writer('3'); char_writer('7'); char_writer(';');
        char_writer('4'); char_writer('1'); char_writer(';');
        char_writer('1');
    }
    finish_color_esc(char_writer);
}

static void colorizer_ansi_reset(int (*char_writer)(int))
{
    start_color_esc(char_writer);
    finish_color_esc(char_writer);
}

static void report_colorizer_start(pfstest_report_colorizer_t *this,
                                   int (*char_writer)(int),
                                   report_color_t color)
{
    this->start(char_writer, color);
}

static void report_colorizer_reset(pfstest_report_colorizer_t *this,
                                   int (*char_writer)(int))
{
    this->reset(char_writer);
}

static pfstest_report_colorizer_t _pfstest_report_colorizer_null[] = {
    {
        colorizer_null_start,
        colorizer_null_reset,
    }
};

static pfstest_report_colorizer_t _pfstest_report_colorizer_ansi[] = {
    {
        colorizer_ansi_start,
        colorizer_ansi_reset,
    }
};

pfstest_report_colorizer_t *pfstest_report_colorizer_null =
    _pfstest_report_colorizer_null;
pfstest_report_colorizer_t *pfstest_report_colorizer_ansi =
    _pfstest_report_colorizer_ansi;
