#include "pfstest.h"
#include "square.h"

test(should_square_numbers)
{
    if (square(5) != 25) {
        fail("square(5) didn't equal 25");
    }
}

void register_tests(void)
{
    register_test(should_square_numbers);
}
