/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include "pfstest.h"

test(first_test)
{
    fail("Intentional failure");
}

void register_tests(void)
{
    register_test(first_test);
}
