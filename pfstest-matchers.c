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

/* is_the_uint */

static bool is_the_uint_test(pfstest_matcher_t *matcher,
                             pfstest_value_t *actual)
{
    pfstest_value_t *expected =
        (pfstest_value_t *)pfstest_matcher_data(matcher);
    uintmax_t expected_uint = *(uintmax_t *)pfstest_value_data(expected);
    uintmax_t actual_uint = *(uintmax_t *)pfstest_value_data(actual);

    return (expected_uint == actual_uint);
}

pfstest_matcher_t *is_the_uint(uintmax_t u)
{
    return pfstest_matcher_new(is_the_whatever_printer,
                               is_the_uint_test,
                               the_uint(u));
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

/* is_the_pointer */

static bool is_the_pointer_test(pfstest_matcher_t *matcher,
                                pfstest_value_t *actual)
{
    pfstest_value_t *expected =
        (pfstest_value_t *)pfstest_matcher_data(matcher);
    void *expected_pointer = pfstest_value_data(expected);
    void *actual_pointer = pfstest_value_data(actual);

    return (expected_pointer == actual_pointer);
}

pfstest_matcher_t *is_the_pointer(void *p)
{
    return pfstest_matcher_new(is_the_whatever_printer,
                               is_the_pointer_test,
                               the_pointer(p));
}

/* is_the_memory */

static bool is_the_memory_test(pfstest_matcher_t *matcher,
                               pfstest_value_t *actual)
{
    pfstest_value_t *expected =
        (pfstest_value_t *)pfstest_matcher_data(matcher);

    void *expected_memory = pfstest_value_data(expected);
    size_t expected_memory_size = pfstest_value_size(expected);

    void *actual_memory = pfstest_value_data(actual);
    size_t actual_memory_size = pfstest_value_size(actual);

    /* If someone is comparing two different-sized blocks of memory in
     * an assertion, that's almost certainly a bug in the tests, not
     * in the production code. Since we're carrying around the size
     * for printing purposes anyway, we might as well check for and
     * catch this mistake. */
    if (expected_memory_size != actual_memory_size)
    {
        /* However, if actual_memory_size is 0, someone is comparing a
         * memory block with a value created with something like
         * the_pointer(). We allow this, especially because auto-mocks
         * are dumb and use the_pointer for everything. The trade-off
         * for this is that the_pointer_printer will print only the
         * address of the actual value. mock verifiers should never
         * print argument values for anything anyway, because the data
         * they'd be printing might be stack data that's gone out of
         * scope by the time the verifier is run. */
        if (actual_memory_size != 0)
            return false;
    }

    return (0 == memcmp(expected_memory,
                        actual_memory,
                        expected_memory_size));
}

pfstest_matcher_t *is_the_memory(void *m, size_t size)
{
    return pfstest_matcher_new(is_the_whatever_printer,
                               is_the_memory_test,
                               the_memory(m, size));
}
