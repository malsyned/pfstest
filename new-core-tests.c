#include "pfstest.h"

#include <stdio.h>

char call_log[150];

pfstest_case(should_run)
{
    pfstest_strcat_nv(call_log, pfstest_nv_string("should_run "));
}

pfstest_case(should_also_run)
{
    pfstest_strcat_nv(call_log, pfstest_nv_string("should_also_run "));
}

pfstest_case_ignored(should_be_ignored)
{
    pfstest_strcat_nv(call_log, pfstest_nv_string("should_be_ignored "));
}

pfstest_case(should_fail)
{
    fail("Expected failure, should have been caught");
}

pfstest_hook(should_be_run_before)
{
    pfstest_strcat_nv(call_log, pfstest_nv_string("should_be_run_before "));
}

pfstest_hook(should_also_be_run_before)
{
    pfstest_strcat_nv(call_log,
                      pfstest_nv_string("should_also_be_run_before "));
}

pfstest_hook(should_be_run_after)
{
    pfstest_strcat_nv(call_log, pfstest_nv_string("should_be_run_after "));
}

pfstest_hook(should_also_be_run_after)
{
    pfstest_strcat_nv(call_log,
                      pfstest_nv_string("should_also_be_run_after "));
}

pfstest_hook(hook_should_fail)
{
    fail("Expected failure, should have been caught");
}

pfstest_hook(hook_should_also_fail)
{
    fail("Another expected failure, should have been caught");
}

pfstest_case(should_also_fail)
{
    fail("Another expected failure, should have been caught");
}

pfstest_case(should_have_multi_line_failure)
{
    fail("Expected failure, should have been caught\n"
         "and formatted correctly\nacross multiple lines");
}

/* TODO: Remove all instances of "new" from identifiers and file names
 * once all functionality has been moved over to the new
 * implementation */

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

static pfstest_output_formatter_t standard_formatter;
static pfstest_output_formatter_t verbose_formatter;

static pfstest_list_t suite;
static pfstest_list_t before_hooks;
static pfstest_list_t after_hooks;

before_tests(setup)
{
    capture_output_init();
    call_log[0] = '\0';

    pfstest_list_reset(&suite);
    pfstest_list_reset(&before_hooks);
    pfstest_list_reset(&after_hooks);

    pfstest_output_formatter_standard_init(&standard_formatter,
                                           capture_output_char);
    pfstest_output_formatter_verbose_init(&verbose_formatter,
                                          capture_output_char);
}

test(should_run_tests)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "should_run should_also_run ");

    pfstest_suite_register_test(&suite, should_run);
    pfstest_suite_register_test(&suite, should_also_run);

    pfstest_suite_run(&before_hooks, &after_hooks, &suite,
                      NULL, NULL,
                      &standard_formatter);

    if (0 != pfstest_strcmp_nv(call_log, expected)) {
        fail("Log did not match expected log");
    }
}

test(should_run_before_hooks)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "should_be_run_before should_also_be_run_before should_run ");

    pfstest_hook_list_register_hook(&before_hooks, should_be_run_before);
    pfstest_hook_list_register_hook(&before_hooks, should_also_be_run_before);
    pfstest_suite_register_test(&suite, should_run);

    pfstest_suite_run(&before_hooks, &after_hooks, &suite,
                      NULL, NULL,
                      &standard_formatter);

    if (0 != pfstest_strcmp_nv(call_log, expected)) {
        fail("Log did not match expected log");
    }
}

test(should_run_after_hooks)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "should_run should_be_run_after should_also_be_run_after ");

    pfstest_hook_list_register_hook(&after_hooks, should_be_run_after);
    pfstest_hook_list_register_hook(&after_hooks, should_also_be_run_after);
    pfstest_suite_register_test(&suite, should_run);

    pfstest_suite_run(&before_hooks, &after_hooks, &suite,
                      NULL, NULL,
                      &standard_formatter);

    if (0 != pfstest_strcmp_nv(call_log, expected)) {
        fail("Log did not match expected log");
    }
}

