#include "pfstest-values.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pfstest-platform.h"
#include "pfstest-alloc.h"

/* the_short */

static void the_short_printer(pfstest_output_formatter_t *formatter,
                              pfstest_value_t *value)
{
    short s = *(short *)pfstest_value_data(value);

    pfstest_output_formatter_message_print_nv_string(
        formatter, pfstest_nv_string("the short "));
    pfstest_output_formatter_message_print_int(formatter, (intmax_t)s);
}

pfstest_value_t *pfstest_the_short(short s)
{
    short *data = pfstest_alloc(sizeof(s));
    *data = s;

    return pfstest_value_new(the_short_printer, data, sizeof(s));
}

/* the_ushort */

static void the_ushort_printer(pfstest_output_formatter_t *formatter,
                               pfstest_value_t *value)
{
    unsigned short u = *(unsigned short *)pfstest_value_data(value);

    pfstest_output_formatter_message_print_nv_string(
        formatter, pfstest_nv_string("the ushort "));
    pfstest_output_formatter_message_print_uint(
        formatter, (uintmax_t)u, 10, 0);
}

pfstest_value_t *pfstest_the_ushort(unsigned short u)
{
    unsigned short *data = pfstest_alloc(sizeof(u));
    *data = u;

    return pfstest_value_new(the_ushort_printer, data, sizeof(u));
}

/* the_int */

static void the_int_printer(pfstest_output_formatter_t *formatter,
                            pfstest_value_t *value)
{
    int i = *(int *)pfstest_value_data(value);

    pfstest_output_formatter_message_print_nv_string(
        formatter, pfstest_nv_string("the int "));
    pfstest_output_formatter_message_print_int(formatter, (intmax_t)i);
}

pfstest_value_t *pfstest_the_int(int i)
{
    int *data = pfstest_alloc(sizeof(i));
    *data = i;
    
    return pfstest_value_new(the_int_printer, data, sizeof(i));
}

/* the_uint */

static void the_uint_printer(pfstest_output_formatter_t *formatter,
                             pfstest_value_t *value)
{
    unsigned int u = *(unsigned int *)pfstest_value_data(value);

    pfstest_output_formatter_message_print_nv_string(
        formatter, pfstest_nv_string("the uint "));
    pfstest_output_formatter_message_print_uint(
        formatter, (uintmax_t)u, 10, 0);
}

pfstest_value_t *pfstest_the_uint(unsigned int u)
{
    unsigned int *data = pfstest_alloc(sizeof(u));
    *data = u;

    return pfstest_value_new(the_uint_printer, data, sizeof(u));
}

/* the_bool */

static void the_bool_printer(pfstest_output_formatter_t *formatter,
                             pfstest_value_t *value)
{
    bool b = *(bool *)pfstest_value_data(value);

    pfstest_output_formatter_message_print_nv_string(
        formatter, pfstest_nv_string("the bool "));
    pfstest_output_formatter_message_print_nv_string(
        formatter,
        b ? pfstest_nv_string("<true>") : pfstest_nv_string("<false>"));
}

pfstest_value_t *pfstest_the_bool(bool b)
{
    unsigned int *data = pfstest_alloc(sizeof(b));
    *data = b;

    return pfstest_value_new(the_bool_printer, data, sizeof(b));
}

/* the_char */

static void the_char_printer(pfstest_output_formatter_t *formatter,
                             pfstest_value_t *value)
{
    char c = *(char *)pfstest_value_data(value);

    pfstest_output_formatter_message_print_nv_string(
        formatter, pfstest_nv_string("the char '"));
    pfstest_output_formatter_message_print_char(formatter, c);
    pfstest_output_formatter_message_print_nv_string(
        formatter, pfstest_nv_string("'"));
}

pfstest_value_t *pfstest_the_char(char c)
{
    char *data = pfstest_alloc(sizeof(c));
    *data = c;

    return pfstest_value_new(the_char_printer, data, sizeof(c));
}

/* the_string */

static void the_string_printer(pfstest_output_formatter_t *formatter,
                               pfstest_value_t *value)
{
    char *data = pfstest_value_data(value);
    char *p = data;

    pfstest_output_formatter_message_print_nv_string(
        formatter, pfstest_nv_string("the string \""));
    
    while (*p) {
        pfstest_output_formatter_message_print_escaped_char(formatter, *p++);
    }

    pfstest_output_formatter_message_print_nv_string(
        formatter, pfstest_nv_string("\""));
}

pfstest_value_t *pfstest_the_string(char *s)
{
    char *data = pfstest_alloc(strlen(s) + 1);
    strcpy(data, s);

    return pfstest_value_new(the_string_printer, data, strlen(s) + 1);
}

/* the_pointer */

static void the_pointer_printer(pfstest_output_formatter_t *formatter,
                                pfstest_value_t *value)
{
    void *data = pfstest_value_data(value);

    pfstest_output_formatter_message_print_nv_string(
        formatter, pfstest_nv_string("the pointer <"));

    if (data == NULL) {
        pfstest_output_formatter_message_print_nv_string(
            formatter, pfstest_nv_string("NULL"));
    } else {
    pfstest_output_formatter_message_print_nv_string(
        formatter, pfstest_nv_string("0x"));
        pfstest_output_formatter_message_print_uint(
            formatter, (uintmax_t)(uintptr_t)data, 16, 0);
    }

    pfstest_output_formatter_message_print_nv_string(
        formatter, pfstest_nv_string(">"));

}

pfstest_value_t *pfstest_the_pointer(void *p)
{
    return pfstest_value_new(the_pointer_printer, p, 0);
}

/* the_memory */

static void the_memory_printer(pfstest_output_formatter_t *formatter,
                               pfstest_value_t *value)
{
    unsigned char *data = pfstest_value_data(value);
    size_t size = pfstest_value_size(value);
    size_t i;

    pfstest_output_formatter_message_print_nv_string(
        formatter, pfstest_nv_string("the memory {"));

    for (i = 0; i < size; i++) {

        pfstest_output_formatter_message_print_nv_string(
            formatter, pfstest_nv_string("0x"));
        pfstest_output_formatter_message_print_uint(
            formatter, data[i], 16, 2);

        if (i < size - 1) {
            pfstest_output_formatter_message_print_nv_string(
                formatter, pfstest_nv_string(", "));
        }
    }
    pfstest_output_formatter_message_print_nv_string(
        formatter, pfstest_nv_string("}"));
}

pfstest_value_t *pfstest_the_memory(void *m, size_t size)
{
    void *data = pfstest_alloc(size);

    memcpy(data, m, size);

    return pfstest_value_new(the_memory_printer, data, size);
}
