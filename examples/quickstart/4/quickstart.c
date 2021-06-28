/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include "pfstest.h"
#include "square.h"
#include "cube.h"

test(should_square_numbers)
{
    if (square(5) != 25) {
        fail("square(5) didn't equal 25");
    }
}

test(should_cube_numbers)
{
    assert_that("5 cubed is 125", the_int(cube(5)), equal_to(the_int(125)));
}

void register_tests(void)
{
    register_test(should_square_numbers);
    register_test(should_cube_numbers);
}
