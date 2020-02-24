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

test(close_to_float_should_printf_itself)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("a float within 0.01 of 1.23457");

    pfstest_matcher_print(close_to_float(1.23456789f, 0.01f), message_spy);

    assert_that("close_to_float prints its expectation and tolerance",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(close_to_float_should_always_printf_positive_tolerance)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("a float within 0.01 of 1.23457");

    pfstest_matcher_print(close_to_float(1.23456789f, -0.01f),
                          message_spy);

    assert_that("close_to_float prints its expectation and tolerance",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(nearly_float_should_printf_itself)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("a float within 1e-06 of 1.23457");

    pfstest_matcher_print(nearly_float(1.23456789f), message_spy);

    assert_that("nearly_float prints its expectation and tolerance",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(the_double_should_printf_itself)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("the double 12.3457");

    pfstest_value_print(the_double(12.3456789f), message_spy);

    assert_that("doubles printf themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(close_to_double_should_printf_itself)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("a double within 0.01 of 1.23457");

    pfstest_matcher_print(close_to_double(1.23456789f, 0.01f), message_spy);

    assert_that("close_to_double prints its expectation and tolerance",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(close_to_double_should_always_printf_positive_tolerance)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("a double within 0.01 of 1.23457");

    pfstest_matcher_print(close_to_double(1.23456789f, -0.01f),
                          message_spy);

    assert_that("close_to_double prints its expectation and tolerance",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(nearly_double_should_printf_itself)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("a double within 1e-12 of 1.23457");

    pfstest_matcher_print(nearly_double(1.23456789f), message_spy);

    assert_that("nearly_double prints its expectation and tolerance",
                the_string(captured_output), matches_the_pg_string(expected));
}
