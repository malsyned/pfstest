#include "pfstest-assert.h"

#include "pfstest-core.h"

#include <stdio.h>

struct assert_that_data
{
    const char *file;
    int line;
    const char *message;
    pfstest_value_t *actual;
    pfstest_matcher_t *matcher;
};

static void assert_that_printer(const void *data)
{
    const struct assert_that_data *this = data;

    printf("    Failed assertion: %s (%s:%d)\n",
           this->message, this->file, this->line);
    printf("            Expected: ");
    pfstest_matcher_print(this->matcher);
    printf("\n");
    printf("              Actual: ");
    pfstest_value_print(this->actual);
}

void _pfstest_assert_that(const char *file,
                          int line,
                          const char *message,
                          pfstest_value_t *actual,
                          pfstest_matcher_t *matcher)
{
    struct assert_that_data data;
    data.message = message;
    data.file = file;
    data.line = line;
    data.actual = actual;
    data.matcher = matcher;

    if (!pfstest_matcher_matches(matcher, actual)) {
        pfstest_fail_with_printer(assert_that_printer, &data);
    }
}
