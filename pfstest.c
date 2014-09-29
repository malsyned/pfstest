#include "pfstest.h"

#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>

#define RESULT_FAIL 1

jmp_buf test_jmp_buf;

static int passed;
static int failed;
static int ignored;

void fail(const char *message)
{
    printf("FAIL: %s\n", message);
    longjmp(test_jmp_buf, RESULT_FAIL);
}

void run_test(struct pfstest *the_test)
{
    printf("%s (%s:%d) ", the_test->name, the_test->file, the_test->line);

    if (the_test->flags & PFSTEST_FLAG_IGNORED) {
        printf("IGNORED\n");
        ignored++;
    } else if (setjmp(test_jmp_buf) == 0) {
        the_test->function();
        if (the_test->flags & PFSTEST_FLAG_EXPECT_FAIL) {
            fail("Test passed, expecting failure");
        }
        passed++;
        printf("PASS\n");
    } else {
        failed++;
    }
}

int run_suite(void (*the_suite)(void))
{
    printf("PFSTest 0.1\n");
    printf("===========\n");
    the_suite();
    printf("\nRun complete. %d passed, %d failed, %d ignored\n",
           passed, failed, ignored);

    if (failed > 0)
        return 1;
    else
        return 0;
}
