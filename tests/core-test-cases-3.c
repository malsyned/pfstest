#include "pfstest-core.h"

#undef __PFSTEST_FILE__
#define __PFSTEST_FILE__ "core-test-cases-3.c"

extern char call_log[];

extern int setup_hook_fail;
extern int teardown_hook_fail;

/* setup() and teardown() are at the bottom because they use #line and
 * I want accurate error location reporting in the rest of this
 * module */

pfstest_case_with_fixture(should_setup_and_teardown)
{
    pfstest_strcat_pg(call_log, pfstest_pg_str("should_setup_and_teardown "));
}

pfstest_case_with_fixture(should_fail_with_setup_and_teardown)
{
    fail("Expected failure, should have been caught");
}

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

