#include <stdio.h>

#include "pfstest.h"

/*
 *  This port correponds to the "-W 0x20,-" command line option.
 */
#define special_output_port (*( (volatile char *)0x20))

static int dbg_putchar(char c, FILE *stream)
{
    special_output_port = c;
    return 0;
}

static FILE mystdout = FDEV_SETUP_STREAM(dbg_putchar, NULL, _FDEV_SETUP_WRITE);

void test_suite(void);

int main(void)
{
    stdout = &mystdout;
    return run_suite(test_suite);
}
