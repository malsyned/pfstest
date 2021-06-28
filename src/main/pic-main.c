/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include "pfstest.h"
#include "pfstest-alloc-pic.h"
#include "register-tests.h"

#include <stdio.h>

#pragma config WDT = OFF

#define HEAP_SIZE ((int)0x700)

#pragma udata heap
unsigned char heap[HEAP_SIZE];
#pragma udata

static int print_char(int c)
{
    return putc(c, stdout);
}

void main(void)
{
    pfstest_reporter_t *reporter;

    pfstest_alloc_pic_init(heap, HEAP_SIZE);

    register_tests();

    reporter = pfstest_reporter_verbose_new(
        print_char, pfstest_report_colorizer_null);

    pfstest_run_registered_tests(NULL, NULL, reporter);

    /* Free the memory allocated for the reporter created above */
    pfstest_alloc_free_frame();

    /* The mcc18 runtime restarts the program if main()
     * returns. mcc18's standard library doesn't define exit() though,
     * so we redefine it as an infinite loop in pfstest-platform.h and
     * then call it here to make sure the tests only run once. */
    exit(1);
}
