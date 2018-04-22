#include "pfstest.h"

#include "capture-output.h"
#include "output-definitions.h"

char call_log[150];

static const pfstest_pg_ptr pfstest_report_colorizer_t *null_colorizer;

static pfstest_reporter_t *standard_reporter;
static pfstest_reporter_t *verbose_reporter;

static pfstest_list_t suite;
static pfstest_list_t plugins;

int setup_hook_fail = 0;
int teardown_hook_fail = 0;

setup()
{
    setup_hook_fail = 0;
    teardown_hook_fail = 0;

    capture_output_init();
    call_log[0] = '\0';

    pfstest_list_reset(&suite);
    pfstest_list_reset(&plugins);

    null_colorizer = pfstest_report_colorizer_null;

    standard_reporter = pfstest_reporter_standard_new(capture_output_char,
                                                      null_colorizer);
    verbose_reporter = pfstest_reporter_verbose_new(capture_output_char,
                                                    null_colorizer);
}

test(should_run_tests)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("should_run should_also_run ");

    pfstest_suite_register_test(&suite, should_run);
    pfstest_suite_register_test(&suite, should_also_run);

    pfstest_suite_run(&plugins, &suite, NULL, NULL, standard_reporter);

    if (0 != pfstest_strcmp_pg(call_log, expected)) {
        fail("Log did not match expected log");
    }
}

test(should_run_fixture_functions)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("setup should_have_fixture teardown ");

    pfstest_suite_register_test(&suite, should_have_fixture);

    pfstest_suite_run(&plugins, &suite, NULL, NULL, standard_reporter);

    if (0 != pfstest_strcmp_pg(call_log, expected)) {
        fail("Log did not match expected log");
    }
}

test(should_skip_ignored_tests)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("");

    pfstest_suite_register_test(&suite, should_be_ignored_with_fixture);

    pfstest_suite_run(&plugins, &suite, NULL, NULL, standard_reporter);

    if (0 != pfstest_strcmp_pg(call_log, expected)) {
        fail("Log did not match expected log");
    }
}

test(should_reset_tests)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        "should_run ");

    /* Create some lists, which modifies ->next pointers of nodes */
    pfstest_suite_register_test(&suite, should_run);
    pfstest_suite_register_test(&suite, should_also_run);

    /* Reset the lists */
    pfstest_list_reset(&suite);

    /* Add some nodes to the lists, hoping the -> next pointers are
     * handled properly */
    pfstest_suite_register_test(&suite, should_run);

    pfstest_suite_run(&plugins, &suite, NULL, NULL, standard_reporter);

    if (0 != pfstest_strcmp_pg(call_log, expected)) {
        fail("Log did not match expected log");
    }
}

test(should_report_results)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        HEADER
        "..\n"
        "Run complete. 2 passed, 0 failed, 0 ignored\n");

    pfstest_suite_register_test(&suite, should_run);
    pfstest_suite_register_test(&suite, should_also_run);

    pfstest_suite_run(&plugins, &suite, NULL, NULL, standard_reporter);

    if (0 != pfstest_strcmp_pg(captured_output, expected)) {
        fail("Output did not match expected output");
    }
}

test(should_report_ignored_tests)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        HEADER
        "I\n"
        "Run complete. 0 passed, 0 failed, 1 ignored\n");

    pfstest_suite_register_test(&suite, should_be_ignored);

    pfstest_suite_run(&plugins, &suite, NULL, NULL, standard_reporter);

    if (0 != pfstest_strcmp_pg(captured_output, expected)) {
        fail("Output did not match expected output");
    }
}

test(should_report_failed_tests)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        HEADER
        "core-test-cases.c:should_fail FAIL\n"
        "    Location: core-test-cases.c:25\n"
        "    Expected failure, should have been caught\n"
        "\n"
        "Run complete. 0 passed, 1 failed, 0 ignored\n");

    pfstest_suite_register_test(&suite, should_fail);
    pfstest_suite_run(&plugins, &suite, NULL, NULL, standard_reporter);

    if (0 != pfstest_strcmp_pg(captured_output, expected)) {
        fail("Output did not match expected output");
    }
}

