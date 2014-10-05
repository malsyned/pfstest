#include "pfstest-values.h"

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include "pfstest-alloc.h"

#ifndef PRIdMAX
# define PRIdMAX "ld"
# define INTMAX_CAST long int
#else
#define INTMAX_CAST intmax_t
#endif

/* the_int */

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

/* the_char */

static void the_char_printer(pfstest_value_t *value)
{
    char c = *(char *)pfstest_value_data(value);

    printf("the char '%c'", c);
}

pfstest_value_t *the_char(char c)
{
    char *data = pfstest_alloc(sizeof(c));
    *data = c;

    return pfstest_value_new(the_char_printer, data);
}

/* the_string */

static void the_string_printer(pfstest_value_t *value)
{
    char *data = pfstest_value_data(value);

    printf("the string \"%s\"", data);
}

pfstest_value_t *the_string(char *s)
{
    char *data = pfstest_alloc(strlen(s) + 1);
    strcpy(data, s);

    return pfstest_value_new(the_string_printer, data);
}

/* the_pointer */

static void the_pointer_printer(pfstest_value_t *value)
{
    void *data = pfstest_value_data(value);

    printf("the pointer %p", data);
}

pfstest_value_t *the_pointer(void *p)
{
    return pfstest_value_new(the_pointer_printer, p);
}
