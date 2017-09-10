#include "pfstest-output-formatter-xml.h"

#include "pfstest-alloc.h"

typedef struct
{
    pfstest_output_formatter_t parent;
    int failed_tests;
    bool test_ignored;
    bool test_failed;
    bool fresh_line;
    bool in_message;
} xml_formatter_t;

static int print_char(
    pfstest_output_formatter_t *formatter, int c)
{
    xml_formatter_t *xml_formatter = (xml_formatter_t *)formatter;
    int i, r;

    if (xml_formatter->in_message && xml_formatter->fresh_line && c != '\n')
    {
        for (i = 0; i < 4; i++) {
            r = formatter->char_writer(' ');
            if (r != ' ')
                return r;
        }
    }

    xml_formatter->fresh_line = (c == '\n');
    return formatter->char_writer(c);
}

static void run_started(pfstest_output_formatter_t *formatter)
{
    ((xml_formatter_t *)formatter)->in_message = false;
    ((xml_formatter_t *)formatter)->failed_tests = 0;

    pfstest_output_formatter_print_nv_string(
        formatter,
        pfstest_nv_string(
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<testsuite>\n"
            ));
}

static void test_started(pfstest_output_formatter_t *formatter,
                         const pfstest_nv_ptr char *test_name,
                         const pfstest_nv_ptr char *test_file)
{
    ((xml_formatter_t *)formatter)->test_ignored = false;
    ((xml_formatter_t *)formatter)->test_failed = false;
    ((xml_formatter_t *)formatter)->in_message = false;

    pfstest_output_formatter_print_nv_string(
        formatter, pfstest_nv_string("  <test name=\""));
    pfstest_output_formatter_print_nv_string(formatter, test_name);
    pfstest_output_formatter_print_nv_string(
        formatter, pfstest_nv_string("\" file=\""));
    pfstest_output_formatter_print_nv_string(formatter, test_file);
    pfstest_output_formatter_print_nv_string(
        formatter, pfstest_nv_string("\" result=\""));
}

static void test_ignored(pfstest_output_formatter_t *formatter)
{
    ((xml_formatter_t *)formatter)->test_ignored = true;
}

static void test_failed_message_start(
    pfstest_output_formatter_t *formatter,
    const pfstest_nv_ptr char *file,
    int line)
{
    ((xml_formatter_t *)formatter)->test_failed = true;
    ((xml_formatter_t *)formatter)->in_message = true;

    pfstest_output_formatter_print_nv_string(
        formatter, pfstest_nv_string("fail\" fail_file=\""));
    pfstest_output_formatter_print_nv_string(formatter, file);
    pfstest_output_formatter_print_nv_string(
        formatter, pfstest_nv_string("\" fail_line=\""));
    pfstest_output_formatter_print_int(formatter, line);
    pfstest_output_formatter_print_nv_string(
        formatter, pfstest_nv_string("\">\n"));
}

static void test_failed_message_complete(
    pfstest_output_formatter_t *formatter)
{
    ((xml_formatter_t *)formatter)->in_message = false;
    ((xml_formatter_t *)formatter)->failed_tests++;
}

static void get_fresh_line(pfstest_output_formatter_t *formatter)
{
    if (!((xml_formatter_t *)formatter)->fresh_line) {
        print_char(formatter, '\n');
    }
}

static void test_complete(pfstest_output_formatter_t *formatter)
{
    ((xml_formatter_t *)formatter)->in_message = false;

    if (((xml_formatter_t *)formatter)->test_ignored)
    {
        pfstest_output_formatter_print_nv_string(
            formatter, pfstest_nv_string("ignored\" />\n"));
    } else if (((xml_formatter_t *)formatter)->test_failed) {
        get_fresh_line(formatter);
        pfstest_output_formatter_print_nv_string(
            formatter, pfstest_nv_string("  </test>\n"));
    } else {
        pfstest_output_formatter_print_nv_string(
            formatter, pfstest_nv_string("pass\" />\n"));
    }
}

static void run_complete(pfstest_output_formatter_t *formatter)
{
    pfstest_output_formatter_print_nv_string(
        formatter, pfstest_nv_string("</testsuite>\n"));
}

static int return_value(pfstest_output_formatter_t *formatter)
{
    return ((xml_formatter_t *)formatter)->failed_tests;
}

static const
pfstest_nv pfstest_output_formatter_vtable_t xml_vtable = {
    print_char,
    run_started,
    test_started,
    test_ignored,
    test_failed_message_start,
    test_failed_message_complete,
    test_complete,
    run_complete,
    return_value,
};

pfstest_output_formatter_t *pfstest_output_formatter_xml_new(
    int (*char_writer)(int))
{
    xml_formatter_t *formatter =
        pfstest_alloc(sizeof(*formatter));

    formatter->parent.vtable = &xml_vtable;
    formatter->parent.char_writer = char_writer;

    return (pfstest_output_formatter_t *)formatter;
}
