#include "pfstest-reporter.h"

#include "pfstest-alloc.h"
#include "pfstest-basename.h"
#include "pfstest-version.h"

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
    volatile bool test_failed;
    bool test_ignored;
    bool fresh_line;
    bool indent;
    const pfstest_nv_ptr char *test_name;
    const pfstest_nv_ptr char *test_file;
} builtin_reporter_t;

static int print_char(builtin_reporter_t *reporter, int c)
{
    pfstest_reporter_t *as_reporter = (pfstest_reporter_t *)reporter;
    int i, r;

    if (reporter->indent && reporter->fresh_line && c != '\n') {
        for (i = 0; i < 4; i++) {
            r = as_reporter->char_writer(' ');
            if (r != ' ')
                return r;
        }
    }
    reporter->fresh_line = (c == '\n');
    return as_reporter->char_writer(c);
}

static void print_nv_string(builtin_reporter_t *reporter,
                            const pfstest_nv_ptr char *s)
{
    pfstest_reporter_print_nv_string((pfstest_reporter_t *)reporter, s);
}

static void get_fresh_line(builtin_reporter_t *reporter)
{
    if (!reporter->fresh_line) {
        print_char(reporter, '\n');
    }
}

static int builtin_print_char(pfstest_reporter_t *reporter, int c)
{
    return print_char((builtin_reporter_t *)reporter, c);
}

static void print_int(builtin_reporter_t *reporter, intmax_t n)
{
    pfstest_reporter_print_int((pfstest_reporter_t *)reporter, n);
}

static void print_context(builtin_reporter_t *reporter)
{
    get_fresh_line(reporter);
    print_nv_string(reporter, pfstest_basename(reporter->test_file));
    print_nv_string(reporter, pfstest_nv_string(":"));
    print_nv_string(reporter, reporter->test_name);
    print_nv_string(reporter, pfstest_nv_string(" "));
}

static void run_started_bookkeeping(builtin_reporter_t *reporter)
{
    reporter->results.passed = 0;
    reporter->results.failed = 0;
    reporter->results.ignored = 0;
}

static void run_started(pfstest_reporter_t *reporter)
{
    builtin_reporter_t *as_builtin_reporter = (builtin_reporter_t *)reporter;

    run_started_bookkeeping(as_builtin_reporter);
    print_nv_string(
        as_builtin_reporter,
        pfstest_nv_string("PFSTest " PFSTEST_VERSION_STRING "\n"));
    print_nv_string(as_builtin_reporter,
                    pfstest_nv_string("===========\n"));
}

static void test_started_bookkeeping(builtin_reporter_t *reporter,
                                     const pfstest_nv_ptr char *test_name,
                                     const pfstest_nv_ptr char *test_file)
{
    reporter->test_failed = false;
    reporter->test_ignored = false;
    reporter->test_name = test_name;
    reporter->test_file = test_file;
}

static void test_started_standard(pfstest_reporter_t *reporter,
                                  const pfstest_nv_ptr char *test_name,
                                  const pfstest_nv_ptr char *test_file)
{
    builtin_reporter_t *as_builtin_reporter = (builtin_reporter_t *)reporter;

    test_started_bookkeeping(as_builtin_reporter, test_name, test_file);
}

static void test_started_verbose(pfstest_reporter_t *reporter,
                                 const pfstest_nv_ptr char *test_name,
                                 const pfstest_nv_ptr char *test_file)
{
    builtin_reporter_t *as_builtin_reporter = (builtin_reporter_t *)reporter;

    test_started_bookkeeping(as_builtin_reporter, test_name, test_file);
    print_context(as_builtin_reporter);
}

static void test_ignored_bookkeeping(builtin_reporter_t *reporter)
{
    reporter->test_ignored = true;
    reporter->results.ignored++;
}

static void test_ignored_standard(pfstest_reporter_t *reporter)
{
    builtin_reporter_t *as_builtin_reporter = (builtin_reporter_t *)reporter;

    test_ignored_bookkeeping(as_builtin_reporter);
    print_char(as_builtin_reporter, 'I');
}

static void test_ignored_verbose(pfstest_reporter_t *reporter)
{
    builtin_reporter_t *as_builtin_reporter = (builtin_reporter_t *)reporter;
    pfstest_report_colorizer_t *colorizer = as_builtin_reporter->colorizer;
    int (*char_writer)(int) = reporter->char_writer;

    test_ignored_bookkeeping(as_builtin_reporter);
    report_colorizer_start(colorizer, char_writer, REPORT_COLOR_YELLOW);
    print_nv_string(as_builtin_reporter, pfstest_nv_string("IGNORED"));
    report_colorizer_reset(colorizer, char_writer);
    print_nv_string(as_builtin_reporter, pfstest_nv_string("\n"));
}

