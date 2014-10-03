#include <stdlib.h>
#include <stdio.h>

#include "pfstest.h"

static void some_value_printer(pfstest_value_t *value)
{
    printf("some value");
}

static pfstest_value_t some_value = {
    .printer = some_value_printer,
    .data = NULL,
};

static bool always_return_true(pfstest_matcher_t *matcher,
                               pfstest_value_t *actual)
{
    return true;
}

static pfstest_matcher_t anything = {
    .printer = NULL,
    .test = always_return_true,
    .data = NULL,
};

test(should_pass_assert_that)
{
    assert_that("always passes", &some_value, &anything);
}

static bool always_return_false(pfstest_matcher_t *matcher,
                                pfstest_value_t *actual)
{
    return false;
}

static void nothing_printer(pfstest_matcher_t *matcher)
{
    printf("nothing (guaranteed to fail)");
}

static pfstest_matcher_t nothing = {
    .printer = nothing_printer,
    .test = always_return_false,
    .data = NULL,
};

failing_test(should_fail_assert_that)
{
    assert_that("always fails", &some_value, &nothing);
}
