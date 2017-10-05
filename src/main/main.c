#include "pfstest.h"
#include "pfstest-cli.h"
#include "register-tests.h"

int main(int argc, char *argv[])
{
    register_tests();

    return pfstest_main(argc, argv);
}