static void test_failed_message_start_common(
    builtin_reporter_t *reporter, const pfstest_nv_ptr char *file, int line)
{
    pfstest_reporter_t *as_reporter = (pfstest_reporter_t *)reporter;
    pfstest_report_colorizer_t *colorizer = reporter->colorizer;
    int(*char_writer)(int) = as_reporter->char_writer;

    report_colorizer_start(colorizer, char_writer, REPORT_COLOR_RED);
    print_nv_string(reporter, pfstest_nv_string("FAIL"));
    report_colorizer_reset(colorizer, char_writer);

    print_nv_string(reporter, pfstest_nv_string("\n"));
    print_nv_string(reporter, pfstest_nv_string("    Location: "));

    report_colorizer_start(colorizer, char_writer, REPORT_COLOR_BOLD);
    print_nv_string(reporter, pfstest_basename(file));
    print_nv_string(reporter, pfstest_nv_string(":"));
    print_int(reporter, line);
    report_colorizer_reset(colorizer, char_writer);

    print_nv_string(reporter, pfstest_nv_string("\n"));

    reporter->indent = true;
}

static void test_failed_message_start_standard(
    pfstest_reporter_t *reporter, const pfstest_nv_ptr char *file, int line)
{
    builtin_reporter_t *as_builtin_reporter = (builtin_reporter_t *)reporter;

    print_context(as_builtin_reporter);
    test_failed_message_start_common(as_builtin_reporter, file, line);
}

static void test_failed_message_start_verbose(
    pfstest_reporter_t *reporter, const pfstest_nv_ptr char *file, int line)
{
    builtin_reporter_t *as_builtin_reporter = (builtin_reporter_t *)reporter;

    if (as_builtin_reporter->test_failed) {
        print_context(as_builtin_reporter);
    }

    test_failed_message_start_common(as_builtin_reporter, file, line);
}

static void test_failed_message_complete(pfstest_reporter_t *reporter)
{

    builtin_reporter_t *as_builtin_reporter = (builtin_reporter_t *)reporter;

    if (!as_builtin_reporter->test_failed) {
        as_builtin_reporter->results.failed++;
        as_builtin_reporter->test_failed = true;
    }
    print_nv_string(as_builtin_reporter, pfstest_nv_string("\n"));
    as_builtin_reporter->indent = false;
}

static bool test_passed(builtin_reporter_t *reporter)
{
    return (!reporter->test_ignored && !reporter->test_failed);
}

static void test_passed_bookkeeping(builtin_reporter_t *reporter)
{
    reporter->results.passed++;
}

static void test_complete_standard(pfstest_reporter_t *reporter)
{
    builtin_reporter_t *as_builtin_reporter = (builtin_reporter_t *)reporter;

    if (test_passed(as_builtin_reporter)) {
        test_passed_bookkeeping(as_builtin_reporter);
        print_char(as_builtin_reporter, '.');
    }
}

static void test_complete_verbose(pfstest_reporter_t *reporter)
{
    builtin_reporter_t *as_builtin_reporter = (builtin_reporter_t *)reporter;
    pfstest_report_colorizer_t *colorizer = as_builtin_reporter->colorizer;
    int (*char_writer)(int) = reporter->char_writer;

    if (test_passed(as_builtin_reporter)) {
        test_passed_bookkeeping(as_builtin_reporter);
        report_colorizer_start(colorizer, char_writer, REPORT_COLOR_GREEN);
        print_nv_string(as_builtin_reporter, pfstest_nv_string("PASS"));
        report_colorizer_reset(colorizer, char_writer);
        print_nv_string(as_builtin_reporter, pfstest_nv_string("\n"));
    }
}

