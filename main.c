#include "pfstest.h"
#include "register-tests.h"

int main(int argc, char *argv[])
{
    register_tests();

    return run_tests(argc, argv);
}
