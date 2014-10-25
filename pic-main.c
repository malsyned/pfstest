#include "pfstest.h"
#include "register-tests.h"

void main(void)
{
    register_tests();

    run_all_tests_verbose();

    /* The mcc18 runtime restarts the program if main()
     * returns. mcc18's standard library doesn't define exit() though,
     * so we redefine it as an infinite loop in pfstest-platform.h and
     * then call it here to make sure the tests only run once. */
    exit(1);
}
