#include "pfstest-assert.h"

#include "pfstest-core.h"

#include <stdio.h>

struct assert_that_args
{
    const pfstest_pg_ptr char *file;
    int line;
    const pfstest_pg_ptr char *message;
    pfstest_value_t *actual;
    pfstest_matcher_t *matcher;
};

static pfstest_bool pg_str_empty(const pfstest_pg_ptr char *str)
{
    return (0 == pfstest_strcmp_pgpg(str, pfstest_pg_string("")));
}

static void assert_that_printer(pfstest_reporter_t *reporter,
                                const void *data)
{
    const struct assert_that_args *args = data;

    pfstest_reporter_print_pg_string(
        reporter, pfstest_pg_string("Failed assertion"));
    if ((args->message != NULL) && !pg_str_empty(args->message))
    {
        pfstest_reporter_print_pg_string(reporter, pfstest_pg_string(": "));
        pfstest_reporter_print_pg_string(reporter, args->message);
    }
    pfstest_reporter_print_pg_string(reporter, pfstest_pg_string("\n"));
    pfstest_reporter_print_pg_string(
        reporter, pfstest_pg_string("Expected: "));
    pfstest_matcher_print(reporter, args->matcher);
    pfstest_reporter_print_pg_string(reporter, pfstest_pg_string("\n"));
    pfstest_reporter_print_pg_string(
        reporter, pfstest_pg_string("Actual:   "));
    pfstest_value_print(reporter, args->actual);
}

void _pfstest_assert_that_at_location(const pfstest_pg_ptr char *file,
                                      int line,
                                      const pfstest_pg_ptr char *message,
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
