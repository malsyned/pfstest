#include "pfstest.h"

#include "mock-dep.h"

test(should_stub)
{
    do_return(the_int(5), when(mock_dep_func1, arg_that(is_the_int(2))));
    do_return(the_int(10), when(mock_dep_func1, arg_that(is_the_int(4))));

    assert_that("stub returns value",
                the_int(dep_func1(2)), is_the_int(5));
    assert_that("stub tests args",
                the_int(dep_func1(4)), is_the_int(10));
    assert_that("stub keeps matching",
                the_int(dep_func1(2)), is_the_int(5));
    assert_that("no stub needed",
                the_int(dep_func1(47)), is_the_int(0));
}

test(should_reset_expectations_between_tests)
{
    assert_that("stub returns default value",
                the_int(dep_func1(2)), is_the_int(0));
}

test(should_provide_default_return_value)
{
    when(mock_dep_func1, arg_that(is_the_int(2)));
    assert_that("stub provides default return value",
                the_int(dep_func1(2)), is_the_int(0));
}

test(should_return_on_first_match)
{
    do_return(the_int(5), when(mock_dep_func1, arg_that(is_the_int(2))));
    do_return(the_int(6), when(mock_dep_func1, arg_that(is_the_int(2))));
    assert_that("stub returns result from first matching expectation",
                the_int(dep_func1(2)), is_the_int(5));
}

test(should_verify_a_call)
{
    verify(when(mock_dep_func1, arg_that(is_the_int(2))));

    dep_func1(2);
}

failing_test(should_report_when_verification_fails)
{
    verify(when(mock_dep_func1, arg_that(is_the_int(2))));

    dep_func1(3);
}

failing_test(should_verify_invocation_counts)
{
    verify(when(mock_dep_func1, arg_that(is_the_int(2))));

    dep_func1(2);
    dep_func1(2);
}

failing_test(should_handle_multiple_verifiers)
{
    verify(when(mock_dep_func1, arg_that(is_the_int(2))));
    verify(when(mock_dep_func1, arg_that(is_the_int(3))));

    dep_func1(3);
}

test(should_handle_multiple_invocations)
{
    verify(when(mock_dep_func1, arg_that(is_the_int(2))));
    /* Ensure an invocation is added. Default invocations don't get
     * chained onto the invocation list (yet?). */
    do_return(the_int(5),
              when(mock_dep_func1, arg_that(is_the_int(3))));

    dep_func1(2);
    dep_func1(3);
}

test(should_return_through_pointer)
{
    char buf[] = "foo";

    when(mock_dep_func2,
         arg_that(is_the_int(2)),
         assign_arg(the_string("bar")));

    dep_func2(2, buf);

    assert_that("string returned through pointer",
                the_string(buf), is_the_string("bar"));
}

failing_test(should_print_sensible_explanation_of_assign_arg_in_failures)
{
    verify(when(mock_dep_func2,
                arg_that(is_the_int(2)), assign_arg(the_string("foo"))));
}

test(should_match_and_return_through_pointer)
{
    char buf[] = "foo";
    char untouched_buf[] = "123";

    when(mock_dep_func2,
         arg_that(is_the_int(2)),
         assign_arg_that(is_the_string("foo"),
                         the_string("bar")));

    dep_func2(2, untouched_buf);
    dep_func2(2, buf);

    assert_that("string returned through pointer on match",
                the_string(buf), is_the_string("bar"));

    assert_that("string untouched when not matched",
                the_string(untouched_buf), is_the_string("123"));
}

failing_test(should_print_matcher_for_failures_involving_assign_arg_that)
{
    verify(when(mock_dep_func2,
                arg_that(is_the_int(2)),
                assign_arg_that(is_the_string("foo"),
                                the_string("bar"))));
}

