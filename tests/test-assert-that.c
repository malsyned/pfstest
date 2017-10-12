#include <stdlib.h>
#include <stdio.h>

#define __PFSTEST_FILE__ "tests/test-assert-that.c"

#include "pfstest.h"
#include "capture-output.h"

static pfstest_reporter_t *standard_reporter;

static void some_value_printer(pfstest_reporter_t *reporter,
                               pfstest_value_t *value)
{
    (void)value;
    pfstest_reporter_print_nv_string(
        reporter, pfstest_nv_string("some value"));
}

static pfstest_value_t some_value[1] = {{
        some_value_printer,
        NULL,
        0,
    }};

static bool always_return_true(pfstest_matcher_t *matcher,
                               pfstest_value_t *actual)
{
    (void)matcher;
    (void)actual;
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
    (void)matcher;
    (void)actual;
    return false;
}

static void nothing_printer(pfstest_reporter_t *reporter,
                            pfstest_matcher_t *matcher)
{
    (void)matcher;
    pfstest_reporter_print_nv_string(
        reporter, pfstest_nv_string("nothing (guaranteed to fail)"));
}

static pfstest_matcher_t matches_nothing[1] = {{
        nothing_printer,
        always_return_false,
        NULL,
    }};

before_tests(set_up_assert_that)
{
    capture_output_init();
    standard_reporter = pfstest_reporter_standard_new(
        capture_output_char, pfstest_report_colorizer_null);
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
                the_int(pfstest_reporter_return_value(message_spy)),
                is_the_int(EXIT_FAILURE));
}

test(should_print_explanation_on_failed_assertion)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "tests/test-assert-that.c:assert_always_fail FAIL\n"
        "    Location: tests/test-assert-that.c:75\n"
        "    Failed assertion: always fails\n"
        "    Expected: nothing (guaranteed to fail)\n"
        "    Actual:   some value\n");

    pfstest_run(assert_always_fail, NULL, NULL, NULL, standard_reporter);

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
        "Actual:   some value");

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
        "Actual:   some value");

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
