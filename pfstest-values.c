#include "pfstest-values.h"

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "pfstest-alloc.h"

#ifndef PRIdMAX
# define PRIdMAX "ld"
# define INTMAX_CAST long int
#else
#define INTMAX_CAST intmax_t
#endif

static void the_int_printer(pfstest_value_t *value)
{
    intmax_t i = *(intmax_t *)pfstest_value_data(value);

    printf("the int %" PRIdMAX, (INTMAX_CAST)i);
}

pfstest_value_t *the_int(intmax_t i)
{
    intmax_t *data = pfstest_alloc(sizeof(i));
    *data = i;
    
    return pfstest_value_new(the_int_printer, data);
}
