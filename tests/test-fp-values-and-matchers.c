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

test(close_to_float_should_print_a_description)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("a float near the expected value");

    pfstest_matcher_print(close_to_float(1.23456789f, 0.01f), message_spy);

    assert_that("is_a_float_within prints a description",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(nearby_floats_should_match)
{
    assert_true("nearby floats match",
                pfstest_matcher_matches(close_to_float(1.0f, 0.1f),
                                        the_float(1.09f)));
}

test(floats_too_high_should_not_match)
{
    assert_false("floats above tolerance don't match",
                 pfstest_matcher_matches(close_to_float(1.0f, 0.1f),
                                         the_float(1.11f)));
}

test(floats_too_low_should_not_match)
{
    assert_false("floats below tolerance don't match",
                 pfstest_matcher_matches(close_to_float(1.0f, 0.1f),
                                         the_float(0.89f)));
}

test(a_float_within_should_handle_negative_tolerance)
{
    assert_true("nearby floats match",
                pfstest_matcher_matches(close_to_float(1.0f, -0.1f),
                                        the_float(1.09f)));
}

test(float_inf_is_within_inf_of_expected)
{
    assert_that("infinity falls within infinite tolerance",
                the_float(1.0f/0.0f), close_to_float(1.0f, 1.0f/0.0f));
}

test(an_inf_float_should_print_itself)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("an inf float");

    pfstest_matcher_print(an_inf_float(), message_spy);

    assert_that("an_inf_float describes itself",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(an_inf_float_matches_infinity)
{
    assert_true("an_inf_float matches infinity",
                pfstest_matcher_matches(an_inf_float(), the_float(1.0f/0.0f)));
}

test(an_inf_float_doesnt_match_numbers)
{
    assert_false("an_inf_float doesn't match numbers",
                 pfstest_matcher_matches(an_inf_float(), the_float(1.0f)));
}

test(a_neg_inf_float_should_print_itself)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("a -inf float");

    pfstest_matcher_print(a_neg_inf_float(), message_spy);

    assert_that("a_neg_inf_float describes itself",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(a_neg_inf_float_matches_negative_infinity)
{
    assert_true("a_neg_inf_float matches infinity",
                pfstest_matcher_matches(a_neg_inf_float(),
                                        the_float(-1.0f/0.0f)));
}

test(a_neg_inf_float_doesnt_match_numbers)
{
    assert_false("a_neg_inf_float doesn't match numbers",
                 pfstest_matcher_matches(a_neg_inf_float(), the_float(1.0f)));
}

test(a_nan_float_should_print_itself)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("a nan float");

    pfstest_matcher_print(a_nan_float(), message_spy);

    assert_that("a_nan_float describes itself",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(a_nan_float_matches_nan)
{
    assert_true("a_nan_float matches NaN",
                pfstest_matcher_matches(a_nan_float(), the_float(0.0f/0.0f)));
}

test(a_nan_float_doesnt_match_numbers)
{
    assert_false("a_nan_float doesn't match numbers",
                 pfstest_matcher_matches(a_nan_float(), the_float(1.0f)));
}

test(the_double_should_print_a_description)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("a double");

    pfstest_value_print(the_double(1.23456789f), message_spy);

    assert_that("doubles print a description of their type",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(close_to_double_should_print_a_description)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("a double near the expected value");

    pfstest_matcher_print(close_to_double(1.23456789f, 0.01f), message_spy);

    assert_that("close_to_double prints a description",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(nearby_doubles_should_match)
{
    assert_true("nearby doubles match",
                pfstest_matcher_matches(close_to_double(1.0f, 0.1f),
                                        the_double(1.09f)));
}

test(doubles_too_high_should_not_match)
{
    assert_false("doubles above tolerance don't match",
                 pfstest_matcher_matches(close_to_double(1.0f, 0.1f),
                                         the_double(1.11f)));
}

test(doubles_too_low_should_not_match)
{
    assert_false("doubles below tolerance don't match",
                 pfstest_matcher_matches(close_to_double(1.0f, 0.1f),
                                         the_double(0.89f)));
}

test(a_double_within_should_handle_negative_tolerance)
{
    assert_true("nearby doubles match",
                pfstest_matcher_matches(close_to_double(1.0f, -0.1f),
                                        the_double(1.09f)));
}

test(double_inf_is_within_inf_of_expected)
{
    assert_that("infinity falls within infinite tolerance",
                the_double(1.0f/0.0f), close_to_double(1.0f, 1.0f/0.0f));
}

test(an_inf_double_should_print_itself)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("an inf double");

    pfstest_matcher_print(an_inf_double(), message_spy);

    assert_that("an_inf_double describes itself",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(an_inf_double_matches_infinity)
{
    assert_true("an_inf_double matches infinity",
                pfstest_matcher_matches(an_inf_double(), the_double(1.0f/0.0f)));
}

test(an_inf_double_doesnt_match_numbers)
{
    assert_false("an_inf_double doesn't match numbers",
                 pfstest_matcher_matches(an_inf_double(), the_double(1.0f)));
}

test(a_neg_inf_double_should_print_itself)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("a -inf double");

    pfstest_matcher_print(a_neg_inf_double(), message_spy);

    assert_that("a_neg_inf_double describes itself",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(a_neg_inf_double_matches_negative_infinity)
{
    assert_true("a_neg_inf_double matches infinity",
                pfstest_matcher_matches(a_neg_inf_double(),
                                        the_double(-1.0f/0.0f)));
}

test(a_neg_inf_double_doesnt_match_numbers)
{
    assert_false("a_neg_inf_double doesn't match numbers",
                 pfstest_matcher_matches(a_neg_inf_double(), the_double(1.0f)));
}

test(a_nan_double_should_print_itself)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("a nan double");

    pfstest_matcher_print(a_nan_double(), message_spy);

    assert_that("a_nan_double describes itself",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(a_nan_double_matches_nan)
{
    assert_true("a_nan_double matches NaN",
                pfstest_matcher_matches(a_nan_double(), the_double(0.0f/0.0f)));
}

test(a_nan_double_doesnt_match_numbers)
{
    assert_false("a_nan_double doesn't match numbers",
                 pfstest_matcher_matches(a_nan_double(), the_double(1.0f)));
}
