#include <malloc.h>
#include <assert.h>

#include "pfstest.h"

int main(int argc, char *argv[])
{
    int r = pfstest_main(argc, argv);

    struct mallinfo mi = mallinfo();

    assert(mi.uordblks == 0);

    return r;
}
