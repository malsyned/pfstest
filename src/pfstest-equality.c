#include "pfstest-equality.h"

void pfstest_equality_printer(pfstest_output_formatter_t *formatter,
                              pfstest_matcher_t *matcher)
{
    pfstest_value_t *expected =
        (pfstest_value_t *)pfstest_matcher_data(matcher);

    pfstest_value_print(formatter, expected);
}

bool pfstest_equality_test(pfstest_matcher_t *matcher,
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

