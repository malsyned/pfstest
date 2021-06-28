/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include "pfstest-core.h"

#undef __PFSTEST_FILE__
#define __PFSTEST_FILE__ "core-test-cases-2.c"

extern char call_log[];

pfstest_case(other_file_test)
{
    pfstest_strcat_pg(call_log, pfstest_pg_str("other_file_test "));
}
