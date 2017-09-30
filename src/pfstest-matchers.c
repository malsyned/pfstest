#include "pfstest-matchers.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pfstest-platform.h"
#include "pfstest-values.h"
#include "pfstest-alloc.h"

static void is_the_whatever_printer(pfstest_output_formatter_t *formatter,
                                    pfstest_matcher_t *matcher)
{
    pfstest_value_t *expected =
        (pfstest_value_t *)pfstest_matcher_data(matcher);

    pfstest_value_print(formatter, expected);
}

static bool is_the_whatever_test(pfstest_matcher_t *matcher,
                                 pfstest_value_t *actual_value)
{
    pfstest_value_t *expected_value =
        (pfstest_value_t *)pfstest_matcher_data(matcher);

    const void *expected = pfstest_value_data(expected_value);
    size_t expected_size = pfstest_value_size(expected_value);

    const void *actual = pfstest_value_data(actual_value);
    size_t actual_size = pfstest_value_size(actual_value);

    /* If someone is comparing two different-sized blocks of memory in
     * an assertion, that's almost certainly a bug in the tests, not
     * in the production code. Since we're carrying around the size
     * for printing purposes anyway, we might as well check for and
     * catch this mistake. */
    if (expected_size != actual_size)
    {
        /* However, if actual_size is 0, someone is comparing a memory
         * block with a value created with something like
         * the_pointer(). We allow this, especially because auto-mocks
         * are dumb and use the_pointer for everything. The trade-off
         * for this is that the_pointer_printer will print only the
         * address of the actual value. mock verifiers should never
         * print argument values for anything anyway, because the data
         * they'd be printing might be stack data that's gone out of
         * scope by the time the verifier is run. */
        if (actual_size != 0)
            return false;
    }

    return (0 == memcmp(expected, actual, expected_size));
}

/* is_the_short */

pfstest_matcher_t *pfstest_is_the_short(short s)
{
    return pfstest_matcher_new(is_the_whatever_printer,
                               is_the_whatever_test,
                               the_short(s));
}

/* is_the_ushort */

pfstest_matcher_t *pfstest_is_the_ushort(unsigned short s)
{
    return pfstest_matcher_new(is_the_whatever_printer,
                               is_the_whatever_test,
                               the_ushort(s));
}

/* is_the_int */

pfstest_matcher_t *pfstest_is_the_int(int i)
{
    return pfstest_matcher_new(is_the_whatever_printer,
                               is_the_whatever_test,
                               the_int(i));
}

/* is_the_uint */

pfstest_matcher_t *pfstest_is_the_uint(unsigned int u)
{
    return pfstest_matcher_new(is_the_whatever_printer,
                               is_the_whatever_test,
                               the_uint(u));
}

/* is_the_bool */

pfstest_matcher_t *pfstest_is_the_bool(bool b)
{
    return pfstest_matcher_new(is_the_whatever_printer,
                               is_the_whatever_test,
                               the_bool(b));
}

/* is_the_char */

pfstest_matcher_t *pfstest_is_the_char(char c)
{
    return pfstest_matcher_new(is_the_whatever_printer,
                               is_the_whatever_test,
                               the_char(c));
}

/* is_the_string */

pfstest_matcher_t *pfstest_is_the_string(const char *s)
{
    return pfstest_matcher_new(is_the_whatever_printer,
                               is_the_whatever_test,
                               the_string(s));
}

/* is_the_pointer */

static bool is_the_pointer_test(pfstest_matcher_t *matcher,
                                pfstest_value_t *actual)
{
    pfstest_value_t *expected =
        (pfstest_value_t *)pfstest_matcher_data(matcher);
    const void *expected_pointer = pfstest_value_data(expected);
    const void *actual_pointer = pfstest_value_data(actual);

    return (expected_pointer == actual_pointer);
}

pfstest_matcher_t *pfstest_is_the_pointer(const void *p)
{
    return pfstest_matcher_new(is_the_whatever_printer,
                               is_the_pointer_test,
                               the_pointer(p));
}

/* is_the_memory */

pfstest_matcher_t *pfstest_is_the_memory(const void *m, size_t size)
{
    return pfstest_matcher_new(is_the_whatever_printer,
                               is_the_whatever_test,
                               the_memory(m, size));
}

/* matches_the_nv_string */

static void matches_the_nv_string_printer(
    pfstest_output_formatter_t *formatter, pfstest_matcher_t *matcher)
{
    const pfstest_nv_ptr char **sp =
        (const pfstest_nv_ptr char **)pfstest_matcher_data(matcher);
    const pfstest_nv_ptr char *expected = *sp;
    char c;

    pfstest_output_formatter_print_nv_string(
        formatter, pfstest_nv_string("the string \""));
    
    while (pfstest_memcpy_nv(&c, expected, sizeof(c)), c) {
        pfstest_output_formatter_print_escaped_char(formatter, c);
        expected++;
    }

    pfstest_output_formatter_print_nv_string(
        formatter, pfstest_nv_string("\""));
}

static bool matches_the_nv_string_test(pfstest_matcher_t *matcher,
                                       pfstest_value_t *actual_value)
{
    const pfstest_nv_ptr char **sp =
        (const pfstest_nv_ptr char **)pfstest_matcher_data(matcher);
    const pfstest_nv_ptr char *expected = *sp;

    const char *actual = (const char *)pfstest_value_data(actual_value);

    return (0 == pfstest_strcmp_nv(actual, expected));
}

pfstest_matcher_t *pfstest_matches_the_nv_string(
    const pfstest_nv_ptr char *s)
{
    const pfstest_nv_ptr char **sp = pfstest_alloc(sizeof(*sp));
    *sp = s;

    /* The cast to (void *) in the third argument is to work around a
     * compiler bug in VC++:

     * https://connect.microsoft.com/VisualStudio/feedback/details/3111046/c-compiler-incorrectly-emits-c4090-warning
     */
    return pfstest_matcher_new(matches_the_nv_string_printer,
                               matches_the_nv_string_test,
                               (void *)sp);
}
