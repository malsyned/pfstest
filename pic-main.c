#include "pfstest.h"
#include "pfstest-alloc-pic.h"
#include "register-tests.h"

#define HEAP_SIZE ((int)2048)

#pragma udata heap
unsigned char heap[HEAP_SIZE];
#pragma udata

void main(void)
{
    pfstest_alloc_pic_init(heap, HEAP_SIZE);

    register_tests();

    run_all_tests_verbose();

    /* The mcc18 runtime restarts the program if main()
     * returns. mcc18's standard library doesn't define exit() though,
     * so we redefine it as an infinite loop in pfstest-platform.h and
     * then call it here to make sure the tests only run once. */
    exit(1);
}
