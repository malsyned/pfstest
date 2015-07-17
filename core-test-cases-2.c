#include "pfstest-core.h"

extern char call_log[];

pfstest_hook(other_file_hook)
{
    pfstest_strcat_nv(call_log, pfstest_nv_string("other_file_hook "));
}

pfstest_case(other_file_test)
{
    pfstest_strcat_nv(call_log, pfstest_nv_string("other_file_test "));
}