static void run_complete(pfstest_reporter_t *reporter)
{
    builtin_reporter_t *as_builtin_reporter = (builtin_reporter_t *)reporter;
    pfstest_report_colorizer_t *colorizer = as_builtin_reporter->colorizer;
    bool pass_green = (as_builtin_reporter->results.passed > 0
                       && as_builtin_reporter->results.failed == 0);
    bool fail_red = (as_builtin_reporter->results.failed > 0);
    bool ignore_yellow = (as_builtin_reporter->results.ignored > 0);

    print_nv_string(as_builtin_reporter,
                    pfstest_nv_string("\nRun complete. "));

    if (pass_green)
    {
        report_colorizer_start(colorizer, reporter->char_writer,
                               REPORT_COLOR_GREEN);
    }
    print_int(as_builtin_reporter, as_builtin_reporter->results.passed);
    print_nv_string(as_builtin_reporter, pfstest_nv_string(" passed"));
    if (pass_green) {
        report_colorizer_reset(colorizer, reporter->char_writer);
    }

    print_nv_string(as_builtin_reporter, pfstest_nv_string(", "));
    if (fail_red) {
        report_colorizer_start(colorizer, reporter->char_writer,
                               REPORT_COLOR_VERY_RED);
    }
    print_int(as_builtin_reporter, as_builtin_reporter->results.failed);
    print_nv_string(as_builtin_reporter, pfstest_nv_string(" failed"));
    if (fail_red) {
        report_colorizer_reset(colorizer, reporter->char_writer);
    }
    print_nv_string(as_builtin_reporter, pfstest_nv_string(", "));

    if (ignore_yellow) {
        report_colorizer_start(colorizer, reporter->char_writer,
                               REPORT_COLOR_YELLOW);
    }
    print_int(as_builtin_reporter, as_builtin_reporter->results.ignored);
    print_nv_string(as_builtin_reporter, pfstest_nv_string(" ignored"));
    if (ignore_yellow) {
        report_colorizer_reset(colorizer, reporter->char_writer);
    }

    print_nv_string(as_builtin_reporter, pfstest_nv_string("\n"));
}

static int return_value(pfstest_reporter_t *reporter)
{
    builtin_reporter_t *as_builtin_reporter = (builtin_reporter_t *)reporter;

    return as_builtin_reporter->results.failed;
}

