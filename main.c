#include <malloc.h>
#include <assert.h>

#include "pfstest.h"

int main(void)
{
    int r = run_tests();

    struct mallinfo mi = mallinfo();

    assert(mi.uordblks == 0);

    return r;
}
