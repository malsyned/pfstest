#include "pfstest-output-formatter-xml.h"

#include "pfstest-alloc.h"
#include "pfstest-version.h"

typedef struct
{
    pfstest_output_formatter_t parent;
    int failed_tests;
    bool test_ignored;
    bool test_failed;
    bool fresh_line;
    bool in_message;
    bool in_attribute;
} xml_formatter_t;

static int print_nv_string_raw(
    pfstest_output_formatter_t *formatter, const pfstest_nv_ptr char *s)
{
    char c;
    int r = 0;

    while (pfstest_memcpy_nv(&c, s, sizeof(c)), c) {
        r = formatter->char_writer(c);
        if (r != c)
            return r;
        s++;
    }

    return r;
}

struct char_reference
{
    char c;
    const pfstest_nv_ptr char *ref;
};

static const pfstest_nv char lt_ref[] = "&lt;";
static const pfstest_nv char gt_ref[] = "&gt;";
static const pfstest_nv char amp_ref[] = "&amp;";
static const pfstest_nv char quot_ref[] = "&quot;";

const pfstest_nv struct char_reference attr_references[] = {
    {'<', lt_ref},
    {'>', gt_ref},
    {'&', amp_ref},
    {'"', quot_ref}
};

const pfstest_nv struct char_reference elem_references[] = {
    {'<', lt_ref},
    {'>', gt_ref},
    {'&', amp_ref}
};

static int print_escaped_char(
    pfstest_output_formatter_t *formatter,
    const pfstest_nv_ptr struct char_reference *ref_table,
    size_t table_length,
    int c)
{
    struct char_reference row;
    size_t i;

    for (i = 0; i < table_length; i++) {
        pfstest_memcpy_nv(&row, &ref_table[i], sizeof(row));
        if (row.c == c)
            return print_nv_string_raw(formatter, row.ref);
    }

    return formatter->char_writer(c);
}

static int print_char(
    pfstest_output_formatter_t *formatter, int c)
{
    xml_formatter_t *xml_formatter = (xml_formatter_t *)formatter;
    int r;

    if (xml_formatter->in_message && xml_formatter->fresh_line && c != '\n')
    {
        r = print_nv_string_raw(formatter, pfstest_nv_string("    "));
        if (r != ' ')
            return r;
    }

    xml_formatter->fresh_line = (c == '\n');

    if (xml_formatter->in_message) {
        print_escaped_char(
            formatter,
            elem_references,
            sizeof(elem_references)/sizeof(elem_references[0]),
            c);
        return c;
    }
    if (xml_formatter->in_attribute) {
        print_escaped_char(
            formatter,
            attr_references,
            sizeof(attr_references)/sizeof(attr_references[0]),
            c);
        return c;
    }

    return formatter->char_writer(c);
}

static void run_started(pfstest_output_formatter_t *formatter)
{
    ((xml_formatter_t *)formatter)->in_message = false;
    ((xml_formatter_t *)formatter)->in_attribute = false;
    ((xml_formatter_t *)formatter)->failed_tests = 0;

    pfstest_output_formatter_print_nv_string(
        formatter,
        pfstest_nv_string(
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<testsuite version=\"" PFSTEST_VERSION_STRING "\">\n"
            ));
}

static void test_started(pfstest_output_formatter_t *formatter,
                         const pfstest_nv_ptr char *test_name,
                         const pfstest_nv_ptr char *test_file)
{
    ((xml_formatter_t *)formatter)->test_ignored = false;
    ((xml_formatter_t *)formatter)->test_failed = false;
    ((xml_formatter_t *)formatter)->in_message = false;
    ((xml_formatter_t *)formatter)->in_attribute = false;

    pfstest_output_formatter_print_nv_string(
        formatter, pfstest_nv_string("  <test name=\""));
    pfstest_output_formatter_print_nv_string(formatter, test_name);
    pfstest_output_formatter_print_nv_string(
        formatter, pfstest_nv_string("\" file=\""));
    ((xml_formatter_t *)formatter)->in_attribute = true;
    pfstest_output_formatter_print_nv_string(formatter, test_file);
    ((xml_formatter_t *)formatter)->in_attribute = false;
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
    pfstest_output_formatter_print_nv_string(
        formatter, pfstest_nv_string("fail\" fail_file=\""));
    ((xml_formatter_t *)formatter)->in_attribute = true;
    pfstest_output_formatter_print_nv_string(formatter, file);
    ((xml_formatter_t *)formatter)->in_attribute = false;
    pfstest_output_formatter_print_nv_string(
        formatter, pfstest_nv_string("\" fail_line=\""));
    pfstest_output_formatter_print_int(formatter, line);
    pfstest_output_formatter_print_nv_string(
        formatter, pfstest_nv_string("\">\n"));
    ((xml_formatter_t *)formatter)->in_message = true;
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
