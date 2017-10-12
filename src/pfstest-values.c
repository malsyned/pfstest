#include "pfstest-values.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pfstest-platform.h"
#include "pfstest-alloc.h"

/* the_short */

static void the_short_printer(pfstest_reporter_t *reporter,
                              pfstest_value_t *value)
{
    short s = *(const short *)pfstest_value_data(value);

    pfstest_reporter_print_nv_string(
        reporter, pfstest_nv_string("the short "));
    pfstest_reporter_print_int(reporter, (intmax_t)s);
}

pfstest_value_t *pfstest_the_short(short s)
{
    short *data = pfstest_alloc(sizeof(s));
    *data = s;

    return pfstest_value_new(the_short_printer, data, sizeof(s));
}

/* the_ushort */

static void the_ushort_printer(pfstest_reporter_t *reporter,
                               pfstest_value_t *value)
{
    unsigned short u = *(const unsigned short *)pfstest_value_data(value);

    pfstest_reporter_print_nv_string(
        reporter, pfstest_nv_string("the ushort "));
    pfstest_reporter_print_uint(reporter, (uintmax_t)u, 10, 0);
}

pfstest_value_t *pfstest_the_ushort(unsigned short u)
{
    unsigned short *data = pfstest_alloc(sizeof(u));
    *data = u;

    return pfstest_value_new(the_ushort_printer, data, sizeof(u));
}

/* the_int */

static void the_int_printer(pfstest_reporter_t *reporter,
                            pfstest_value_t *value)
{
    int i = *(const int *)pfstest_value_data(value);

    pfstest_reporter_print_nv_string(
        reporter, pfstest_nv_string("the int "));
    pfstest_reporter_print_int(reporter, (intmax_t)i);
}

pfstest_value_t *pfstest_the_int(int i)
{
    int *data = pfstest_alloc(sizeof(i));
    *data = i;
    
    return pfstest_value_new(the_int_printer, data, sizeof(i));
}

/* the_uint */

static void the_uint_printer(pfstest_reporter_t *reporter,
                             pfstest_value_t *value)
{
    unsigned int u = *(const unsigned int *)pfstest_value_data(value);

    pfstest_reporter_print_nv_string(
        reporter, pfstest_nv_string("the uint "));
    pfstest_reporter_print_uint(reporter, (uintmax_t)u, 10, 0);
}

pfstest_value_t *pfstest_the_uint(unsigned int u)
{
    unsigned int *data = pfstest_alloc(sizeof(u));
    *data = u;

    return pfstest_value_new(the_uint_printer, data, sizeof(u));
}

/* the_enum */

struct enum_aux
{
    const pfstest_nv_ptr char *const pfstest_nv_ptr *name_map;
};

static void the_enum_printer(pfstest_reporter_t *reporter,
                             pfstest_value_t *value)
{
    int e = *(const int *)pfstest_value_data(value);
    struct enum_aux *aux = pfstest_value_aux(value);
    const pfstest_nv_ptr char *const pfstest_nv_ptr *name_map = aux->name_map;
    const pfstest_nv_ptr char *const pfstest_nv_ptr *name_p;
    const pfstest_nv_ptr char *name;
    int count = 0;
    bool in_range = false;

    for (count = 0, name_p = name_map;
         pfstest_memcpy_nv(&name, name_p, sizeof(name)), name != NULL;
         count++, name_p++)
    {
        if (count == e) {
            in_range = true;
            break;
        }
    }

    pfstest_reporter_print_nv_string(reporter,
                                     pfstest_nv_string("the enum "));
    if (in_range) {
        pfstest_reporter_print_nv_string(reporter, name);
    } else {
        pfstest_reporter_print_int(reporter, e);
    }

    if (!in_range) {
        pfstest_reporter_print_nv_string(
            reporter, pfstest_nv_string(" [out of range]"));
    }
}

pfstest_value_t *pfstest_the_enum(
    int e, const pfstest_nv_ptr char *const pfstest_nv_ptr *name_map)
{
    int *data = pfstest_alloc(sizeof(e));
    struct enum_aux *aux = pfstest_alloc(sizeof(*aux));
    *data = e;
    aux->name_map = name_map;

    return pfstest_value_new_with_aux(the_enum_printer, data, sizeof(e), aux);
}

/* the_bool */

static void the_bool_printer(pfstest_reporter_t *reporter,
                             pfstest_value_t *value)
{
    bool b = *(const bool *)pfstest_value_data(value);

    pfstest_reporter_print_nv_string(
        reporter, pfstest_nv_string("the bool "));
    pfstest_reporter_print_nv_string(
        reporter,
        b ? pfstest_nv_string("<true>") : pfstest_nv_string("<false>"));
}

