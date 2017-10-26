#include "pfstest-core.h"

#undef __PFSTEST_FILE__
#define __PFSTEST_FILE__ "core-test-cases-2.c"

extern char call_log[];

pfstest_hook(other_file_hook)
{
    pfstest_strcat_pg(call_log, pfstest_pg_string("other_file_hook "));
}

pfstest_case(other_file_test)
{
    pfstest_strcat_pg(call_log, pfstest_pg_string("other_file_test "));
}
