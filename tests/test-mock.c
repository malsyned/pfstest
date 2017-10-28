#include "pfstest.h"
#include "capture-output.h"

#include "dep.mock.h"

static pfstest_list_t *plugins;

/* Each plugin can only be a member of one plugin list, and
 * pfstest_mock_plugin is already a member of the registered plugins
 * list. */
pfstest_plugin_define(test_mock_plugin,
                      pfstest_mock_setup,
                      pfstest_mock_run_verifiers,
                      pfstest_mock_teardown);

before_tests(set_up_mock_tests)
{
    plugins = pfstest_alloc(sizeof(*plugins));
    pfstest_list_reset(plugins);
    pfstest_plugin_list_register_plugin(plugins, test_mock_plugin);

    capture_output_init();
}

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

test(stub_should_return_pointer)
{
    char s[] = "foo";

    do_return(the_pointer(s), when(mock_dep_func3));

    assert_that("stub returns pointer",
                the_pointer(dep_func3()), is_the_pointer(s));
}

test(stub_should_return_null_pointer_by_default)
{
    assert_that("stub returns NULL by default",
                the_pointer(dep_func3()), is_the_pointer(NULL));
}

test(should_verify_a_call)
{
    verify(when(mock_dep_func1, arg_that(is_the_int(2))));

    dep_func1(2);
}

pfstest_case(fails_a_verification)
{
    verify(when(mock_dep_func1, arg_that(is_the_int(2))));

    dep_func1(3);
}

test(should_report_when_verification_fails)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        "Never called dep_func1 with (the int 2)");

    capture_test_results_with_plugins(fails_a_verification, plugins);

    assert_that("Verification failures are reported",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

pfstest_case(fails_invocation_count)
{
    verify(when(mock_dep_func1, arg_that(is_the_int(2))));

    dep_func1(2);
    dep_func1(2);
}

test(should_verify_invocation_counts)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        "Wanted dep_func1 with (the int 2) exactly 1 time\n"
        "Was called 2 times");

    capture_test_results_with_plugins(fails_invocation_count, plugins);

    assert_that("Invocation counts are verified",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

pfstest_case(fails_to_satisfy_multiple_verifiers)
{
    verify(when(mock_dep_func1, arg_that(is_the_int(2))));
    verify(when(mock_dep_func1, arg_that(is_the_int(3))));

    dep_func1(3);
}

test(should_handle_multiple_verifiers)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        "Never called dep_func1 with (the int 2)");

    capture_test_results_with_plugins(fails_to_satisfy_multiple_verifiers,
                                      plugins);

    assert_that("Multiple verifiers are checked",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

test(should_handle_multiple_invocations)
{
    verify(when(mock_dep_func1, arg_that(is_the_int(2))));
    do_return(the_int(5),
              when(mock_dep_func1, arg_that(is_the_int(3))));

    dep_func1(2);
    dep_func1(3);
}

static char foo[] = "foo";
static char bar[] = "bar";
static char one23[] = "123";

test(should_return_through_pointer)
{
    char buf[] = "foo";

    when(mock_dep_func2,
         arg_that(is_the_int(2)),
         assign_arg(the_string(bar)));

    dep_func2(2, buf);

    assert_that("string returned through pointer",
                the_string(buf), is_the_string(bar));
}

pfstest_case(fails_to_invoke_assign_arg_expectation)
{
    verify(when(mock_dep_func2,
                arg_that(is_the_int(2)), assign_arg(the_string(foo))));
}

test(should_print_sensible_explanation_of_assign_arg_in_failures)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        "Never called dep_func2 with (the int 2, any pointer)");

    capture_test_results_with_plugins(fails_to_invoke_assign_arg_expectation,
                                      plugins);

    assert_that("assign_arg description is sensible",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

test(should_match_and_return_through_pointer)
{
    char buf[] = "foo";
    char untouched_buf[] = "123";

    when(mock_dep_func2,
         arg_that(is_the_int(2)),
         assign_arg_that(is_the_string(foo),
                         the_string(bar)));

    dep_func2(2, untouched_buf);
    dep_func2(2, buf);

    assert_that("string returned through pointer on match",
                the_string(buf), is_the_string(bar));

    assert_that("string untouched when not matched",
                the_string(untouched_buf), is_the_string(one23));
}

pfstest_case(fails_to_invoke_assign_arg_that_expectation)
{
    verify(when(mock_dep_func2,
                arg_that(is_the_int(2)),
                assign_arg_that(is_the_string(foo),
                                the_string(bar))));
}

test(should_print_matcher_for_failures_involving_assign_arg_that)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        "Never called dep_func2 with (the int 2, the string \"foo\")");

    capture_test_results_with_plugins(
        fails_to_invoke_assign_arg_that_expectation, plugins);

    assert_that("assign_arg_that description is sensible",
                the_string(captured_output),
                matches_the_pg_string(expected));
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
                                arg_that(is_the_string(foo))));

    dep_func1(1);
    dep_func1(2);
    dep_func1(3);
    dep_func2(4, bar);
    dep_func2(4, foo);
}

