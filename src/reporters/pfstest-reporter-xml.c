/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include "pfstest-reporter-xml.h"

#include <stdlib.h>

#include "pfstest-alloc.h"
#include "pfstest-version.h"

typedef struct
{
    pfstest_reporter_t parent;
    pfstest_bool failed;
    pfstest_bool test_ignored;
    pfstest_bool test_failed;
    pfstest_bool fresh_line;
    pfstest_bool in_message;
    pfstest_bool in_attribute;
} xml_reporter_t;

static int print_pg_str_raw(pfstest_reporter_t *reporter,
                            const pfstest_pg_ptr char *s)
{
    char c;
    int r = 0;

    while (PFSTEST_READ_RETURN_PG(c, *s++)) {
        r = reporter->char_writer(c);
        if (r != c)
            return r;
    }

    return r;
}

struct char_reference
{
    char c;
    const pfstest_pg_ptr char *ref;
};

static const pfstest_pg char lt_ref[] = "&lt;";
static const pfstest_pg char gt_ref[] = "&gt;";
static const pfstest_pg char amp_ref[] = "&amp;";
static const pfstest_pg char quot_ref[] = "&quot;";

const pfstest_pg struct char_reference attr_references[] = {
    {'<', lt_ref},
    {'>', gt_ref},
    {'&', amp_ref},
    {'"', quot_ref}
};

const pfstest_pg struct char_reference elem_references[] = {
    {'<', lt_ref},
    {'>', gt_ref},
    {'&', amp_ref}
};

static int print_escaped_char(
    pfstest_reporter_t *reporter,
    const pfstest_pg_ptr struct char_reference *ref_table,
    size_t table_length,
    int c)
{
    struct char_reference row;
    size_t i;

    for (i = 0; i < table_length; i++) {
        PFSTEST_READ_PG(row, ref_table[i]);
        if (row.c == c)
            return print_pg_str_raw(reporter, row.ref);
    }

    return reporter->char_writer(c);
}

static int print_char(pfstest_reporter_t *reporter, int c)
{
    xml_reporter_t *xml_reporter = (xml_reporter_t *)reporter;
    int r;

    if (xml_reporter->in_message && xml_reporter->fresh_line && c != '\n')
    {
        r = print_pg_str_raw(reporter, pfstest_pg_str("    "));
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
    } else if (xml_reporter->in_attribute) {
        print_escaped_char(
            reporter,
            attr_references,
            sizeof(attr_references)/sizeof(attr_references[0]),
            c);
        return c;
    } else {
        return reporter->char_writer(c);
    }
}

static void run_started(pfstest_reporter_t *reporter)
{
    xml_reporter_t *xml_reporter = (xml_reporter_t *)reporter;

    xml_reporter->in_message = pfstest_false;
    xml_reporter->in_attribute = pfstest_false;
    xml_reporter->failed = pfstest_false;

    pfstest_reporter_print_pg_str(
        reporter,
        pfstest_pg_str(
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<testsuite version=\"" PFSTEST_VERSION_STRING "\">\n"
            ));
}

static void test_started(pfstest_reporter_t *reporter,
                         const pfstest_pg_ptr char *test_name,
                         const pfstest_pg_ptr char *test_file)
{
    xml_reporter_t *xml_reporter = (xml_reporter_t *)reporter;

    xml_reporter->test_ignored = pfstest_false;
    xml_reporter->test_failed = pfstest_false;
    xml_reporter->in_message = pfstest_false;
    xml_reporter->in_attribute = pfstest_false;

    pfstest_reporter_print_pg_str(reporter,
                                  pfstest_pg_str("  <test name=\""));
    pfstest_reporter_print_pg_str(reporter, test_name);
    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("\" file=\""));
    xml_reporter->in_attribute = pfstest_true;
    pfstest_reporter_print_pg_str(reporter, test_file);
    xml_reporter->in_attribute = pfstest_false;
    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("\" result=\""));
}

static void test_ignored(pfstest_reporter_t *reporter)
{
    xml_reporter_t *xml_reporter = (xml_reporter_t *)reporter;

    xml_reporter->test_ignored = pfstest_true;
}

static void test_failed_message_start(
    pfstest_reporter_t *reporter,
    const pfstest_pg_ptr char *file,
    int line)
{
    xml_reporter_t *xml_reporter = (xml_reporter_t *)reporter;

    xml_reporter->test_failed = pfstest_true;
    pfstest_reporter_print_pg_str(reporter,
                                  pfstest_pg_str("fail\" fail_file=\""));
    xml_reporter->in_attribute = pfstest_true;
    pfstest_reporter_print_pg_str(reporter, file);
    xml_reporter->in_attribute = pfstest_false;
    pfstest_reporter_print_pg_str(reporter,
                                  pfstest_pg_str("\" fail_line=\""));
    pfstest_reporter_print_int(reporter, line);
    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("\">\n"));
    xml_reporter->in_message = pfstest_true;
}

static void test_failed_message_complete(
    pfstest_reporter_t *reporter)
{
    xml_reporter_t *xml_reporter = (xml_reporter_t *)reporter;

    xml_reporter->in_message = pfstest_false;
    xml_reporter->failed = pfstest_true;
}

static void get_fresh_line(pfstest_reporter_t *reporter)
{
    xml_reporter_t *xml_reporter = (xml_reporter_t *)reporter;

    if (!xml_reporter->fresh_line)
        print_char(reporter, '\n');
}

static void test_complete(pfstest_reporter_t *reporter)
{
    xml_reporter_t *xml_reporter = (xml_reporter_t *)reporter;

    xml_reporter->in_message = pfstest_false;

    if (xml_reporter->test_ignored)
    {
        pfstest_reporter_print_pg_str(reporter,
                                      pfstest_pg_str("ignored\" />\n"));
    } else if (xml_reporter->test_failed) {
        get_fresh_line(reporter);
        pfstest_reporter_print_pg_str(reporter,
                                      pfstest_pg_str("  </test>\n"));
    } else {
        pfstest_reporter_print_pg_str(reporter,
                                      pfstest_pg_str("pass\" />\n"));
    }
}

static void run_complete(pfstest_reporter_t *reporter)
{
    pfstest_reporter_print_pg_str(reporter,
                                  pfstest_pg_str("</testsuite>\n"));
}

static int return_value(pfstest_reporter_t *reporter)
{
    xml_reporter_t *xml_reporter = (xml_reporter_t *)reporter;

    if (xml_reporter->failed)
        return EXIT_FAILURE;
    else
        return EXIT_SUCCESS;
}

static const
pfstest_pg pfstest_reporter_vtable_t xml_vtable = {
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
