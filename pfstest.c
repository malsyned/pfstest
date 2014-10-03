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

void run_test(struct pfstest *the_test)
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

struct list tests = LIST_EMPTY();

void register_test(struct pfstest *the_test)
{
    list_append(&tests, (struct list_node *)the_test);
}

int run_tests(void)
{
    struct list_node *test_node = list_head(&tests);

    printf("PFSTest 0.1\n");
    printf("===========\n");
    while (test_node != NULL) {
        struct pfstest *test = (struct pfstest *)test_node;
        run_test(test);
        test_node = test_node->next;
    }
    printf("\nRun complete. %d passed, %d failed, %d ignored\n",
           passed, failed, ignored);

    if (failed > 0)
        return 1;
    else
        return 0;
}
