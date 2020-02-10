#include <stdlib.h>
#include <stdio.h>

#define __PFSTEST_FILE__ "tests/test-assert-that.c"

#include "pfstest.h"
#include "capture-output.h"
#include "output-definitions.h"

static pfstest_reporter_t *standard_reporter;

static void some_value_printer(pfstest_value_t *value,
                               pfstest_reporter_t *reporter)
{
    (void)value;
    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("some value"));
}

static pfstest_value_t *some_value;

static pfstest_bool always_return_true(pfstest_matcher_t *matcher,
                                       pfstest_value_t *actual)
{
    (void)matcher;
    (void)actual;
    return pfstest_true;
}

static pfstest_matcher_t *matches_anything;

static pfstest_bool always_return_false(pfstest_matcher_t *matcher,
                                        pfstest_value_t *actual)
{
    (void)matcher;
    (void)actual;
    return pfstest_false;
}

static void nothing_printer(pfstest_matcher_t *matcher,
                            pfstest_reporter_t *reporter)
{
    (void)matcher;
    pfstest_reporter_print_pg_str(
        reporter, pfstest_pg_str("nothing (guaranteed to fail)"));
}

static pfstest_matcher_t *matches_nothing;

setup()
{
    capture_output_init();
    standard_reporter = pfstest_reporter_standard_new(
        capture_output_char, pfstest_report_colorizer_null);

    some_value = pfstest_value_new(some_value_printer, NULL, 0);
    matches_anything = pfstest_matcher_new(NULL, always_return_true, NULL);
    matches_nothing =
        pfstest_matcher_new(nothing_printer, always_return_false, NULL);
}

test(should_pass_assertion)
{
    assert_that("always passes", some_value, matches_anything);
}

pfstest_case(assert_always_fail)
{
    /* Ensure predictable failure position reporting */
#undef __PFSTEST_LINE__
#define __PFSTEST_LINE__ 1111
    assert_that("always fails", some_value, matches_nothing);
#undef __PFSTEST_LINE__
#define __PFSTEST_LINE__ __LINE__
}

test(should_fail_on_false_assertion)
{
    capture_test_results(assert_always_fail);

    assert_that("False assertions fail",
                the_int(pfstest_reporter_return_value(message_spy)),
                is(the_int(EXIT_FAILURE)));
}

test(should_print_explanation_on_failed_assertion)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        HEADER
        "tests/test-assert-that.c:assert_always_fail FAIL\n"
        "    Location: tests/test-assert-that.c:1111\n"
        "    Failed assertion: always fails\n"
        "    Expected: nothing (guaranteed to fail)\n"
        "    Actual:   some value\n\n"
        "Run complete. 0 passed, 1 failed, 0 ignored\n");
    pfstest_list_t *suite = pfstest_alloc(sizeof(*suite));
    pfstest_list_reset(suite);
    pfstest_suite_register_test(suite, assert_always_fail);

    pfstest_suite_run(NULL, suite, NULL, NULL, standard_reporter);

    assert_that("Failing assertions should print an explanatory message",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

/* avr-gcc throws a compiler error on NULL assert_that message
 * strings */
#if !(defined(__GNUC__) && defined(__AVR__))
pfstest_case(assert_null_message)
{
    /* This is a failing test so that the output can be inspected */
    assert_that(NULL, some_value, matches_nothing);
}

test(should_cope_with_null_message)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        "Failed assertion\n"
        "Expected: nothing (guaranteed to fail)\n"
        "Actual:   some value");

    capture_test_results(assert_null_message);

    assert_that("assert_that handles NULL messages",
                the_string(captured_output),
                matches_the_pg_string(expected));
}
#endif

pfstest_case(assert_empty_message)
{
    /* This is a failing test so that the output can be inspected */
    assert_that("", some_value, matches_nothing);
}

test(should_cope_with_empty_message)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        "Failed assertion\n"
        "Expected: nothing (guaranteed to fail)\n"
        "Actual:   some value");

    capture_test_results(assert_empty_message);

    assert_that("assert_that handles empty messages",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

pfstest_case(assert_null_value)
{
    assert_that("assert_that handles null value", NULL, matches_anything);
}

test(should_cope_with_null_value)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("assert_that called with NULL value");

    capture_test_results(assert_null_value);

    assert_that("assert_that handles NULL value",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

pfstest_case(assert_null_matcher)
{
    assert_that("assert_that handles null matcher", some_value, NULL);
}

test(should_cope_with_null_matcher)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("assert_that called with NULL matcher");

    capture_test_results(assert_null_matcher);

    assert_that("assert_that handles NULL matcher",
                the_string(captured_output),
                matches_the_pg_string(expected));
}
