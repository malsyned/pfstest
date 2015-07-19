#include "pfstest.h"
#include "pfstest-alloc-pic.h"
#include "register-tests.h"

#include <stdio.h>

#define HEAP_SIZE ((int)2048)

#pragma udata heap
unsigned char heap[HEAP_SIZE];
#pragma udata

static int print_char(int c)
{
    return putchar(c);
}

void main(void)
{
    pfstest_arguments_t args;
    memset(&args, 0, sizeof(args));
    args.verbose = true;

    pfstest_alloc_pic_init(heap, HEAP_SIZE);

    register_tests();

    pfstest_start(print_char, &args);

    /* The mcc18 runtime restarts the program if main()
     * returns. mcc18's standard library doesn't define exit() though,
     * so we redefine it as an infinite loop in pfstest-platform.h and
     * then call it here to make sure the tests only run once. */
    exit(1);
}
