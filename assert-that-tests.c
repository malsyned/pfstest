#include <stdlib.h>
#include <stdio.h>

#include "pfstest.h"

static void some_value_printer(pfstest_output_formatter_t *formatter,
                               pfstest_value_t *value)
{
    pfstest_output_formatter_message_print_nv_string(
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
    pfstest_output_formatter_message_print_nv_string(
        formatter, pfstest_nv_string("nothing (guaranteed to fail)"));
}

static pfstest_matcher_t matches_nothing[1] = {{
        nothing_printer,
        always_return_false,
        NULL,
    }};

static char captured_output[300];
static int captured_output_idx = 0;

void capture_output_init(void)
{
    captured_output[0] = '\0';
    captured_output_idx = 0;
}

int capture_output_char(int c)
{
    if (captured_output_idx < sizeof(captured_output) - 1) {
        captured_output[captured_output_idx++] = c;
        captured_output[captured_output_idx] = '\0';
        return c;
    }

    return EOF;
}

static pfstest_output_formatter_t message_spy;

before_tests(set_up)
{
    capture_output_init();
    pfstest_output_formatter_message_spy_init(&message_spy,
                                              capture_output_char);
}

test(should_pass_assertion)
{
    assert_that("always passes", some_value, matches_anything);
}

static void assert_always_fail(void)
{
    assert_that("always fails", some_value, matches_nothing);
}

test(should_fail_on_false_assertion)
{
    pfstest_protect_call(assert_always_fail, &message_spy);

    assert_that("False assertions fail",
                the_int(pfstest_output_formatter_return_value(&message_spy)),
                is_the_int(1));
}

test(should_print_explanation_on_failed_assertion)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "Failed assertion: always fails\n"
        "Expected: nothing (guaranteed to fail)\n"
        "Actual: some value");

    pfstest_protect_call(assert_always_fail, &message_spy);

    assert_that("Failing assertions should print an explanatory message",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

/* avr-gcc throws a compiler error on NULL assert_that message
 * strings */
#if !(defined(__GNUC__) && defined(__AVR__))
static void assert_null_string(void)
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

    pfstest_protect_call(assert_null_string, &message_spy);

    assert_that("assert_that handles NULL strings",
                the_string(captured_output),
                matches_the_nv_string(expected));
}
#endif

static void assert_empty_string(void)
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

    pfstest_protect_call(assert_empty_string, &message_spy);

    assert_that("assert_that handles empty strings",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

static void assert_null_value(void)
{
    assert_that("assert_that handles null value", NULL, matches_anything);
}

test(should_cope_with_null_value)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "assert_that called with NULL value");

    pfstest_protect_call(assert_null_value, &message_spy);

    assert_that("assert_that handles NULL value",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

static void assert_null_matcher(void)
{
    assert_that("assert_that handles null matcher", some_value, NULL);
}

test(should_cope_with_null_matcher)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "assert_that called with NULL matcher");

    pfstest_protect_call(assert_null_matcher, &message_spy);

    assert_that("assert_that handles NULL matcher",
                the_string(captured_output),
                matches_the_nv_string(expected));
}