test(should_report_results_including_failed_tests)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        HEADER
        ".\n"
        "core-test-cases.c:should_fail FAIL\n"
        "    Location: core-test-cases.c:25\n"
        "    Expected failure, should have been caught\n"
        ".\n"
        "Run complete. 2 passed, 1 failed, 0 ignored\n");

    pfstest_suite_register_test(&suite, should_run);
    pfstest_suite_register_test(&suite, should_fail);
    pfstest_suite_register_test(&suite, should_also_run);
    pfstest_suite_run(&plugins, &suite, NULL, NULL, standard_reporter);

    if (0 != pfstest_strcmp_pg(captured_output, expected)) {
        fail("Output did not match expected output");
    }
}

test(should_report_results_verbose)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        HEADER
        "core-test-cases.c:should_run PASS\n"
        "core-test-cases.c:should_fail FAIL\n"
        "    Location: core-test-cases.c:25\n"
        "    Expected failure, should have been caught\n"
        "core-test-cases.c:should_be_ignored IGNORED\n"
        "\n"
        "Run complete. 1 passed, 1 failed, 1 ignored\n");

    pfstest_suite_register_test(&suite, should_run);
    pfstest_suite_register_test(&suite, should_fail);
    pfstest_suite_register_test(&suite, should_be_ignored);

    pfstest_suite_run(&plugins, &suite, NULL, NULL, verbose_reporter);

    if (0 != pfstest_strcmp_pg(captured_output, expected)) {
        fail("Output did not match expected output");
    }
}

test(should_indent_multi_line_error_messages)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        HEADER
        "core-test-cases.c:should_have_multi_line_failure FAIL\n"
        "    Location: core-test-cases.c:35\n"
        "    Expected failure, should have been caught\n"
        "    and formatted correctly\n"
        "    across multiple lines\n"
        "\n"
        "Run complete. 0 passed, 1 failed, 0 ignored\n");

    pfstest_suite_register_test(&suite, should_have_multi_line_failure);
    pfstest_suite_run(&plugins, &suite, NULL, NULL, standard_reporter);

    if (0 != pfstest_strcmp_pg(captured_output, expected)) {
        fail("Output did not match expected output");
    }
}

test(should_indent_multi_line_error_messages_verbose)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        HEADER
        "core-test-cases.c:should_have_multi_line_failure FAIL\n"
        "    Location: core-test-cases.c:35\n"
        "    Expected failure, should have been caught\n"
        "    and formatted correctly\n"
        "    across multiple lines\n"
        "\n"
        "Run complete. 0 passed, 1 failed, 0 ignored\n");

    pfstest_suite_register_test(&suite, should_have_multi_line_failure);
    pfstest_suite_run(&plugins, &suite, NULL, NULL, verbose_reporter);

    if (0 != pfstest_strcmp_pg(captured_output, expected)) {
        fail("Output did not match expected output");
    }
}

test(should_catch_failures_in_setup_functions)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        HEADER
        "core-test-cases.c:should_have_fixture FAIL\n"
        "    Location: core-test-cases.c:1\n"
        "    Expected failure, should have been caught\n"
        "\n"
        "Run complete. 0 passed, 1 failed, 0 ignored\n");

    pfstest_suite_register_test(&suite, should_have_fixture);

    setup_hook_fail = 1;

    pfstest_suite_run(&plugins, &suite, NULL, NULL, standard_reporter);

    if (0 != pfstest_strcmp_pg(captured_output, expected)) {
        fail("Output did not match expected output");
    }
}

test(should_run_teardown_when_test_fails)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("setup teardown ");

    pfstest_suite_register_test(&suite, should_fail_with_fixture);

    pfstest_suite_run(&plugins, &suite, NULL, NULL, standard_reporter);

    if (0 != pfstest_strcmp_pg(call_log, expected)) {
        fail("Log did not match expected log");
    }
}

test(should_catch_failure_in_teardown)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        HEADER
        "core-test-cases.c:should_have_fixture FAIL\n"
        "    Location: core-test-cases.c:2\n"
        "    Expected failure, should have been caught\n"
        "\n"
        "Run complete. 0 passed, 1 failed, 0 ignored\n");

    pfstest_suite_register_test(&suite, should_have_fixture);

    teardown_hook_fail = 1;

    pfstest_suite_run(&plugins, &suite, NULL, NULL, standard_reporter);

    if (0 != pfstest_strcmp_pg(captured_output, expected)) {
        fail("Output did not match expected output");
    }
}

