#include "pfstest-fp-printf.h"

#include <stdio.h>

void pfstest_fp_printf_printer(double n, pfstest_reporter_t *reporter)
{
    char buf[32];
    char *p = buf;

    sprintf(buf, "%g", n);

    while (*p)
        pfstest_reporter_print_char(reporter, *p++);
}
