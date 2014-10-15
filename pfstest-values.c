#include "pfstest-values.h"

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include "pfstest-alloc.h"

/* the_short */

static void the_short_printer(pfstest_value_t *value)
{
    short s = *(short *)pfstest_value_data(value);

    printf("the short %hd", s);
}

pfstest_value_t *the_short(short s)
{
    short *data = pfstest_alloc(sizeof(s));
    *data = s;

    return pfstest_value_new(the_short_printer, data, sizeof(s));
}

/* the_ushort */

static void the_ushort_printer(pfstest_value_t *value)
{
    unsigned short u = *(unsigned short *)pfstest_value_data(value);

    printf("the ushort %hu", u);
}

pfstest_value_t *the_ushort(unsigned short u)
{
    unsigned short *data = pfstest_alloc(sizeof(u));
    *data = u;

    return pfstest_value_new(the_ushort_printer, data, sizeof(u));
}

/* the_int */

static void the_int_printer(pfstest_value_t *value)
{
    int i = *(int *)pfstest_value_data(value);

    printf("the int %d", i);
}

pfstest_value_t *the_int(int i)
{
    int *data = pfstest_alloc(sizeof(i));
    *data = i;
    
    return pfstest_value_new(the_int_printer, data, sizeof(i));
}

/* the_uint */

static void the_uint_printer(pfstest_value_t *value)
{
    unsigned int u = *(unsigned int *)pfstest_value_data(value);

    printf("the uint %u", u);
}

pfstest_value_t *the_uint(unsigned int u)
{
    unsigned int *data = pfstest_alloc(sizeof(u));
    *data = u;

    return pfstest_value_new(the_uint_printer, data, sizeof(u));
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

    return pfstest_value_new(the_char_printer, data, sizeof(c));
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

    return pfstest_value_new(the_string_printer, data, strlen(s) + 1);
}

/* the_pointer */

static void the_pointer_printer(pfstest_value_t *value)
{
    void *data = pfstest_value_data(value);

    printf("the pointer <%p>", data);
}

pfstest_value_t *the_pointer(void *p)
{
    return pfstest_value_new(the_pointer_printer, p, 0);
}

/* the_memory */

static void the_memory_printer(pfstest_value_t *value)
{
    char *data = pfstest_value_data(value);
    size_t size = pfstest_value_size(value);
    size_t i;

    printf("the memory {");
    for (i = 0; i < size; i++) {
        printf("0x%02x", data[i]);
        if (i < size - 1)
            printf(", ");
    }
    printf("}");
}

pfstest_value_t *the_memory(void *m, size_t size)
{
    void *data = pfstest_alloc(size);

    memcpy(data, m, size);

    return pfstest_value_new(the_memory_printer, data, size);
}
