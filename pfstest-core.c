#include "pfstest-core.h"

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

static pfstest_list_t tests = PFSTEST_LIST_EMPTY();
static pfstest_list_t before = PFSTEST_LIST_EMPTY();
static pfstest_list_t after = PFSTEST_LIST_EMPTY();

void pfstest_fail_with_printer(void (*printer)(const void *),
                               const void *object)
{
    if (fail_expected)
        printf("FAIL (expected)");
    else
        printf("FAIL");
    printf("\n");
    printer(object);
    printf("\n");
    longjmp(test_jmp_buf, RESULT_FAIL);
}

static void message_printer(const void *object)
{
    const char *message = object;
    printf("    %s", message);
}

void pfstest_fail(const char *message)
{
    pfstest_fail_with_printer(message_printer, message);
}

static void pass(void)
{
    longjmp(test_jmp_buf, RESULT_PASS);
}

static void ignore(void)
{
    longjmp(test_jmp_buf, RESULT_IGNORE);
}

static void run_test(pfstest_t *the_test)
{
    printf("%s:%s ", the_test->file, the_test->name);

    fail_expected = (the_test->flags & _PFSTEST_FLAG_EXPECT_FAIL);

    switch (setjmp(test_jmp_buf)) {
        case 0:
            if (the_test->flags & _PFSTEST_FLAG_IGNORED) {
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
                printf("    ");
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

void pfstest_register_test(pfstest_t *the_test)
{
    pfstest_list_append(&tests, (pfstest_list_node_t *)the_test);
}

void pfstest_register_before(pfstest_hook_t *the_hook)
{
    pfstest_list_append(&before, (pfstest_list_node_t *)the_hook);
}

void pfstest_register_after(pfstest_hook_t *the_hook)
{
    pfstest_list_append(&after, (pfstest_list_node_t *)the_hook);
}

static void do_hook_list(pfstest_list_t *list)
{
    pfstest_list_node_t *hook_node = pfstest_list_head(list);

    while (hook_node != NULL) {
        pfstest_hook_t *hook = (pfstest_hook_t *)hook_node;
        hook->function();
        hook_node = hook_node->next;
    }
}

static void do_tests_list(void)
{
    pfstest_list_node_t *test_node = pfstest_list_head(&tests);

    while (test_node != NULL) {
        pfstest_t *test = (pfstest_t *)test_node;
        do_hook_list(&before);
        run_test(test);
        do_hook_list(&after);
        test_node = test_node->next;
    }
}

int pfstest_run_tests(void)
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
