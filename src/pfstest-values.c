#include "pfstest-values.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pfstest-platform.h"
#include "pfstest-alloc.h"

/* as_hex for unsigned values */

struct unsigned_aux
{
    unsigned int base;
    const pfstest_pg_ptr char *prefix;
};

static struct unsigned_aux *default_unsigned_aux(void)
{
    struct unsigned_aux *aux = pfstest_alloc(sizeof(*aux));
    aux->base = 10;
    aux->prefix = pfstest_pg_str("");

    return aux;
}

static void print_unsigned(pfstest_reporter_t *reporter,
                           pfstest_uintmax_t n, struct unsigned_aux *aux)
{
    pfstest_reporter_print_pg_str(reporter, aux->prefix);
    pfstest_reporter_print_uint(reporter, (pfstest_uintmax_t)n,
                                aux->base, 0);
}

pfstest_value_t *pfstest_as_hex(pfstest_value_t *value)
{
    struct unsigned_aux *aux = pfstest_value_aux(value);
    aux->base = 16;
    aux->prefix = pfstest_pg_str("0x");
    return value;
}

/* the_short */

static void the_short_printer(pfstest_reporter_t *reporter,
                              pfstest_value_t *value)
{
    short n = *(const short *)pfstest_value_data(value);

    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("the short "));
    pfstest_reporter_print_int(reporter, (pfstest_intmax_t)n);
}

pfstest_value_t *pfstest_the_short(short n)
{
    short *data = pfstest_alloc(sizeof(n));
    *data = n;

    return pfstest_value_new(the_short_printer, data, sizeof(n));
}

/* the_ushort */

static void the_ushort_printer(pfstest_reporter_t *reporter,
                               pfstest_value_t *value)
{
    unsigned short n = *(const unsigned short *)pfstest_value_data(value);
    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("the ushort "));
    print_unsigned(reporter, n, pfstest_value_aux(value));
}

pfstest_value_t *pfstest_the_ushort(unsigned short n)
{
    unsigned short *data = pfstest_alloc(sizeof(n));
    *data = n;

    return pfstest_value_new_with_aux(the_ushort_printer, data, sizeof(n),
                                      default_unsigned_aux());
}

/* the_int */

static void the_int_printer(pfstest_reporter_t *reporter,
                            pfstest_value_t *value)
{
    int n = *(const int *)pfstest_value_data(value);

    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("the int "));
    pfstest_reporter_print_int(reporter, (pfstest_intmax_t)n);
}

pfstest_value_t *pfstest_the_int(int n)
{
    int *data = pfstest_alloc(sizeof(n));
    *data = n;
    
    return pfstest_value_new(the_int_printer, data, sizeof(n));
}

/* the_uint */

static void the_uint_printer(pfstest_reporter_t *reporter,
                             pfstest_value_t *value)
{
    unsigned int n = *(const unsigned int *)pfstest_value_data(value);

    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("the uint "));
    print_unsigned(reporter, n, pfstest_value_aux(value));
}

pfstest_value_t *pfstest_the_uint(unsigned int n)
{
    unsigned int *data = pfstest_alloc(sizeof(n));
    *data = n;

    return pfstest_value_new_with_aux(the_uint_printer, data, sizeof(n),
                                      default_unsigned_aux());
}

/* the_long */

static void the_long_printer(pfstest_reporter_t *reporter,
                             pfstest_value_t *value)
{
    long n = *(const long *)pfstest_value_data(value);
    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("the long "));
    pfstest_reporter_print_int(reporter, (pfstest_intmax_t)n);
}

pfstest_value_t *pfstest_the_long(long n)
{
    long *data = pfstest_alloc(sizeof(n));
    *data = n;
    return pfstest_value_new(the_long_printer, data, sizeof(n));
}

/* the_ulong */

static void the_ulong_printer(pfstest_reporter_t *reporter,
                              pfstest_value_t *value)
{
    unsigned long n = *(const unsigned long *)pfstest_value_data(value);
    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("the ulong "));
    print_unsigned(reporter, n, pfstest_value_aux(value));
}

pfstest_value_t *pfstest_the_ulong(unsigned long n)
{
    unsigned long *data = pfstest_alloc(sizeof(n));
    *data = n;
    return pfstest_value_new_with_aux(the_ulong_printer, data, sizeof(n),
                                      default_unsigned_aux());
}

/* the_enum */

struct enum_aux
{
    const pfstest_pg_ptr char *const pfstest_pg_ptr *name_map;
};

