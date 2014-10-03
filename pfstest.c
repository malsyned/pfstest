#include "pfstest.h"

#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define RESULT_PASS 1
#define RESULT_FAIL 2
#define RESULT_IGNORE 3

static jmp_buf test_jmp_buf;
static bool fail_expected;

static int passed;
static int failed;
static int ignored;

void fail_with_printer(void (*printer)(const void *), const void *object)
{
    printf("FAIL");
    printf("\n");
    printer(object);
    printf("\n");
    longjmp(test_jmp_buf, RESULT_FAIL);
}

void message_printer(const void *object)
{
    const char *message = object;
    printf("    %s", message);
}

void fail(const char *message)
{
    fail_with_printer(message_printer, message);
}

static void pass(void)
{
    longjmp(test_jmp_buf, RESULT_PASS);
}

static void ignore(void)
{
    longjmp(test_jmp_buf, RESULT_IGNORE);
}

void run_test(pfstest_t *the_test)
{
    printf("%s:%d: %s() ", the_test->file, the_test->line, the_test->name);

    if (the_test->flags & PFSTEST_FLAG_EXPECT_FAIL)
        fail_expected = true;
    else
        fail_expected = false;

    switch (setjmp(test_jmp_buf)) {
        case 0:
            if (the_test->flags & PFSTEST_FLAG_IGNORED) {
                ignore();
            }
            the_test->function();
            pass();
            break;
        case RESULT_PASS:
            if (fail_expected) {
                fail_expected = false;
                fail("Test passed when failure expected");
            }

            passed++;
            printf("PASS\n");
            break;
        case RESULT_FAIL:
            if (fail_expected) {
                fail_expected = false;
                printf("    (expected failure)\n    ");
                pass();
            }
    
            failed++;
            /* Message already printed before the stack unwound */
            break;
        case RESULT_IGNORE:
            ignored++;
            printf("IGNORED\n");
            break;
        default:
            printf("FATAL ERROR\n");
            exit(1);
            break;
    }
}

list_t tests = LIST_EMPTY();

void register_test(pfstest_t *the_test)
{
    list_append(&tests, (list_node_t *)the_test);
}

list_t before = LIST_EMPTY();

void register_before(pfstest_hook_t *the_hook)
{
    list_append(&before, (list_node_t *)the_hook);
}

list_t after = LIST_EMPTY();

void register_after(pfstest_hook_t *the_hook)
{
    list_append(&after, (list_node_t *)the_hook);
}

static void do_hook_list(list_t *list)
{
    list_node_t *hook_node = list_head(list);

    while (hook_node != NULL) {
        pfstest_hook_t *hook = (pfstest_hook_t *)hook_node;
        hook->function();
        hook_node = hook_node->next;
    }
}

static void do_before_list(void)
{
    do_hook_list(&before);
}

static void do_after_list(void)
{
    do_hook_list(&after);
}

static void do_tests_list(void)
{
    list_node_t *test_node = list_head(&tests);

    while (test_node != NULL) {
        pfstest_t *test = (pfstest_t *)test_node;
        do_before_list();
        run_test(test);
        do_after_list();
        test_node = test_node->next;
    }
}

int run_tests(void)
{
    printf("PFSTest 0.1\n");
    printf("===========\n");
    do_tests_list();
    printf("\nRun complete. %d passed, %d failed, %d ignored\n",
           passed, failed, ignored);

    if (failed > 0)
        return 1;
    else
        return 0;
}
