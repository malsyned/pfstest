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

int before1_called = 0;
int before2_called = 0;

before_tests(before1)
{
    before1_called++;
}

before_tests(before2)
{
    before2_called++;
}

after_tests(after1)
{
    before1_called--;
}

after_tests(after2)
{
    before2_called--;
}

test(should_call_befores_and_afters)
{
    if (before1_called == 0)
        fail("before1 not called");
    if (before1_called != 1)
        fail("after1 not called");
    if (before2_called == 0)
        fail("before2 not called");
    if (before2_called != 1)
        fail("after2 not called");
}
