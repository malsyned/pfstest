/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include "pfstest.h"

/* These are very minimal tests, mostly to make sure that the headers
 * are included, the names are defined, and types are respected, and
 * to demonstrate how the shorthand macros are used. */

test(should_assert_true)
{
    assert_true("assert_true passes on true input", pfstest_true);
}

test(should_assert_false)
{
    assert_false("assert_false passes on false input", pfstest_false);
}

test(should_assert_null)
{
    assert_null("assert_null passes on NULL pointers", NULL);
}
