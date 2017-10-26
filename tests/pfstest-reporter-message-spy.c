#include "pfstest-reporter-message-spy.h"

#include <stdlib.h>

#include "pfstest-alloc.h"

typedef struct
{
    pfstest_reporter_t parent;
    pfstest_bool failed;
} message_spy_reporter_t;

static void run_started(pfstest_reporter_t *reporter)
{
    ((message_spy_reporter_t *)reporter)->failed = pfstest_false;
}

static void test_failed_message_complete(pfstest_reporter_t *reporter)
{
    ((message_spy_reporter_t *)reporter)->failed = pfstest_true;
}

static void test_started(pfstest_reporter_t *reporter,
                         const pfstest_nv_ptr char *test_name,
                         const pfstest_nv_ptr char *test_file)
{
    (void)reporter;
    (void)test_name;
    (void)test_file;
}

static void test_ignored(pfstest_reporter_t *reporter)
{
    (void)reporter;
}

static void test_failed_message_start(
    pfstest_reporter_t *reporter, const pfstest_nv_ptr char *file, int line)
{
    (void)reporter;
    (void)file;
    (void)line;
}

static void test_complete(pfstest_reporter_t *reporter)
{
    (void)reporter;
}

static void run_complete(pfstest_reporter_t *reporter)
{
    (void)reporter;
}

static int print_char(pfstest_reporter_t *reporter, int c)
{
    return reporter->char_writer(c);
}

static int return_value(pfstest_reporter_t *reporter)
{
    if (((message_spy_reporter_t *)reporter)->failed)
        return EXIT_FAILURE;
    else
        return EXIT_SUCCESS;
}

static const
pfstest_nv pfstest_reporter_vtable_t message_spy_vtable = {
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

pfstest_reporter_t *pfstest_reporter_message_spy_new(int (*char_writer)(int))
{
    message_spy_reporter_t *reporter = pfstest_alloc(sizeof(*reporter));

    reporter->failed = pfstest_false;
    reporter->parent.char_writer = char_writer;
    reporter->parent.vtable = &message_spy_vtable;

    return (pfstest_reporter_t *)reporter;
}
