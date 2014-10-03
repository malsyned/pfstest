#include "pfstest-assert.h"

#include "pfstest-core.h"

#include <stdio.h>

struct assert_that_args
{
    const char *file;
    int line;
    const char *message;
    pfstest_value_t *actual;
    pfstest_matcher_t *matcher;
};

static void assert_that_printer(const void *data)
{
    const struct assert_that_args *args = data;

    printf("    Failed assertion: ");
    if (args->message != NULL) {
        printf("%s ", args->message);
    }
    printf("(%s:%d)\n", args->file, args->line);
    printf("            Expected: ");
    pfstest_matcher_print(args->matcher);
    printf("\n");
    printf("              Actual: ");
    pfstest_value_print(args->actual);
}

static void null_value_failure_printer(const void *data)
{
    const struct assert_that_args *args = data;

    printf("    assert_that called with NULL value (%s:%d)",
           args->file, args->line);
}

static void null_matcher_failure_printer(const void *data)
{
    const struct assert_that_args *args = data;

    printf("    assert_that called with NULL matcher (%s:%d)",
           args->file, args->line);
}

void _pfstest_assert_that(const char *file,
                          int line,
                          const char *message,
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
        pfstest_fail_with_printer(null_value_failure_printer, &args);
    }
    if (matcher == NULL) {
        pfstest_fail_with_printer(null_matcher_failure_printer, &args);
    }

    if (!pfstest_matcher_matches(matcher, actual)) {
        pfstest_fail_with_printer(assert_that_printer, &args);
    }
}