static void the_enum_printer(pfstest_reporter_t *reporter,
                             pfstest_value_t *value)
{
    int e = *(const int *)pfstest_value_data(value);
    struct enum_aux *aux = pfstest_value_aux(value);
    const pfstest_pg_ptr char *const pfstest_pg_ptr *name_map = aux->name_map;
    const pfstest_pg_ptr char *const pfstest_pg_ptr *name_p;
    const pfstest_pg_ptr char *name;
    int count = 0;
    pfstest_bool in_range = pfstest_false;

    for (count = 0, name_p = name_map;
         pfstest_memcpy_pg(&name, name_p, sizeof(name)), name != NULL;
         count++, name_p++)
    {
        if (count == e) {
            in_range = pfstest_true;
            break;
        }
    }

    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("the enum "));
    if (in_range) {
        pfstest_reporter_print_pg_str(reporter, name);
    } else {
        pfstest_reporter_print_int(reporter, e);
    }

    if (!in_range) {
        pfstest_reporter_print_pg_str(reporter,
                                      pfstest_pg_str(" [out of range]"));
    }
}

pfstest_value_t *pfstest_the_enum(
    int e, const pfstest_pg_ptr char *const pfstest_pg_ptr *name_map)
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
    pfstest_bool b = *(const pfstest_bool *)pfstest_value_data(value);

    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("the bool "));
    if (b) {
        pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("<true>"));
    } else {
        pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("<false>"));
    }
}

pfstest_value_t *pfstest_the_bool(pfstest_bool b)
{
    pfstest_bool *data = pfstest_alloc(sizeof(b));
    *data = b ? pfstest_true : pfstest_false;

    return pfstest_value_new(the_bool_printer, data, sizeof(b));
}

/* the_char */

static void the_char_printer(pfstest_reporter_t *reporter,
                             pfstest_value_t *value)
{
    char c = *(const char *)pfstest_value_data(value);

    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("the char '"));
    pfstest_reporter_print_escaped_char(reporter, c);
    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("'"));
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

    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("the string \""));
    
    while (*p) {
        pfstest_reporter_print_escaped_char(reporter, *p++);
    }

    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("\""));
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

    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("the pointer <"));

    if (data == NULL) {
        pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("NULL"));
    } else {
        pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("0x"));
        pfstest_reporter_print_uint(
            reporter, (pfstest_uintmax_t)(pfstest_uintptr_t)data, 16, 0);
    }

    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str(">"));

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

    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("the memory {"));

    for (i = 0; i < size; i++) {

        pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("0x"));
        pfstest_reporter_print_uint(reporter, data[i], 16, 2);

        if (i < size - 1)
            pfstest_reporter_print_pg_str(reporter, pfstest_pg_str(", "));
    }
    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("}"));
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

    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("{ "));

    for (i = 0; i < length; i++) {
        pfstest_value_print(reporter, pfstest_the_int(data[i]));
        if (i < length - 1)
            pfstest_reporter_print_pg_str(reporter, pfstest_pg_str(", "));
    }

    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str(" }"));
}

pfstest_value_t *pfstest_the_int_array(const int *a, size_t length)
{
    size_t size = length * sizeof(*a);
    void *data = pfstest_alloc(size);
    memcpy(data, a, size);
    return pfstest_value_new(the_int_array_printer, data, size);
}

#ifdef PFSTEST_HAS_STDINT
static void the_u8_printer(pfstest_reporter_t *reporter,
                           pfstest_value_t *value)
{
    uint8_t n = *(const uint8_t *)pfstest_value_data(value);

    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("the uint8_t "));
    print_unsigned(reporter, n, pfstest_value_aux(value));
}

pfstest_value_t *pfstest_the_u8(uint8_t n)
{
    uint8_t *data = pfstest_alloc(sizeof(n));
    *data = n;

    return pfstest_value_new_with_aux(the_u8_printer, data, sizeof(*data),
                                      default_unsigned_aux());
}

static void the_u16_printer(pfstest_reporter_t *reporter,
                            pfstest_value_t *value)
{
    uint16_t n = *(const uint16_t *)pfstest_value_data(value);

    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("the uint16_t "));
    print_unsigned(reporter, n, pfstest_value_aux(value));
}

pfstest_value_t *pfstest_the_u16(uint16_t n)
{
    uint16_t *data = pfstest_alloc(sizeof(n));
    *data = n;

    return pfstest_value_new_with_aux(the_u16_printer, data, sizeof(*data),
                                      default_unsigned_aux());
}
#endif  /* defined(PFSTEST_HAS_STDINT) */