pfstest_value_t *pfstest_the_bool(bool b)
{
    bool *data = pfstest_alloc(sizeof(b));
    *data = b;

    return pfstest_value_new(the_bool_printer, data, sizeof(b));
}

/* the_char */

static void the_char_printer(pfstest_reporter_t *reporter,
                             pfstest_value_t *value)
{
    char c = *(const char *)pfstest_value_data(value);

    pfstest_reporter_print_nv_string(
        reporter, pfstest_nv_string("the char '"));
    pfstest_reporter_print_escaped_char(reporter, c);
    pfstest_reporter_print_nv_string(reporter, pfstest_nv_string("'"));
}

pfstest_value_t *pfstest_the_char(char c)
{
    char *data = pfstest_alloc(sizeof(c));
    *data = c;

    return pfstest_value_new(the_char_printer, data, sizeof(c));
}

/* the_string */

static void the_string_printer(pfstest_reporter_t *reporter,
                               pfstest_value_t *value)
{
    const char *data = pfstest_value_data(value);
    const char *p = data;

    pfstest_reporter_print_nv_string(
        reporter, pfstest_nv_string("the string \""));
    
    while (*p) {
        pfstest_reporter_print_escaped_char(reporter, *p++);
    }

    pfstest_reporter_print_nv_string(reporter, pfstest_nv_string("\""));
}

pfstest_value_t *pfstest_the_string(const char *s)
{
    char *data = pfstest_alloc(strlen(s) + 1);
    strcpy(data, s);

    return pfstest_value_new(the_string_printer, data, strlen(s) + 1);
}

/* the_pointer */

static void the_pointer_printer(pfstest_reporter_t *reporter,
                                pfstest_value_t *value)
{
    const void *data = pfstest_value_data(value);

    pfstest_reporter_print_nv_string(
        reporter, pfstest_nv_string("the pointer <"));

    if (data == NULL) {
        pfstest_reporter_print_nv_string(
            reporter, pfstest_nv_string("NULL"));
    } else {
        pfstest_reporter_print_nv_string(reporter, pfstest_nv_string("0x"));
        pfstest_reporter_print_uint(
            reporter, (uintmax_t)(uintptr_t)data, 16, 0);
    }

    pfstest_reporter_print_nv_string(reporter, pfstest_nv_string(">"));

}

pfstest_value_t *pfstest_the_pointer(const void *p)
{
    return pfstest_value_new(the_pointer_printer, p, 0);
}

/* the_memory */

static void the_memory_printer(pfstest_reporter_t *reporter,
                               pfstest_value_t *value)
{
    const unsigned char *data = pfstest_value_data(value);
    size_t size = pfstest_value_size(value);
    size_t i;

    pfstest_reporter_print_nv_string(
        reporter, pfstest_nv_string("the memory {"));

    for (i = 0; i < size; i++) {

        pfstest_reporter_print_nv_string(reporter, pfstest_nv_string("0x"));
        pfstest_reporter_print_uint(reporter, data[i], 16, 2);

        if (i < size - 1) {
            pfstest_reporter_print_nv_string(
                reporter, pfstest_nv_string(", "));
        }
    }
    pfstest_reporter_print_nv_string(reporter, pfstest_nv_string("}"));
}

pfstest_value_t *pfstest_the_memory(const void *m, size_t size)
{
    void *data = pfstest_alloc(size);

    memcpy(data, m, size);

    return pfstest_value_new(the_memory_printer, data, size);
}

/* the_int_array */

static void the_int_array_printer(pfstest_reporter_t *reporter,
                                  pfstest_value_t *value)
{
    const int *data = (const int *)pfstest_value_data(value);
    size_t length = pfstest_value_size(value) / sizeof(data[0]);
    size_t i;

    pfstest_reporter_print_nv_string(
        reporter, pfstest_nv_string("{ "));

    for (i = 0; i < length; i++) {
        pfstest_value_print(reporter, pfstest_the_int(data[i]));
        if (i < length - 1) {
            pfstest_reporter_print_nv_string(
                reporter, pfstest_nv_string(", "));
        }
    }

    pfstest_reporter_print_nv_string(
        reporter, pfstest_nv_string(" }"));
}

pfstest_value_t *pfstest_the_int_array(const int *a, size_t length)
{
    size_t size = length * sizeof(*a);
    void *data = pfstest_alloc(size);
    memcpy(data, a, size);
    return pfstest_value_new(the_int_array_printer, data, size);
}
