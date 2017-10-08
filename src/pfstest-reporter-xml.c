#include "pfstest-reporter-xml.h"

#include "pfstest-alloc.h"
#include "pfstest-version.h"

typedef struct
{
    pfstest_reporter_t parent;
    int failed_tests;
    bool test_ignored;
    bool test_failed;
    bool fresh_line;
    bool in_message;
    bool in_attribute;
} xml_reporter_t;

static int print_nv_string_raw(
    pfstest_reporter_t *reporter, const pfstest_nv_ptr char *s)
{
    char c;
    int r = 0;

    while (pfstest_memcpy_nv(&c, s, sizeof(c)), c) {
        r = reporter->char_writer(c);
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
    pfstest_reporter_t *reporter,
    const pfstest_nv_ptr struct char_reference *ref_table,
    size_t table_length,
    int c)
{
    struct char_reference row;
    size_t i;

    for (i = 0; i < table_length; i++) {
        pfstest_memcpy_nv(&row, &ref_table[i], sizeof(row));
        if (row.c == c)
            return print_nv_string_raw(reporter, row.ref);
    }

    return reporter->char_writer(c);
}

static int print_char(pfstest_reporter_t *reporter, int c)
{
    xml_reporter_t *xml_reporter = (xml_reporter_t *)reporter;
    int r;

    if (xml_reporter->in_message && xml_reporter->fresh_line && c != '\n')
    {
        r = print_nv_string_raw(reporter, pfstest_nv_string("    "));
        if (r != ' ')
            return r;
    }

    xml_reporter->fresh_line = (c == '\n');

    if (xml_reporter->in_message) {
        print_escaped_char(
            reporter,
            elem_references,
            sizeof(elem_references)/sizeof(elem_references[0]),
            c);
        return c;
    }
    if (xml_reporter->in_attribute) {
        print_escaped_char(
            reporter,
            attr_references,
            sizeof(attr_references)/sizeof(attr_references[0]),
            c);
        return c;
    }

    return reporter->char_writer(c);
}

static void run_started(pfstest_reporter_t *reporter)
{
    ((xml_reporter_t *)reporter)->in_message = false;
    ((xml_reporter_t *)reporter)->in_attribute = false;
    ((xml_reporter_t *)reporter)->failed_tests = 0;

    pfstest_reporter_print_nv_string(
        reporter,
        pfstest_nv_string(
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<testsuite version=\"" PFSTEST_VERSION_STRING "\">\n"
            ));
}

static void test_started(pfstest_reporter_t *reporter,
                         const pfstest_nv_ptr char *test_name,
                         const pfstest_nv_ptr char *test_file)
{
    ((xml_reporter_t *)reporter)->test_ignored = false;
    ((xml_reporter_t *)reporter)->test_failed = false;
    ((xml_reporter_t *)reporter)->in_message = false;
    ((xml_reporter_t *)reporter)->in_attribute = false;

    pfstest_reporter_print_nv_string(
        reporter, pfstest_nv_string("  <test name=\""));
    pfstest_reporter_print_nv_string(reporter, test_name);
    pfstest_reporter_print_nv_string(
        reporter, pfstest_nv_string("\" file=\""));
    ((xml_reporter_t *)reporter)->in_attribute = true;
    pfstest_reporter_print_nv_string(reporter, test_file);
    ((xml_reporter_t *)reporter)->in_attribute = false;
    pfstest_reporter_print_nv_string(
        reporter, pfstest_nv_string("\" result=\""));
}

static void test_ignored(pfstest_reporter_t *reporter)
{
    ((xml_reporter_t *)reporter)->test_ignored = true;
}

static void test_failed_message_start(
    pfstest_reporter_t *reporter,
    const pfstest_nv_ptr char *file,
    int line)
{
    ((xml_reporter_t *)reporter)->test_failed = true;
    pfstest_reporter_print_nv_string(
        reporter, pfstest_nv_string("fail\" fail_file=\""));
    ((xml_reporter_t *)reporter)->in_attribute = true;
    pfstest_reporter_print_nv_string(reporter, file);
    ((xml_reporter_t *)reporter)->in_attribute = false;
    pfstest_reporter_print_nv_string(
        reporter, pfstest_nv_string("\" fail_line=\""));
    pfstest_reporter_print_int(reporter, line);
    pfstest_reporter_print_nv_string(reporter, pfstest_nv_string("\">\n"));
    ((xml_reporter_t *)reporter)->in_message = true;
}

static void test_failed_message_complete(
    pfstest_reporter_t *reporter)
{
    ((xml_reporter_t *)reporter)->in_message = false;
    ((xml_reporter_t *)reporter)->failed_tests++;
}

static void get_fresh_line(pfstest_reporter_t *reporter)
{
    if (!((xml_reporter_t *)reporter)->fresh_line) {
        print_char(reporter, '\n');
    }
}

static void test_complete(pfstest_reporter_t *reporter)
{
    ((xml_reporter_t *)reporter)->in_message = false;

    if (((xml_reporter_t *)reporter)->test_ignored)
    {
        pfstest_reporter_print_nv_string(
            reporter, pfstest_nv_string("ignored\" />\n"));
    } else if (((xml_reporter_t *)reporter)->test_failed) {
        get_fresh_line(reporter);
        pfstest_reporter_print_nv_string(
            reporter, pfstest_nv_string("  </test>\n"));
    } else {
        pfstest_reporter_print_nv_string(
            reporter, pfstest_nv_string("pass\" />\n"));
    }
}

static void run_complete(pfstest_reporter_t *reporter)
{
    pfstest_reporter_print_nv_string(
        reporter, pfstest_nv_string("</testsuite>\n"));
}

static int return_value(pfstest_reporter_t *reporter)
{
    return ((xml_reporter_t *)reporter)->failed_tests;
}

static const
pfstest_nv pfstest_reporter_vtable_t xml_vtable = {
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

pfstest_reporter_t *pfstest_reporter_xml_new(int (*char_writer)(int))
{
    xml_reporter_t *reporter = pfstest_alloc(sizeof(*reporter));

    reporter->parent.vtable = &xml_vtable;
    reporter->parent.char_writer = char_writer;

    return (pfstest_reporter_t *)reporter;
}