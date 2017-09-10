#include <stdlib.h>
#include <stdio.h>

#include "pfstest.h"
#include "capture-output.h"

static void some_value_printer(pfstest_output_formatter_t *formatter,
                               pfstest_value_t *value)
{
    pfstest_output_formatter_print_nv_string(
        formatter, pfstest_nv_string("some value"));
}

static pfstest_value_t some_value[1] = {{
        some_value_printer,
        NULL,
    }};

static bool always_return_true(pfstest_matcher_t *matcher,
                               pfstest_value_t *actual)
{
    return true;
}

static pfstest_matcher_t matches_anything[1] = {{
        NULL,
        always_return_true,
        NULL,
    }};

static bool always_return_false(pfstest_matcher_t *matcher,
                                pfstest_value_t *actual)
{
    return false;
}

static void nothing_printer(pfstest_output_formatter_t *formatter,
                            pfstest_matcher_t *matcher)
{
    pfstest_output_formatter_print_nv_string(
        formatter, pfstest_nv_string("nothing (guaranteed to fail)"));
}

static pfstest_matcher_t matches_nothing[1] = {{
        nothing_printer,
        always_return_false,
        NULL,
    }};

before_tests(set_up_assert_that)
{
    capture_output_init();
}

test(should_pass_assertion)
{
    assert_that("always passes", some_value, matches_anything);
}

pfstest_case(assert_always_fail)
{
    assert_that("always fails", some_value, matches_nothing);
}

test(should_fail_on_false_assertion)
{
    capture_test_results(assert_always_fail);

    assert_that("False assertions fail",
                the_int(pfstest_output_formatter_return_value(message_spy)),
                is_the_int(1));
}

test(should_print_explanation_on_failed_assertion)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "Failed assertion: always fails\n"
        "Expected: nothing (guaranteed to fail)\n"
        "Actual: some value");

    capture_test_results(assert_always_fail);

    assert_that("Failing assertions should print an explanatory message",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

/* avr-gcc throws a compiler error on NULL assert_that message
 * strings */
#if !(defined(__GNUC__) && defined(__AVR__))
pfstest_case(assert_null_string)
{
    /* This is a failing test so that the output can be inspected */
    assert_that(NULL, some_value, matches_nothing);
}

test(should_cope_with_null_string)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "Failed assertion\n"
        "Expected: nothing (guaranteed to fail)\n"
        "Actual: some value");

    capture_test_results(assert_null_string);

    assert_that("assert_that handles NULL strings",
                the_string(captured_output),
                matches_the_nv_string(expected));
}
#endif

pfstest_case(assert_empty_string)
{
    /* This is a failing test so that the output can be inspected */
    assert_that("", some_value, matches_nothing);
}

test(should_cope_with_empty_string)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "Failed assertion\n"
        "Expected: nothing (guaranteed to fail)\n"
        "Actual: some value");

    capture_test_results(assert_empty_string);

    assert_that("assert_that handles empty strings",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

pfstest_case(assert_null_value)
{
    assert_that("assert_that handles null value", NULL, matches_anything);
}

test(should_cope_with_null_value)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "assert_that called with NULL value");

    capture_test_results(assert_null_value);

    assert_that("assert_that handles NULL value",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

pfstest_case(assert_null_matcher)
{
    assert_that("assert_that handles null matcher", some_value, NULL);
}

test(should_cope_with_null_matcher)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "assert_that called with NULL matcher");

    capture_test_results(assert_null_matcher);

    assert_that("assert_that handles NULL matcher",
                the_string(captured_output),
                matches_the_nv_string(expected));
}
