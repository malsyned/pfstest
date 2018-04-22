#include "pfstest-core.h"

#undef __PFSTEST_FILE__
#define __PFSTEST_FILE__ "core-test-cases-3.c"

extern char call_log[];

extern int setup_hook_fail;
extern int teardown_hook_fail;

setup()
{
    pfstest_strcat_pg(call_log, pfstest_pg_str("setup "));

    if (setup_hook_fail) {
#line 1
        fail("Expected failure, should have been caught");
    }
}

teardown()
{
    pfstest_strcat_pg(call_log, pfstest_pg_str("teardown "));

    if (teardown_hook_fail) {
#line 2
        fail("Expected failure, should have been caught");
    }
}

pfstest_case(should_setup_and_teardown)
{
    pfstest_strcat_pg(call_log, pfstest_pg_str("should_setup_and_teardown "));
}

pfstest_case(should_fail_with_setup_and_teardown)
{
    fail("Expected failure, should have been caught");
}