test(should_skip_ignored_tests)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string("");

    pfstest_hook_list_register_hook(&before_hooks, should_be_run_before);
    pfstest_hook_list_register_hook(&after_hooks, should_be_run_after);
    pfstest_suite_register_test(&suite, should_be_ignored);

    pfstest_suite_run(&before_hooks, &after_hooks, &suite,
                      NULL, NULL,
                      &standard_formatter);

    if (0 != pfstest_strcmp_nv(call_log, expected)) {
        fail("Log did not match expected log");
    }
}

test(should_reset_hooks_and_tests)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "should_be_run_before should_run should_be_run_after ");

    /* Create some lists, which modifies ->next pointers of nodes */
    pfstest_hook_list_register_hook(&before_hooks, should_be_run_before);
    pfstest_hook_list_register_hook(&before_hooks, should_also_be_run_before);
    pfstest_hook_list_register_hook(&after_hooks, should_be_run_after);
    pfstest_hook_list_register_hook(&after_hooks, should_also_be_run_after);
    pfstest_suite_register_test(&suite, should_run);
    pfstest_suite_register_test(&suite, should_also_run);

    /* Reset the lists */
    pfstest_list_reset(&suite);
    pfstest_list_reset(&before_hooks);
    pfstest_list_reset(&after_hooks);

    /* Add some nodes to the lists, hoping the -> next pointers are
     * handled properly */
    pfstest_hook_list_register_hook(&before_hooks, should_be_run_before);
    pfstest_hook_list_register_hook(&after_hooks, should_be_run_after);
    pfstest_suite_register_test(&suite, should_run);

    pfstest_suite_run(&before_hooks, &after_hooks, &suite,
                      NULL, NULL,
                      &standard_formatter);

    if (0 != pfstest_strcmp_nv(call_log, expected)) {
        fail("Log did not match expected log");
    }
}

test(should_report_results)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "..\n"
        "Run complete. 2 passed, 0 failed, 0 ignored\n");

    pfstest_suite_register_test(&suite, should_run);
    pfstest_suite_register_test(&suite, should_also_run);

    pfstest_suite_run(&before_hooks, &after_hooks, &suite,
                      NULL, NULL,
                      &standard_formatter);

    if (0 != pfstest_strcmp_nv(captured_output, expected)) {
        fail("Output did not match expected output");
    }
}

test(should_report_ignored_tests)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "I\n"
        "Run complete. 0 passed, 0 failed, 1 ignored\n");

    pfstest_suite_register_test(&suite, should_be_ignored);

    pfstest_suite_run(&before_hooks, &after_hooks, &suite,
                      NULL, NULL,
                      &standard_formatter);

    if (0 != pfstest_strcmp_nv(captured_output, expected)) {
        fail("Output did not match expected output");
    }
}

test(should_report_failed_tests)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "new-core-tests.c:should_fail FAIL\n"
        "    Location: new-core-tests.c:24\n"
        "    Expected failure, should have been caught\n"
        "\n"
        "Run complete. 0 passed, 1 failed, 0 ignored\n");

    pfstest_suite_register_test(&suite, should_fail);
    pfstest_suite_run(&before_hooks, &after_hooks, &suite,
                      NULL, NULL,
                      &standard_formatter);

    if (0 != pfstest_strcmp_nv(captured_output, expected)) {
        fail("Output did not match expected output");
    }
}

test(should_report_results_including_failed_tests)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        ".\n"
        "new-core-tests.c:should_fail FAIL\n"
        "    Location: new-core-tests.c:24\n"
        "    Expected failure, should have been caught\n"
        ".\n"
        "Run complete. 2 passed, 1 failed, 0 ignored\n");

    pfstest_suite_register_test(&suite, should_run);
    pfstest_suite_register_test(&suite, should_fail);
    pfstest_suite_register_test(&suite, should_also_run);
    pfstest_suite_run(&before_hooks, &after_hooks, &suite,
                      NULL, NULL,
                      &standard_formatter);

    if (0 != pfstest_strcmp_nv(captured_output, expected)) {
        fail("Output did not match expected output");
    }
}

