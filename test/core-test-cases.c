#include "pfstest-core.h"

extern char call_log[];

pfstest_case(should_run)
{
    pfstest_strcat_nv(call_log, pfstest_nv_string("should_run "));
}

pfstest_case(should_also_run)
{
    pfstest_strcat_nv(call_log, pfstest_nv_string("should_also_run "));
}

pfstest_case_ignored(should_be_ignored)
{
    pfstest_strcat_nv(call_log, pfstest_nv_string("should_be_ignored "));
}

pfstest_case(should_fail)
{
    fail("Expected failure, should have been caught");
}

pfstest_hook(should_be_run_before)
{
    pfstest_strcat_nv(call_log, pfstest_nv_string("should_be_run_before "));
}

pfstest_hook(should_also_be_run_before)
{
    pfstest_strcat_nv(call_log,
                      pfstest_nv_string("should_also_be_run_before "));
}

pfstest_hook(should_be_run_after)
{
    pfstest_strcat_nv(call_log, pfstest_nv_string("should_be_run_after "));
}

pfstest_hook(should_also_be_run_after)
{
    pfstest_strcat_nv(call_log,
                      pfstest_nv_string("should_also_be_run_after "));
}

pfstest_hook(hook_should_fail)
{
    fail("Expected failure, should have been caught");
}

pfstest_hook(hook_should_also_fail)
{
    fail("Another expected failure, should have been caught");
}

pfstest_case(should_also_fail)
{
    fail("Another expected failure, should have been caught");
}

pfstest_case(should_have_multi_line_failure)
{
    fail("Expected failure, should have been caught\nand formatted correctly\nacross multiple lines");
}