test(should_only_count_each_failing_test_once)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        HEADER
        "core-test-cases.c:should_fail_with_fixture FAIL\n"
        "    Location: core-test-cases.c:3\n"
        "    Expected failure, should have been caught\n"
        "core-test-cases.c:should_fail_with_fixture FAIL\n"
        "    Location: core-test-cases.c:2\n"
        "    Expected failure, should have been caught\n"
        "\n"
        "Run complete. 0 passed, 1 failed, 0 ignored\n");

    teardown_hook_fail = 1;

    pfstest_suite_register_test(&suite, should_fail_with_fixture);

    pfstest_suite_run(&plugins, &suite, NULL, NULL, standard_reporter);

    if (0 != pfstest_strcmp_pg(captured_output, expected)) {
        fail("Output did not match expected output");
    }
}

test(should_return_EXIT_SUCCESS_on_success)
{
    int result;

    pfstest_suite_register_test(&suite, should_run);
    pfstest_suite_register_test(&suite, should_also_run);

    result = 
        pfstest_suite_run(&plugins, &suite, NULL, NULL, standard_reporter);

    if (EXIT_SUCCESS != result) {
        fail("pfstest_suite_run did not return EXIT_SUCCESS");
    }
}

test(should_return_EXIT_FAILURE_on_failure)
{
    int result;

    pfstest_suite_register_test(&suite, should_fail);
    pfstest_suite_register_test(&suite, should_also_fail);

    result = 
        pfstest_suite_run(&plugins, &suite, NULL, NULL, standard_reporter);

    if (EXIT_FAILURE != result) {
        fail("pfstest_suite_run did not return EXIT_FAILURE");
    }
}

test(should_filter_by_file_name)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("should_run ");
    const char filter_file[] = "core-test-cases.c";

    pfstest_suite_register_test(&suite, should_run);
    pfstest_suite_register_test(&suite, other_file_test);
    
    pfstest_suite_run(&plugins, &suite,
                      filter_file, NULL, standard_reporter);

    if (0 != pfstest_strcmp_pg(call_log, expected)) {
        fail("Log did not match expected log");
    }
}

test(should_filter_by_test_name)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("should_run ");
    const char filter_name[] = "should_run";

    pfstest_suite_register_test(&suite, should_run);
    pfstest_suite_register_test(&suite, should_also_run);
    
    pfstest_suite_run(&plugins, &suite,
                      NULL, filter_name, standard_reporter);

    if (0 != pfstest_strcmp_pg(call_log, expected)) {
        fail("Log did not match expected log");
    }
}


static void plugin_setup(void)
{
    pfstest_strcat_pg(call_log, pfstest_pg_str("plugin_setup "));
}

static void plugin_checks(void)
{
    pfstest_strcat_pg(call_log, pfstest_pg_str("plugin_check "));
}

static void plugin_teardown(void)
{
    pfstest_strcat_pg(call_log, pfstest_pg_str("plugin_teardown "));
}

pfstest_plugin_define(setup_only_plugin, plugin_setup, NULL, NULL);
pfstest_plugin_define(teardown_only_plugin, NULL, NULL, plugin_teardown);
pfstest_plugin_define(plugin_with_check,
                      NULL, plugin_checks, plugin_teardown);

test(should_call_plugin_setup_hook)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("plugin_setup should_run ");

    pfstest_plugin_list_register_plugin(&plugins, setup_only_plugin);
    pfstest_suite_register_test(&suite, should_run);

    pfstest_suite_run(&plugins, &suite, NULL, NULL, standard_reporter);

    if (0 != pfstest_strcmp_pg(call_log, expected)) {
        fail("Log did not match expected log");
    }
}

test(should_call_plugin_teardown_hook)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("should_run plugin_teardown ");

    pfstest_plugin_list_register_plugin(&plugins, teardown_only_plugin);
    pfstest_suite_register_test(&suite, should_run);

    pfstest_suite_run(&plugins, &suite, NULL, NULL, standard_reporter);

    if (0 != pfstest_strcmp_pg(call_log, expected)) {
        fail("Log did not match expected log");
    }
}

test(should_call_plugin_check_hook)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("should_run plugin_check plugin_teardown ");

    pfstest_plugin_list_register_plugin(&plugins, plugin_with_check);
    pfstest_suite_register_test(&suite, should_run);

    pfstest_suite_run(&plugins, &suite, NULL, NULL, standard_reporter);

    if (0 != pfstest_strcmp_pg(call_log, expected)) {
        fail("Log did not match expected log");
    }
}