#ifdef __18CXX
/* All of the strings in this file overran 256 bytes of idata */
#pragma idata mock_tests_2
#endif

pfstest_case(fails_to_invoke_in_order)
{
    in_order_t *order = in_order_new();

    in_order_verify(order, when(mock_dep_func1,
                                arg_that(is_the_int(2))));
    in_order_verify(order, when(mock_dep_func2,
                                arg_that(is_the_int(4)),
                                arg_that(is_the_string(foo))));

    dep_func1(1);
    dep_func2(4, foo);
    dep_func1(2);
    dep_func1(3);
    dep_func2(4, bar);
}

test(should_fail_when_out_of_order)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        "Not called in order: dep_func2 with (the int 4, the string \"foo\")\n"
        "Expected after: dep_func1 with (the int 2)");

    capture_test_results_with_plugins(fails_to_invoke_in_order, plugins);

    assert_that("in_order invocations are verified",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

test(should_verify_multiple_in_orders)
{
    in_order_t *order1 = in_order_new();
    in_order_t *order2 = in_order_new();

    in_order_verify(order1, when(mock_dep_func1,
                                 arg_that(is_the_int(1))));
    in_order_verify(order1, when(mock_dep_func2,
                                 arg_that(is_the_int(2)),
                                 arg_that(is_the_string(foo))));

    in_order_verify(order2, when(mock_dep_func1,
                                 arg_that(is_the_int(3))));
    in_order_verify(order2, when(mock_dep_func2,
                                 arg_that(is_the_int(4)),
                                 arg_that(is_the_string(bar))));

    dep_func1(1);
    dep_func1(3);
    dep_func2(4, bar);
    dep_func2(2, foo);
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

pfstest_case(invokes_too_many_times)
{
    verify_times(exactly(3),
                 when(mock_dep_func1, arg_that(is_the_int(1))));

    dep_func1(1);
    dep_func1(1);
    dep_func1(1);
    dep_func1(1);
}

test(should_reject_too_many_invocations)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        "Wanted dep_func1 with (the int 1) exactly 3 times\n"
        "Was called 4 times");

    capture_test_results_with_plugins(invokes_too_many_times, plugins);

    assert_that("too many invocation counts are verified in exact mode",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

pfstest_case(invokes_too_few_times)
{
    verify_times(exactly(3),
                 when(mock_dep_func1, arg_that(is_the_int(1))));

    dep_func1(1);
    dep_func1(1);
}

test(should_reject_too_few_invocations)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        "Wanted dep_func1 with (the int 1) exactly 3 times\n"
        "Was called 2 times");

    capture_test_results_with_plugins(invokes_too_few_times, plugins);

    assert_that("too few invocation counts are verified in exact mode",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

test(should_verify_at_most_invocation_count)
{
    verify_times(at_most(3),
                 when(mock_dep_func1, arg_that(is_the_int(1))));

    dep_func1(1);
    dep_func1(2);
    dep_func1(1);
    dep_func1(1);
}

pfstest_case(invokes_too_many_times_in_at_most_mode)
{
    verify_times(at_most(3),
                 when(mock_dep_func1, arg_that(is_the_int(1))));

    dep_func1(1);
    dep_func1(1);
    dep_func1(1);
    dep_func1(1);
}

test(at_most_should_reject_too_many_invocations)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        "Wanted dep_func1 with (the int 1) at most 3 times\n"
        "Was called 4 times");

    capture_test_results_with_plugins(
        invokes_too_many_times_in_at_most_mode, plugins);

    assert_that("too many invocation counts are verified in at_most mode",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

test(at_most_should_allow_fewer_invocations)
{
    verify_times(at_most(3),
                 when(mock_dep_func1, arg_that(is_the_int(1))));

    dep_func1(1);
    dep_func1(1);
}

test(should_verify_at_least_invocation_count)
{
    verify_times(at_least(3),
                 when(mock_dep_func1, arg_that(is_the_int(1))));

    dep_func1(1);
    dep_func1(2);
    dep_func1(1);
    dep_func1(1);
}

pfstest_case(invokes_too_few_times_in_at_least_mode)
{
    verify_times(at_least(3),
                 when(mock_dep_func1, arg_that(is_the_int(1))));

    dep_func1(1);
    dep_func1(1);
}

test(at_least_should_reject_insufficient_invocations)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        "Wanted dep_func1 with (the int 1) at least 3 times\n"
        "Was called 2 times");

    capture_test_results_with_plugins(invokes_too_few_times_in_at_least_mode,
                                      plugins);

    assert_that("too few invocation counts are verified in at_least mode",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

test(at_least_should_allow_more_invocations)
{
    verify_times(at_least(3),
                 when(mock_dep_func1, arg_that(is_the_int(1))));

    dep_func1(1);
    dep_func1(1);
    dep_func1(1);
    dep_func1(1);
}

test(should_verify_no_more_interactions)
{
    verify(when(mock_dep_func1, arg_that(is_the_int(1))));
    verify(when(mock_dep_func1, arg_that(is_the_int(2))));
    verify_no_more_interactions(mock_dep_func1);

    dep_func1(1);
    dep_func1(2);
    dep_func2(5, foo);
}

pfstest_case(fails_to_pass_verify_no_more_interactions)
{
    verify(when(mock_dep_func1, arg_that(is_the_int(1))));
    verify(when(mock_dep_func1, arg_that(is_the_int(2))));
    verify_no_more_interactions(mock_dep_func1);

    dep_func1(1);
    dep_func1(2);
    dep_func1(3);
}

test(verify_no_more_interactions_should_reject_surplus_interactions)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        "Unexpected interactions with dep_func1");

    capture_test_results_with_plugins(
        fails_to_pass_verify_no_more_interactions, plugins);

    assert_that("verify_no_more_interactions rejects surplus interactions",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

test(in_order_should_mark_interactions)
{
    in_order_t *order = in_order_new();

    in_order_verify(order, when(mock_dep_func1,
                                arg_that(is_the_int(2))));
    verify_no_more_interactions(mock_dep_func1);

    dep_func1(2);
}

test(should_verify_no_more_invocations)
{
    verify(when(mock_dep_func1, arg_that(is_the_int(1))));
    verify(when(mock_dep_func2,
                arg_that(is_the_int(5)),
                arg_that(is_the_string(foo))));
    verify_no_more_invocations();

    dep_func1(1);
    dep_func2(5, foo);
}

pfstest_case(fails_to_pass_verify_no_more_invocations)
{
    verify(when(mock_dep_func1, arg_that(is_the_int(1))));
    verify(when(mock_dep_func2,
                arg_that(is_the_int(5)),
                arg_that(is_the_string(foo))));
    verify_no_more_invocations();

    dep_func1(1);
    dep_func2(5, foo);
    dep_func1(3);
}

test(verify_no_more_invocations_should_reject_surplus_invocations)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        "Unexpected mock invocations");

    capture_test_results_with_plugins(
        fails_to_pass_verify_no_more_invocations, plugins);

    assert_that("verify_no_more_invocations rejects surplus invocations",
                the_string(captured_output),
                matches_the_pg_string(expected));
}
