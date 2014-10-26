#include <stdlib.h>
#include <stdio.h>

#include "pfstest.h"

static void some_value_printer(pfstest_value_t *value)
{
    pfstest_print_nv_string(pfstest_nv_string("some value"));
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

static void nothing_printer(pfstest_matcher_t *matcher)
{
    pfstest_print_nv_string(
        pfstest_nv_string("nothing (guaranteed to fail)"));
}

static pfstest_matcher_t matches_nothing[1] = {{
        nothing_printer,
        always_return_false,
        NULL,
    }};

test(should_pass_assertion)
{
    assert_that("always passes", some_value, matches_anything);
}

failing_test(should_fail_assertion)
{
    assert_that("always fails", some_value, matches_nothing);
}

/* avr-gcc throws a compiler error on NULL assert_that message
 * strings */
#if !(defined(__GNUC__) && defined(__AVR__))
failing_test(should_cope_with_null_string)
{
    /* This is a failing test so that the output can be visually inspected */
    assert_that(NULL, some_value, matches_nothing);
}
#endif

failing_test(should_cope_with_null_value)
{
    assert_that("assert_that handles null value", NULL, matches_anything);
}

failing_test(should_cope_with_null_matcher)
{
    assert_that("assert_that handles null matcher", some_value, NULL);
}
