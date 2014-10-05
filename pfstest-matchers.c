#include "pfstest-matchers.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pfstest-values.h"

static void is_the_whatever_printer(pfstest_matcher_t *matcher)
{
    pfstest_value_t *expected =
        (pfstest_value_t *)pfstest_matcher_data(matcher);

    printf("= ");
    pfstest_value_print(expected);
}

/* is_the_int */

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
    return pfstest_matcher_new(is_the_whatever_printer,
                               is_the_int_test,
                               the_int(i));
}

/* is_the_char */

static bool is_the_char_test(pfstest_matcher_t *matcher,
                             pfstest_value_t *actual)
{
    pfstest_value_t *expected =
        (pfstest_value_t *)pfstest_matcher_data(matcher);
    char expected_char = *(char *)pfstest_value_data(expected);
    char actual_char = *(char *)pfstest_value_data(actual);

    return (expected_char == actual_char);
}

pfstest_matcher_t *is_the_char(char c)
{
    return pfstest_matcher_new(is_the_whatever_printer,
                               is_the_char_test,
                               the_char(c));
}

/* is_the_string */

static bool is_the_string_test(pfstest_matcher_t *matcher,
                               pfstest_value_t *actual)
{
    pfstest_value_t *expected =
        (pfstest_value_t *)pfstest_matcher_data(matcher);
    char *expected_string = pfstest_value_data(expected);
    char *actual_string = pfstest_value_data(actual);

    return (0 == strcmp(expected_string, actual_string));
}

pfstest_matcher_t *is_the_string(char *s)
{
    return pfstest_matcher_new(is_the_whatever_printer,
                               is_the_string_test,
                               the_string(s));
}
