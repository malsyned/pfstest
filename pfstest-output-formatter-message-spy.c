#include "pfstest-output-formatter-message-spy.h"

#include "pfstest-alloc.h"

typedef struct
{
    pfstest_output_formatter_t parent;
    int failed;
} message_spy_formatter_t;

static void run_started(pfstest_output_formatter_t *formatter)
{
    ((message_spy_formatter_t *)formatter)->failed = 0;
}

static void test_failed_message_complete(
    pfstest_output_formatter_t *formatter)
{
    ((message_spy_formatter_t *)formatter)->failed++;
}

static void test_started(pfstest_output_formatter_t *formatter,
                         const pfstest_nv_ptr char *test_name,
                         const pfstest_nv_ptr char *test_file)
{
}

static void test_ignored(pfstest_output_formatter_t *formatter)
{
}

static void test_failed_message_start(
    pfstest_output_formatter_t *formatter,
    const pfstest_nv_ptr char *file,
    int line)
{
}

static void test_complete(pfstest_output_formatter_t *formatter)
{
}

static void run_complete(pfstest_output_formatter_t *formatter)
{
}

static int print_char(
    pfstest_output_formatter_t *formatter, int c)
{
    return formatter->char_writer(c);
}

static int return_value(pfstest_output_formatter_t *formatter)
{
    return ((message_spy_formatter_t *)formatter)->failed;
}

static const
pfstest_nv pfstest_output_formatter_vtable_t message_spy_vtable = {
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

pfstest_output_formatter_t *pfstest_output_formatter_message_spy_new(
    int (*char_writer)(int))
{
    message_spy_formatter_t *formatter =
        pfstest_alloc(sizeof(*formatter));

    formatter->failed = 0;
    formatter->parent.char_writer = char_writer;
    formatter->parent.vtable = &message_spy_vtable;

    return (pfstest_output_formatter_t *)formatter;
}
