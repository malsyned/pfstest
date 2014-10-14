#include "pfstest.h"

#include <stdio.h>

extern int before1_called;

test(should_only_call_before_tests_hooks_in_same_file)
{
    if (before1_called > 0)
        fail("before1 called outside of its file");
}

test(should_only_call_after_tests_hooks_in_same_file)
{
    if (before1_called < 0)
        fail("after1 called outside of its file");
}
