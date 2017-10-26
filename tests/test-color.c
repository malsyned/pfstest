#include "pfstest.h"

#include "capture-output.h"
#include "output-definitions.h"

static pfstest_report_colorizer_t *colorizer;

static pfstest_list_t suite;

static pfstest_reporter_t *standard_reporter;
static pfstest_reporter_t *verbose_reporter;

before_tests(setup_color_tests)
{
    capture_output_init();

    pfstest_list_reset(&suite);

    colorizer = pfstest_report_colorizer_ansi;

    standard_reporter =
        pfstest_reporter_standard_new(capture_output_char, colorizer);
    verbose_reporter =
        pfstest_reporter_verbose_new(capture_output_char, colorizer);
}

test(should_print_pass_summary_in_green)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_string(
        HEADER
        ".\n"
        "Run complete. \033[32m1 passed\033[m, 0 failed, 0 ignored\n");

    pfstest_suite_register_test(&suite, should_run);
    pfstest_suite_run(NULL, NULL, NULL,
                      &suite,
                      NULL, NULL,
                      standard_reporter);

    assert_that("pass count is green in the summary line",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

test(should_colorize_failures)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_string(
        HEADER
        "core-test-cases.c:should_fail \033[31mFAIL\033[m\n"
        "    Location: \033[1mcore-test-cases.c:25\033[m\n"
        "    Expected failure, should have been caught\n"
        "\n"
        "Run complete. 0 passed, \033[37;41;1m1 failed\033[m, 0 ignored\n");

    pfstest_suite_register_test(&suite, should_fail);
    pfstest_suite_run(NULL, NULL, NULL, &suite,
                      NULL, NULL,
                      standard_reporter);

    assert_that("failures are properly colorized",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

test(should_print_ignore_summary_in_yellow)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_string(
        HEADER
        "I\n"
        "Run complete. 0 passed, 0 failed, \033[33m1 ignored\033[m\n");

    pfstest_suite_register_test(&suite, should_be_ignored);
    pfstest_suite_run(NULL, NULL, NULL,
                      &suite,
                      NULL, NULL,
                      standard_reporter);

    assert_that("ignore count is yellow in the summary line",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

test(should_not_print_pass_in_green_if_failures)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_string(
        HEADER
        ".\n"
        "core-test-cases.c:should_fail \033[31mFAIL\033[m\n"
        "    Location: \033[1mcore-test-cases.c:25\033[m\n"
        "    Expected failure, should have been caught\n"
        "\n"
        "Run complete. 1 passed, \033[37;41;1m1 failed\033[m, 0 ignored\n");

    pfstest_suite_register_test(&suite, should_run);
    pfstest_suite_register_test(&suite, should_fail);
    pfstest_suite_run(NULL, NULL, NULL, &suite,
                      NULL, NULL,
                      standard_reporter);

    assert_that("passed count isn't green if there are failures",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

test(should_print_verbose_pass_in_green)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_string(
        HEADER
        "core-test-cases.c:should_run \033[32mPASS\033[m\n"
        "\n"
        "Run complete. \033[32m1 passed\033[m, 0 failed, 0 ignored\n");

    pfstest_suite_register_test(&suite, should_run);

    pfstest_suite_run(NULL, NULL, NULL, &suite,
                      NULL, NULL,
                      verbose_reporter);

    assert_that("PASS is green in verbose mode",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

test(should_print_verbose_ignore_in_yellow)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_string(
        HEADER
        "core-test-cases.c:should_be_ignored \033[33mIGNORED\033[m\n"
        "\n"
        "Run complete. 0 passed, 0 failed, \033[33m1 ignored\033[m\n");

    pfstest_suite_register_test(&suite, should_be_ignored);

    pfstest_suite_run(NULL, NULL, NULL, &suite,
                      NULL, NULL,
                      verbose_reporter);

    assert_that("IGNORED is yellow in verbose mode",
                the_string(captured_output),
                matches_the_pg_string(expected));
}
