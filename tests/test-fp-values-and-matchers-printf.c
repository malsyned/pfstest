#include "pfstest.h"
#include "capture-output.h"

setup()
{
    pfstest_fp_set_printer(pfstest_fp_printf_printer);
    capture_output_init();
}

teardown()
{
    pfstest_fp_set_printer(NULL);
}

test(the_float_should_printf_itself)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("the float 12.3457");

    pfstest_value_print(the_float(12.3456789f), message_spy);

    assert_that("floats printf themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(is_a_float_within_should_printf_itself)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("a float within 0.01 of 1.23457");

    pfstest_matcher_print(is_a_float_within(1.23456789f, 0.01f), message_spy);

    assert_that("is_a_float_within prints its expectation and tolerance",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(is_a_float_within_should_always_printf_positive_tolerance)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("a float within 0.01 of 1.23457");

    pfstest_matcher_print(is_a_float_within(1.23456789f, -0.01f),
                          message_spy);

    assert_that("is_a_float_within prints its expectation and tolerance",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(is_a_float_near_should_printf_itself)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("a float within 1e-06 of 1.23457");

    pfstest_matcher_print(is_a_float_within(1.23456789f, 1e-6f),
                          message_spy);

    assert_that("is_a_float_near prints its expectation and tolerance",
                the_string(captured_output), matches_the_pg_string(expected));
}