static const pfstest_nv pfstest_reporter_vtable_t standard_vtable = {
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

static const pfstest_nv pfstest_reporter_vtable_t verbose_vtable = {
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

static void bookkeeping_init(builtin_reporter_t *reporter)
{
    reporter->fresh_line = true;
    reporter->indent = false;
}


pfstest_reporter_t *pfstest_reporter_standard_new(
    int (*char_writer)(int), pfstest_report_colorizer_t *colorizer)
{
    builtin_reporter_t *reporter = pfstest_alloc(sizeof(*reporter));

    reporter->parent.vtable = &standard_vtable;
    reporter->parent.char_writer = char_writer;
    reporter->colorizer = colorizer;
    bookkeeping_init(reporter);

    return (pfstest_reporter_t *)reporter;
}

pfstest_reporter_t *pfstest_reporter_verbose_new(
    int (*char_writer)(int), pfstest_report_colorizer_t *colorizer)
{
    builtin_reporter_t *reporter = pfstest_alloc(sizeof(*reporter));

    reporter->parent.vtable = &verbose_vtable;
    reporter->parent.char_writer = char_writer;
    reporter->colorizer = colorizer;
    bookkeeping_init(reporter);

    return (pfstest_reporter_t *)reporter;
}

/* Message Output */

struct escape_char_map
{
    char ch;
    const pfstest_nv_ptr char *str;
};

static const pfstest_nv char esc_a[] = "\\a";
static const pfstest_nv char esc_b[] = "\\b";
static const pfstest_nv char esc_f[] = "\\f";
static const pfstest_nv char esc_n[] = "\\n";
static const pfstest_nv char esc_r[] = "\\r";
static const pfstest_nv char esc_t[] = "\\t";
static const pfstest_nv char esc_v[] = "\\v";
static const pfstest_nv char esc_backslash[] = "\\\\";
static const pfstest_nv char esc_apostrophe[] = "\\'";
static const pfstest_nv char esc_quote[] = "\\\"";
static const pfstest_nv char esc_question[] = "\\?";

static struct escape_char_map escape_char_map[] =
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

    for (i = 0; i < sizeof(escape_char_map)/sizeof(escape_char_map[0]); i++)
    {
        if (c == escape_char_map[i].ch) {
            pfstest_reporter_print_nv_string(
                reporter, escape_char_map[i].str);
            return;
        }
    }
    pfstest_reporter_print_char(reporter, c);
}

void pfstest_reporter_print_nv_string(
    pfstest_reporter_t *reporter, const pfstest_nv_ptr char *s)
{
    char c;

    while (pfstest_memcpy_nv(&c, s, sizeof(c)), c) {
        pfstest_reporter_print_char(reporter, c);
        s++;
    }
}

void pfstest_reporter_print_int(pfstest_reporter_t *reporter, intmax_t n)
{
    uintmax_t nabs;

    if (n < 0) {
        pfstest_reporter_print_char(reporter, '-');
        /* Casting a negative number to an unsigned type is guaranteed
         * to result in that number, modulo MAX+1. (C Standard
         * 6.3.1.3.2) */
        nabs = UINTMAX_MAX - (uintmax_t)n + 1;
    } else {
        nabs = (uintmax_t)n;
    }

    pfstest_reporter_print_uint(reporter, nabs, 10, 0);
}

static const pfstest_nv char digits[] = "0123456789abcdef";

static char digit_char(unsigned int digit)
{
    char d;

    assert(digit <= 0xf);

    pfstest_memcpy_nv(&d, &digits[digit], sizeof(d));

    return d;
}

void pfstest_reporter_print_uint(
    pfstest_reporter_t *reporter, uintmax_t n, unsigned int base, int zpad)
{
    uintmax_t d = 1;
    int digits = 1;

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
    const pfstest_nv_ptr void *print_char_rom =
        &(reporter->vtable->print_char);
    pfstest_memcpy_nv(&print_char, print_char_rom,
                      sizeof(print_char));

    return print_char(reporter, c);
}

void pfstest_reporter_run_started(pfstest_reporter_t *reporter)
{
    void (*run_started)(pfstest_reporter_t *);
    const pfstest_nv_ptr void *run_started_rom =
        &(reporter->vtable->run_started);
    pfstest_memcpy_nv(&run_started, run_started_rom,
                      sizeof(run_started));

    run_started(reporter);
}

void pfstest_reporter_test_started(pfstest_reporter_t *reporter,
                                   const pfstest_nv_ptr char *test_name,
                                   const pfstest_nv_ptr char *test_file)
{
    void (*test_started)(pfstest_reporter_t *,
                         const pfstest_nv_ptr char *,
                         const pfstest_nv_ptr char *);
    const pfstest_nv_ptr void *test_started_rom =
        &reporter->vtable->test_started;
    pfstest_memcpy_nv(&test_started, test_started_rom,
                      sizeof(test_started));

    test_started(reporter, test_name, test_file);
}

void pfstest_reporter_test_ignored(pfstest_reporter_t *reporter)
{
    void (*test_ignored)(pfstest_reporter_t *);
    const pfstest_nv_ptr void *test_ignored_rom =
        &reporter->vtable->test_ignored;
    pfstest_memcpy_nv(&test_ignored, test_ignored_rom,
                      sizeof(test_ignored));

    test_ignored(reporter);
}

void pfstest_reporter_test_failed_message_start(
    pfstest_reporter_t *reporter, const pfstest_nv_ptr char *file, int line)
{
    void (*test_failed_message_start)(pfstest_reporter_t *,
                                      const pfstest_nv_ptr char *,
                                      int);
    const pfstest_nv_ptr void *test_failed_message_start_rom =
        &reporter->vtable->test_failed_message_start;
    pfstest_memcpy_nv(&test_failed_message_start,
                      test_failed_message_start_rom,
                      sizeof(test_failed_message_start));
    
    test_failed_message_start(reporter, file, line);
}

void pfstest_reporter_test_failed_message_complete(
    pfstest_reporter_t *reporter)
{
    void (*test_failed_message_complete)(pfstest_reporter_t *);
    const pfstest_nv_ptr void *test_failed_message_complete_rom =
        &reporter->vtable->test_failed_message_complete;
    pfstest_memcpy_nv(&test_failed_message_complete,
                      test_failed_message_complete_rom,
                      sizeof(test_failed_message_complete));
        
    test_failed_message_complete(reporter);
}

void pfstest_reporter_test_complete(pfstest_reporter_t *reporter)
{
    void (*test_complete)(pfstest_reporter_t *);
    const pfstest_nv_ptr void *test_complete_rom =
        &reporter->vtable->test_complete;
    pfstest_memcpy_nv(&test_complete, test_complete_rom,
                      sizeof(test_complete));
        
    test_complete(reporter);
}

void pfstest_reporter_run_complete(pfstest_reporter_t *reporter)
{
    void (*run_complete)(pfstest_reporter_t *);
    const pfstest_nv_ptr void *run_complete_rom =
        &reporter->vtable->run_complete;
    pfstest_memcpy_nv(&run_complete, run_complete_rom,
                      sizeof(run_complete));
        
    run_complete(reporter);
}

int pfstest_reporter_return_value(pfstest_reporter_t *reporter)
{
    int (*return_value)(pfstest_reporter_t *);
    const pfstest_nv_ptr void *return_value_rom =
        &reporter->vtable->return_value;
    pfstest_memcpy_nv(&return_value, return_value_rom,
                      sizeof(return_value));
        
    return return_value(reporter);
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