test(should_stub_different_return_values_with_one_time)
{
    one_time(do_return(the_int(3), when(mock_dep_func1,
                                        arg_that(is_the_int(4)))));
    do_return(the_int(6), when(mock_dep_func1,
                               arg_that(is_the_int(4))));

    assert_that("First result comes from one_time expectation",
                the_int(dep_func1(4)), is_the_int(3));
    assert_that("Second result doesn't come from one_time expectation",
                the_int(dep_func1(4)), is_the_int(6));
    assert_that("Subsequent results come from infinite-repeat expectation",
                the_int(dep_func1(4)), is_the_int(6));
}

test(should_stub_different_return_values_with_do_times)
{
    do_times(2,
             do_return(the_int(3), when(mock_dep_func1,
                                        arg_that(is_the_int(4)))));
    do_return(the_int(6), when(mock_dep_func1, arg_that(is_the_int(4))));

    assert_that("First result comes from do_times",
                the_int(dep_func1(4)), is_the_int(3));
    assert_that("Second result comes from do_times",
                the_int(dep_func1(4)), is_the_int(3));
    assert_that("Subsequent results come from infinite-repeat expectation",
                the_int(dep_func1(4)), is_the_int(6));
}

test(should_verify_in_order)
{
    in_order_t *order = in_order_new();

    in_order_verify(order, when(mock_dep_func1,
                                arg_that(is_the_int(2))));
    in_order_verify(order, when(mock_dep_func2,
                                arg_that(is_the_int(4)),
                                arg_that(is_the_string("foo"))));

    dep_func1(1);
    dep_func1(2);
    dep_func1(3);
    dep_func2(4, "bar");
    dep_func2(4, "foo");
}

failing_test(should_fail_when_out_of_order)
{
    in_order_t *order = in_order_new();

    in_order_verify(order, when(mock_dep_func1,
                                arg_that(is_the_int(2))));
    in_order_verify(order, when(mock_dep_func2,
                                arg_that(is_the_int(4)),
                                arg_that(is_the_string("foo"))));

    dep_func1(1);
    dep_func2(4, "foo");
    dep_func1(2);
    dep_func1(3);
    dep_func2(4, "bar");
}

test(should_verify_multiple_in_orders)
{
    in_order_t *order1 = in_order_new();
    in_order_t *order2 = in_order_new();

    in_order_verify(order1, when(mock_dep_func1,
                                 arg_that(is_the_int(1))));
    in_order_verify(order1, when(mock_dep_func2,
                                 arg_that(is_the_int(2)),
                                 arg_that(is_the_string("foo"))));

    in_order_verify(order2, when(mock_dep_func1,
                                 arg_that(is_the_int(3))));
    in_order_verify(order2, when(mock_dep_func2,
                                 arg_that(is_the_int(4)),
                                 arg_that(is_the_string("bar"))));

    dep_func1(1);
    dep_func1(3);
    dep_func2(4, "bar");
    dep_func2(2, "foo");
}

test(should_verify_exact_invocation_count)
{
    verify_times(exactly(3),
                 when(mock_dep_func1, arg_that(is_the_int(1))));

    dep_func1(1);
    dep_func1(2);
    dep_func1(1);
    dep_func1(1);
}

failing_test(should_reject_too_many_invocations)
{
    verify_times(exactly(3),
                 when(mock_dep_func1, arg_that(is_the_int(1))));

    dep_func1(1);
    dep_func1(1);
    dep_func1(1);
    dep_func1(1);
}

failing_test(should_reject_too_few_invocations)
{
    verify_times(exactly(3),
                 when(mock_dep_func1, arg_that(is_the_int(1))));

    dep_func1(1);
    dep_func1(1);
}

ignore_test(should_verify_at_most_invocation_count)
{
}

ignore_failing_test(at_most_should_reject_too_many_invocations)
{
}

ignore_test(should_verify_at_least_invocation_count)
{
}

ignore_failing_test(at_least_should_reject_insufficient_invocations)
{
}

ignore_test(should_verify_no_more_interactions)
{
}

ignore_failing_test(verify_no_more_interactions_should_reject_surplus_invocations)
{
}
