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

static void assert_that_printer(const void *data)
{
    const struct assert_that_args *args = data;

    pfstest_print_nv_string(pfstest_nv_string("    Failed assertion"));
    if (args->message != NULL) {
        pfstest_print_nv_string(pfstest_nv_string(": "));
        pfstest_print_nv_string(args->message);
        pfstest_print_nv_string(pfstest_nv_string(" "));
    }
    pfstest_print_nv_string(pfstest_nv_string("\n"));
    pfstest_print_nv_string(pfstest_nv_string("    Expected: "));
    pfstest_matcher_print(args->matcher);
    pfstest_print_nv_string(pfstest_nv_string("\n"));
    pfstest_print_nv_string(pfstest_nv_string("    Actual: "));
    pfstest_value_print(args->actual);
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
