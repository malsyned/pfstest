#include <math.h>

#include "pfstest.h"
#include "capture-output.h"

setup()
{
    capture_output_init();
}

test(the_float_should_print_a_description)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("a float");

    pfstest_value_print(the_float(1.23456789f), message_spy);

    assert_that("floats print a description of their type",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(is_a_float_within_should_print_a_description)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("a float near the expected value");

    pfstest_matcher_print(is_a_float_within(1.23456789f, 0.01f), message_spy);

    assert_that("is_a_float_within prints a description",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(nearby_floats_should_match)
{
    assert_true("nearby floats match",
                pfstest_matcher_matches(is_a_float_within(1.0f, 0.1f),
                                        the_float(1.09f)));
}

test(floats_too_high_should_not_match)
{
    assert_false("floats above tolerance don't match",
                 pfstest_matcher_matches(is_a_float_within(1.0f, 0.1f),
                                         the_float(1.11f)));
}

test(floats_too_low_should_not_match)
{
    assert_false("floats below tolerance don't match",
                 pfstest_matcher_matches(is_a_float_within(1.0f, 0.1f),
                                         the_float(0.89f)));
}

test(a_float_within_should_handle_negative_tolerance)
{
    assert_true("nearby floats match",
                pfstest_matcher_matches(is_a_float_within(1.0f, -0.1f),
                                        the_float(1.09f)));
}

test(float_inf_is_within_inf_of_expected)
{
    assert_that("infinity falls within infinite tolerance",
                the_float(1.0f/0.0f), is_a_float_within(1.0f, 1.0f/0.0f));
}

test(is_float_inf_should_print_itself)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("the float inf");

    pfstest_matcher_print(is_float_inf, message_spy);

    assert_that("is_float_inf describes itself",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(is_float_inf_matches_infinity)
{
    assert_true("is_float_inf matches infinity",
                pfstest_matcher_matches(is_float_inf, the_float(1.0f/0.0f)));
}

test(is_float_inf_doesnt_match_numbers)
{
    assert_false("is_float_inf doesn't match numbers",
                 pfstest_matcher_matches(is_float_inf, the_float(1.0f)));
}

test(is_float_neg_inf_should_print_itself)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("the float -inf");

    pfstest_matcher_print(is_float_neg_inf, message_spy);

    assert_that("is_float_neg_inf describes itself",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(is_float_neg_inf_matches_negative_infinity)
{
    assert_true("is_float_neg_inf matches infinity",
                pfstest_matcher_matches(is_float_neg_inf,
                                        the_float(-1.0f/0.0f)));
}

test(is_float_neg_inf_doesnt_match_numbers)
{
    assert_false("is_float_neg_inf doesn't match numbers",
                 pfstest_matcher_matches(is_float_neg_inf, the_float(1.0f)));
}

test(is_float_nan_should_print_itself)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("the float nan");

    pfstest_matcher_print(is_float_nan, message_spy);

    assert_that("is_float_nan describes itself",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(is_float_nan_matches_nan)
{
    assert_true("is_float_nan matches NaN",
                pfstest_matcher_matches(is_float_nan, the_float(0.0f/0.0f)));
}

test(is_float_nan_doesnt_match_numbers)
{
    assert_false("is_float_nan doesn't match numbers",
                 pfstest_matcher_matches(is_float_nan, the_float(1.0f)));
}
