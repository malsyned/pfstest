#include "pfstest-assert.h"

#include "pfstest-core.h"

#include <stdio.h>

struct assert_that_args
{
    const pfstest_nv_ptr char *file;
    int line;
    const pfstest_nv_ptr char *message;
    pfstest_value_t *actual;
    pfstest_matcher_t *matcher;
};

static pfstest_bool nv_str_empty(const pfstest_nv_ptr char *str)
{
    return (0 == pfstest_strcmp_nvnv(str, pfstest_nv_string("")));
}

static void assert_that_printer(pfstest_reporter_t *reporter,
                                const void *data)
{
    const struct assert_that_args *args = data;

    pfstest_reporter_print_nv_string(
        reporter, pfstest_nv_string("Failed assertion"));
    if ((args->message != NULL) && !nv_str_empty(args->message))
    {
        pfstest_reporter_print_nv_string(reporter, pfstest_nv_string(": "));
        pfstest_reporter_print_nv_string(reporter, args->message);
    }
    pfstest_reporter_print_nv_string(reporter, pfstest_nv_string("\n"));
    pfstest_reporter_print_nv_string(
        reporter, pfstest_nv_string("Expected: "));
    pfstest_matcher_print(reporter, args->matcher);
    pfstest_reporter_print_nv_string(reporter, pfstest_nv_string("\n"));
    pfstest_reporter_print_nv_string(
        reporter, pfstest_nv_string("Actual:   "));
    pfstest_value_print(reporter, args->actual);
}

void _pfstest_assert_that_at_location(const pfstest_nv_ptr char *file,
                                      int line,
                                      const pfstest_nv_ptr char *message,
                                      pfstest_value_t *actual,
                                      pfstest_matcher_t *matcher)
{
    struct assert_that_args args;
    args.message = message;
    args.file = file;
    args.line = line;
    args.actual = actual;
    args.matcher = matcher;

    if (actual == NULL) {
        pfstest_fail_at_location(file, line,
                                 "assert_that called with NULL value");
    }
    if (matcher == NULL) {
        pfstest_fail_at_location(file, line,
                                 "assert_that called with NULL matcher");
    }

    if (!pfstest_matcher_matches(matcher, actual)) {
        pfstest_fail_with_printer(file, line,
                                  assert_that_printer, (const void *)&args);
    }
}
