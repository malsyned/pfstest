#include "pfstest-matchers.h"

#include <stdlib.h>
#include <stdio.h>

#include "pfstest-values.h"

static void is_the_int_printer(pfstest_matcher_t *matcher)
{
    pfstest_value_t *expected =
        (pfstest_value_t *)pfstest_matcher_data(matcher);

    printf("= ");
    pfstest_value_print(expected);
}

static bool is_the_int_test(pfstest_matcher_t *matcher,
                            pfstest_value_t *actual)
{
    pfstest_value_t *expected =
        (pfstest_value_t *)pfstest_matcher_data(matcher);
    intmax_t expected_int = *(intmax_t *)pfstest_value_data(expected);
    intmax_t actual_int = *(intmax_t *)pfstest_value_data(actual);

    return (expected_int == actual_int);
}

pfstest_matcher_t *is_the_int(intmax_t i)
{
    return pfstest_matcher_new(is_the_int_printer,
                               is_the_int_test,
                               the_int(i));
}
