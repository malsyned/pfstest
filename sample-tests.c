#include <stdio.h>

#include "pfstest.h"

int pass_count = 0;
int fail_count = 0;
int ignore_count = 0;

test(passes)
{
}

test(fails)
{
    fail("failure message");
    printf("!!! BUG !!!\n");
}

ignore_test(ignored)
{
    fail("!!! BUG !!!");
}

failing_test(expected_failure)
{
    fail("!!! BUG !!!");
}

failing_test(fails_to_fail)
{
}

void test_suite(void)
{
    run_test(passes);
    run_test(fails);
    run_test(ignored);
    run_test(expected_failure);
    run_test(fails_to_fail);
}

int main(void)
{
    return run_suite(test_suite);
}