test(should_report_results_verbose)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "new-core-tests.c:should_run PASS\n"
        "new-core-tests.c:should_fail FAIL\n"
        "    Location: new-core-tests.c:24\n"
        "    Expected failure, should have been caught\n"
        "new-core-tests.c:should_be_ignored IGNORED\n"
        "\n"
        "Run complete. 1 passed, 1 failed, 1 ignored\n");

    pfstest_suite_register_test(&suite, should_run);
    pfstest_suite_register_test(&suite, should_fail);
    pfstest_suite_register_test(&suite, should_be_ignored);

    pfstest_suite_run(&before_hooks, &after_hooks, &suite,
                      NULL, NULL,
                      &verbose_formatter);

    if (0 != pfstest_strcmp_nv(captured_output, expected)) {
        fail("Output did not match expected output");
    }
}

test(should_indent_multi_line_error_messages)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "new-core-tests.c:should_have_multi_line_failure FAIL\n"
        "    Location: new-core-tests.c:67\n"
        "    Expected failure, should have been caught\n"
        "    and formatted correctly\n"
        "    across multiple lines\n"
        "\n"
        "Run complete. 0 passed, 1 failed, 0 ignored\n");

    pfstest_suite_register_test(&suite, should_have_multi_line_failure);
    pfstest_suite_run(&before_hooks, &after_hooks, &suite,
                      NULL, NULL,
                      &standard_formatter);

    if (0 != pfstest_strcmp_nv(captured_output, expected)) {
        fail("Output did not match expected output");
    }
}

test(should_indent_multi_line_error_messages_verbose)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "new-core-tests.c:should_have_multi_line_failure FAIL\n"
        "    Location: new-core-tests.c:67\n"
        "    Expected failure, should have been caught\n"
        "    and formatted correctly\n"
        "    across multiple lines\n"
        "\n"
        "Run complete. 0 passed, 1 failed, 0 ignored\n");

    pfstest_suite_register_test(&suite, should_have_multi_line_failure);
    pfstest_suite_run(&before_hooks, &after_hooks, &suite,
                      NULL, NULL,
                      &verbose_formatter);

    if (0 != pfstest_strcmp_nv(captured_output, expected)) {
        fail("Output did not match expected output");
    }
}

test(should_catch_failures_in_before_hooks)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "new-core-tests.c:should_run FAIL\n"
        "    Location: new-core-tests.c:51\n"
        "    Expected failure, should have been caught\n"
        "\n"
        "Run complete. 0 passed, 1 failed, 0 ignored\n");

    pfstest_hook_list_register_hook(&before_hooks, hook_should_fail);
    pfstest_suite_register_test(&suite, should_run);

    pfstest_suite_run(&before_hooks, &after_hooks, &suite,
                      NULL, NULL,
                      &standard_formatter);

    if (0 != pfstest_strcmp_nv(captured_output, expected)) {
        fail("Output did not match expected output");
    }
}

test(should_run_after_hooks_when_test_fails)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "should_be_run_after ");

    pfstest_suite_register_test(&suite, should_fail);
    pfstest_hook_list_register_hook(&after_hooks, should_be_run_after);

    pfstest_suite_run(&before_hooks, &after_hooks, &suite,
                      NULL, NULL,
                      &standard_formatter);

    if (0 != pfstest_strcmp_nv(call_log, expected)) {
        fail("Log did not match expected log");
    }
}

test(should_catch_failures_in_after_hooks)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "new-core-tests.c:should_run FAIL\n"
        "    Location: new-core-tests.c:51\n"
        "    Expected failure, should have been caught\n"
        "\n"
        "Run complete. 0 passed, 1 failed, 0 ignored\n");

    pfstest_suite_register_test(&suite, should_run);
    pfstest_hook_list_register_hook(&after_hooks, hook_should_fail);

    pfstest_suite_run(&before_hooks, &after_hooks, &suite,
                      NULL, NULL,
                      &standard_formatter);

    if (0 != pfstest_strcmp_nv(captured_output, expected)) {
        fail("Output did not match expected output");
    }
}

