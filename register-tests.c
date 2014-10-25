#include "pfstest.h"
#include "register-tests.h"

static void register_assert_that_tests(void)
{
    register_test(should_pass_assertion);
    register_test(should_fail_assertion);
    register_test(should_cope_with_null_string);
    register_test(should_cope_with_null_value);
    register_test(should_cope_with_null_matcher);
}

static void register_values_and_matchers_tests(void)
{
    register_test(should_match_shorts);
    register_test(should_fail_on_different_shorts);
    register_test(should_match_ushorts);
    register_test(should_fail_on_different_ushorts);
    register_test(should_match_ints);
    register_test(should_fail_on_different_ints);
    register_test(should_match_uints);
    register_test(should_fail_on_different_uints);
    register_test(should_detect_data_type_mismatches);
    register_test(should_match_chars);
    register_test(should_fail_on_different_chars);
    register_test(should_match_strings);
    register_test(should_fail_on_different_strings);
    register_test(should_fail_on_shorter_actual_string);
    register_test(should_fail_on_shorter_expected_string);
    register_test(should_match_pointers);
    register_test(should_fail_on_different_pointers);
    register_test(should_match_memory);
    register_test(should_fail_on_different_memory);
    register_test(should_fail_expecting_shorter_memory);
    register_test(should_fail_expecting_longer_memory);
    register_test(should_match_memory_with_pointer);
    register_test(should_match_primitive_with_pointer);
}

static void register_mock_tests(void)
{
    register_test(should_stub);
    register_test(should_reset_expectations_between_tests);
    register_test(should_provide_default_return_value);
    register_test(should_return_on_first_match);
    register_test(should_verify_a_call);
    register_test(should_report_when_verification_fails);
    register_test(should_verify_invocation_counts);
    register_test(should_handle_multiple_verifiers);
    register_test(should_handle_multiple_invocations);
    register_test(should_return_through_pointer);
    register_test(should_print_sensible_explanation_of_assign_arg_in_failures);
    register_test(should_match_and_return_through_pointer);
    register_test(should_print_matcher_for_failures_involving_assign_arg_that);
    register_test(should_stub_different_return_values_with_one_time);
    register_test(should_stub_different_return_values_with_do_times);
    register_test(should_verify_in_order);
    register_test(should_fail_when_out_of_order);
    register_test(should_verify_multiple_in_orders);
    register_test(should_verify_exact_invocation_count);
    register_test(should_reject_too_many_invocations);
    register_test(should_reject_too_few_invocations);
    register_test(should_verify_at_most_invocation_count);
    register_test(at_most_should_reject_too_many_invocations);
    register_test(at_most_should_allow_fewer_invocations);
    register_test(should_verify_at_least_invocation_count);
    register_test(at_least_should_reject_insufficient_invocations);
    register_test(at_least_should_allow_more_invocations);
    register_test(should_verify_no_more_interactions);
    register_test(verify_no_more_interactions_should_reject_surplus_interactions);
    register_test(in_order_should_mark_interactions);
    register_test(should_verify_no_more_invocations);
    register_test(verify_no_more_invocations_should_reject_surpluss_invocations);
}

void register_tests(void)
{
    register_assert_that_tests();
    register_values_and_matchers_tests();
    register_mock_tests();
}
