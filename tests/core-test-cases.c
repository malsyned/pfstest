#include "pfstest-core.h"

#undef __PFSTEST_FILE__
#define __PFSTEST_FILE__ "core-test-cases.c"

extern char call_log[];

extern int setup_hook_fail;
extern int teardown_hook_fail;

/* setup() and teardown() are at the bottom because they use #line and
 * I want accurate error location reporting in the rest of this
 * module */

pfstest_case(should_run)
{
    pfstest_strcat_pg(call_log, pfstest_pg_str("should_run "));
}

pfstest_case(should_also_run)
{
    pfstest_strcat_pg(call_log, pfstest_pg_str("should_also_run "));
}

pfstest_case_ignored(should_be_ignored)
{
    pfstest_strcat_pg(call_log, pfstest_pg_str("should_be_ignored "));
}

pfstest_case_with_fixture(should_have_fixture)
{
    pfstest_strcat_pg(call_log, pfstest_pg_str("should_have_fixture "));
}

pfstest_case_with_fixture_ignored(should_be_ignored_with_fixture)
{
    pfstest_strcat_pg(call_log,
                      pfstest_pg_str("should_be_ignored_with_fixture "));
}

/* Below this point, #line is used to ensure that the unit tests are
 * resilient against changes to unrelated parts of this file. */

pfstest_case(should_fail)
{
#line 1111
    fail("Expected failure, should have been caught");
}

pfstest_case(should_also_fail)
{
#line 2222
    fail("Another expected failure, should have been caught");
}

pfstest_case(should_have_multi_line_failure)
{
#line 3333
    fail("Expected failure, should have been caught\nand formatted correctly\nacross multiple lines");
}

pfstest_case_with_fixture(should_fail_with_fixture)
{
#line 4444
    fail("Expected failure, should have been caught");
}

setup()
{
    pfstest_strcat_pg(call_log, pfstest_pg_str("setup "));

    if (setup_hook_fail) {
#line 5555
        fail("Expected failure, should have been caught");
    }
}

teardown()
{
    pfstest_strcat_pg(call_log, pfstest_pg_str("teardown "));

    if (teardown_hook_fail) {
#line 6666
        fail("Expected failure, should have been caught");
    }
}