test(should_catch_multiple_after_hook_failures)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "new-core-tests.c:should_run FAIL\n"
        "    Location: new-core-tests.c:51\n"
        "    Expected failure, should have been caught\n"
        "new-core-tests.c:should_run FAIL\n"
        "    Location: new-core-tests.c:56\n"
        "    Another expected failure, should have been caught\n"
        "\n"
        "Run complete. 0 passed, 1 failed, 0 ignored\n");

    pfstest_suite_register_test(&suite, should_run);
    pfstest_hook_list_register_hook(&after_hooks, hook_should_fail);
    pfstest_hook_list_register_hook(&after_hooks, hook_should_also_fail);
    
    pfstest_suite_run(&before_hooks, &after_hooks, &suite,
                      NULL, NULL,
                      &standard_formatter);

    if (0 != pfstest_strcmp_nv(captured_output, expected)) {
        fail("Output did not match expected output");
    }
}

test(should_only_count_each_failing_test_once)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "new-core-tests.c:should_fail FAIL\n"
        "    Location: new-core-tests.c:24\n"
        "    Expected failure, should have been caught\n"
        "new-core-tests.c:should_fail FAIL\n"
        "    Location: new-core-tests.c:51\n"
        "    Expected failure, should have been caught\n"
        "\n"
        "Run complete. 0 passed, 1 failed, 0 ignored\n");

    pfstest_suite_register_test(&suite, should_fail);
    pfstest_hook_list_register_hook(&after_hooks, hook_should_fail);

    pfstest_suite_run(&before_hooks, &after_hooks, &suite,
                      NULL, NULL,
                      &standard_formatter);

    if (0 != pfstest_strcmp_nv(captured_output, expected)) {
        fail("Output did not match expected output");
    }
}

test(should_return_0_on_success)
{
    int result;

    pfstest_suite_register_test(&suite, should_run);
    pfstest_suite_register_test(&suite, should_also_run);

    result = 
        pfstest_suite_run(&before_hooks, &after_hooks, &suite,
                          NULL, NULL,
                          &standard_formatter);

    if (0 != result) {
        fail("pfstest_suite_run did not return 0");
    }
}

test(should_return_fail_count_on_failure)
{
    int result;

    pfstest_suite_register_test(&suite, should_fail);
    pfstest_suite_register_test(&suite, should_also_fail);

    result = 
        pfstest_suite_run(&before_hooks, &after_hooks, &suite,
                          NULL, NULL,
                          &standard_formatter);

    if (2 != result) {
        fail("pfstest_suite_run did not return 2");
    }
}

test(should_only_call_before_hooks_in_same_file)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "should_be_run_before should_run ");
    pfstest_hook_list_register_hook(&before_hooks, should_be_run_before);
    pfstest_hook_list_register_hook(&before_hooks, other_file_hook);
    pfstest_suite_register_test(&suite, should_run);
    
    pfstest_suite_run(&before_hooks, &after_hooks, &suite,
                      NULL, NULL,
                      &standard_formatter);

    if (0 != pfstest_strcmp_nv(call_log, expected)) {
        fail("Log did not match expected log");
    }
}

test(should_only_call_after_hooks_in_same_file)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "should_run should_be_run_after ");

    pfstest_hook_list_register_hook(&after_hooks, should_be_run_after);
    pfstest_hook_list_register_hook(&after_hooks, other_file_hook);
    pfstest_suite_register_test(&suite, should_run);
    
    pfstest_suite_run(&before_hooks, &after_hooks, &suite,
                      NULL, NULL,
                      &standard_formatter);

    if (0 != pfstest_strcmp_nv(call_log, expected)) {
        fail("Log did not match expected log");
    }
}

test(should_filter_by_file_name)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "should_run ");

    pfstest_suite_register_test(&suite, should_run);
    pfstest_suite_register_test(&suite, other_file_test);
    
    pfstest_suite_run(&before_hooks, &after_hooks, &suite,
                      pfstest_nv_string(__FILE__), NULL,
                      &standard_formatter);

    if (0 != pfstest_strcmp_nv(call_log, expected)) {
        fail("Log did not match expected log");
    }
}

test(should_filter_by_test_name)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "should_run ");

    pfstest_suite_register_test(&suite, should_run);
    pfstest_suite_register_test(&suite, should_also_run);
    
    pfstest_suite_run(&before_hooks, &after_hooks, &suite,
                      NULL, pfstest_nv_string("should_run"),
                      &standard_formatter);

    if (0 != pfstest_strcmp_nv(call_log, expected)) {
        fail("Log did not match expected log");
    }
}

ignore_test(should_print_test_registrations)
{
}
