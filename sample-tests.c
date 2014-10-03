#include <stdio.h>

#include "pfstest.h"

test(should_pass)
{
}

test(should_fail)
{
    fail("failure message");
    printf("!!! BUG !!!\n");
}

ignore_test(should_be_ignored)
{
    fail("!!! BUG !!!");
}

failing_test(should_expect_failure)
{
    fail("no surprise");
}

failing_test(should_fail_to_fail)
{
}

void test_suite(void)
{
    run_test(should_pass);
    run_test(should_fail);
    run_test(should_be_ignored);
    run_test(should_expect_failure);
    run_test(should_fail_to_fail);
}
