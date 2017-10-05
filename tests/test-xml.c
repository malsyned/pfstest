#include "pfstest.h"

#include "pfstest-reporter-xml.h"

#include "output-definitions.h"
#include "capture-output.h"

static pfstest_reporter_t *xml_reporter;

before_tests(set_up_xml)
{
    capture_output_init();
    xml_reporter = pfstest_reporter_xml_new(capture_output_char);
}

test(should_print_empty_suite)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<testsuite version=\"" VERSION "\">\n"
        "</testsuite>\n"
        );

    pfstest_reporter_run_started(xml_reporter);
    pfstest_reporter_run_complete(xml_reporter);

    assert_that("Empty test suites output boilerplate start and end tags",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

test(should_write_passing_test)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<testsuite version=\"" VERSION "\">\n"
        "  <test name=\"the_test\" file=\"foo.c\" result=\"pass\" />\n"
        "</testsuite>\n"
        );

    pfstest_reporter_run_started(xml_reporter);
    pfstest_reporter_test_started(
        xml_reporter,
        pfstest_nv_string("the_test"), pfstest_nv_string("foo.c"));
    pfstest_reporter_test_complete(xml_reporter);
    pfstest_reporter_run_complete(xml_reporter);

    assert_that("A tag is generated for a passing test",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

test(should_write_ignored_test)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<testsuite version=\"" VERSION "\">\n"
        "  <test name=\"the_test\" file=\"foo.c\" result=\"ignored\" />\n"
        "</testsuite>\n"
        );

    pfstest_reporter_run_started(xml_reporter);
    pfstest_reporter_test_started(
        xml_reporter,
        pfstest_nv_string("the_test"), pfstest_nv_string("foo.c"));
    pfstest_reporter_test_ignored(xml_reporter);
    pfstest_reporter_test_complete(xml_reporter);
    pfstest_reporter_run_complete(xml_reporter);

    assert_that("A tag is generated for an ignored test",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

test(should_write_failed_test)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<testsuite version=\"" VERSION "\">\n"
        "  <test name=\"the_test\" file=\"foo.c\" result=\"fail\" "
        "fail_file=\"bar.c\" fail_line=\"47\">\n"
        "    two lines of\n"
        "    message text\n"
        "  </test>\n"
        "</testsuite>\n"
        );

    pfstest_reporter_run_started(xml_reporter);
    pfstest_reporter_test_started(
        xml_reporter,
        pfstest_nv_string("the_test"), pfstest_nv_string("foo.c"));
    pfstest_reporter_test_failed_message_start(
        xml_reporter, pfstest_nv_string("bar.c"), 47);
    pfstest_reporter_print_nv_string(
        xml_reporter, pfstest_nv_string("two lines of\n"));
    pfstest_reporter_print_nv_string(
        xml_reporter, pfstest_nv_string("message text"));
    pfstest_reporter_test_failed_message_complete(xml_reporter);
    pfstest_reporter_test_complete(xml_reporter);
    pfstest_reporter_run_complete(xml_reporter);

    assert_that("A tag with message contents is generated for a failing test",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

test(return_value_should_start_at_0)
{
    pfstest_reporter_run_started(xml_reporter);
    pfstest_reporter_run_complete(xml_reporter);

    assert_that("The return value starts at 0",
                the_int(pfstest_reporter_return_value(xml_reporter)),
                is_the_int(0));
}

test(should_return_failing_test_count)
{
    pfstest_reporter_run_started(xml_reporter);
    pfstest_reporter_test_started(
        xml_reporter,
        pfstest_nv_string("the_test"), pfstest_nv_string("foo.c"));
    pfstest_reporter_test_failed_message_start(
        xml_reporter, pfstest_nv_string("the_test"), 47);
    pfstest_reporter_test_failed_message_complete(xml_reporter);
    pfstest_reporter_test_complete(xml_reporter);
    pfstest_reporter_test_started(
        xml_reporter,
        pfstest_nv_string("the_test"), pfstest_nv_string("foo.c"));
    pfstest_reporter_test_failed_message_start(
        xml_reporter, pfstest_nv_string("the_test"), 47);
    pfstest_reporter_test_failed_message_complete(xml_reporter);
    pfstest_reporter_test_complete(xml_reporter);
    pfstest_reporter_run_complete(xml_reporter);

    assert_that("The return value matches the failing test count",
                the_int(pfstest_reporter_return_value(xml_reporter)),
                is_the_int(2));
}

test(should_write_multiple_results)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<testsuite version=\"" VERSION "\">\n"
        "  <test name=\"the_test\" file=\"foo.c\" result=\"ignored\" />\n"
        "  <test name=\"the_test\" file=\"foo.c\" result=\"pass\" />\n"
        "  <test name=\"the_test\" file=\"foo.c\" result=\"fail\" "
        "fail_file=\"bar.c\" fail_line=\"47\">\n"
        "  </test>\n"
        "</testsuite>\n"
        );

    pfstest_reporter_run_started(xml_reporter);
    pfstest_reporter_test_started(
        xml_reporter,
        pfstest_nv_string("the_test"), pfstest_nv_string("foo.c"));
    pfstest_reporter_test_ignored(xml_reporter);
    pfstest_reporter_test_complete(xml_reporter);
    pfstest_reporter_test_started(
        xml_reporter,
        pfstest_nv_string("the_test"), pfstest_nv_string("foo.c"));
    pfstest_reporter_test_complete(xml_reporter);
    pfstest_reporter_test_started(
        xml_reporter,
        pfstest_nv_string("the_test"), pfstest_nv_string("foo.c"));
    pfstest_reporter_test_failed_message_start(
        xml_reporter, pfstest_nv_string("bar.c"), 47);
    pfstest_reporter_test_failed_message_complete(xml_reporter);
    pfstest_reporter_test_complete(xml_reporter);
    pfstest_reporter_run_complete(xml_reporter);

    assert_that("Multiple test types can intermingle",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

test(should_escape_in_file_name)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<testsuite version=\"" VERSION "\">\n"
        "  <test name=\"the_test\" file=\"foo&lt;&quot;&amp;&gt;bar.c\" result=\"pass\" />\n"
        "</testsuite>\n"
        );

    pfstest_reporter_run_started(xml_reporter);
    pfstest_reporter_test_started(
        xml_reporter,
        pfstest_nv_string("the_test"), pfstest_nv_string("foo<\"&>bar.c"));
    pfstest_reporter_test_complete(xml_reporter);
    pfstest_reporter_run_complete(xml_reporter);

    assert_that("Special characters are escaped in test file names",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

test(should_escape_in_failure_file_name)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<testsuite version=\"" VERSION "\">\n"
        "  <test name=\"the_test\" file=\"foo.c\" result=\"fail\" "
        "fail_file=\"bar&lt;&quot;&amp;&gt;foo.c\" fail_line=\"47\">\n"
        "  </test>\n"
        "</testsuite>\n"
        );

    pfstest_reporter_run_started(xml_reporter);
    pfstest_reporter_test_started(
        xml_reporter,
        pfstest_nv_string("the_test"), pfstest_nv_string("foo.c"));
    pfstest_reporter_test_failed_message_start(
        xml_reporter, pfstest_nv_string("bar<\"&>foo.c"), 47);
    pfstest_reporter_test_failed_message_complete(xml_reporter);
    pfstest_reporter_test_complete(xml_reporter);
    pfstest_reporter_run_complete(xml_reporter);

    assert_that("Special characters are escaped in failure point file names",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

test(should_escape_in_message_body)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<testsuite version=\"" VERSION "\">\n"
        "  <test name=\"the_test\" file=\"foo.c\" result=\"fail\" "
        "fail_file=\"bar.c\" fail_line=\"47\">\n"
        "    &lt;\"&amp;&gt;\n"
        "  </test>\n"
        "</testsuite>\n"
        );

    pfstest_reporter_run_started(xml_reporter);
    pfstest_reporter_test_started(
        xml_reporter,
        pfstest_nv_string("the_test"), pfstest_nv_string("foo.c"));
    pfstest_reporter_test_failed_message_start(
        xml_reporter, pfstest_nv_string("bar.c"), 47);
    pfstest_reporter_print_nv_string(
        xml_reporter, pfstest_nv_string("<\"&>"));
    pfstest_reporter_test_failed_message_complete(xml_reporter);
    pfstest_reporter_test_complete(xml_reporter);
    pfstest_reporter_run_complete(xml_reporter);

    assert_that("Special characters are escaped in failure messages",
                the_string(captured_output),
                matches_the_nv_string(expected));
}
