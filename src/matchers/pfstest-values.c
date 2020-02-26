#include "pfstest-values.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pfstest-alloc.h"
#include "pfstest-core.h"

/* as_hex for unsigned values */

struct unsigned_aux
{
    unsigned int base;
    pfstest_bool known_width;
    int zpad;
    const pfstest_pg_ptr char *prefix;
};

static struct unsigned_aux *default_unsigned_aux(void)
{
    struct unsigned_aux *aux = pfstest_alloc(sizeof(*aux));
    aux->base = 10;
    aux->known_width = pfstest_false;
    aux->zpad = 0;
    aux->prefix = pfstest_pg_str("");

    return aux;
}

static void print_unsigned(pfstest_reporter_t *reporter,
                           pfstest_uintmax_t n, struct unsigned_aux *aux)
{
    pfstest_reporter_print_pg_str(reporter, aux->prefix);
    pfstest_reporter_print_uint(reporter, (pfstest_uintmax_t)n,
                                aux->base, aux->zpad);
}

#define HEX_CHARS_PER_BYTE 2

pfstest_value_t *pfstest_as_hex(pfstest_value_t *value)
{
    struct unsigned_aux *aux = pfstest_value_aux(value);
    aux->base = 16;
    aux->prefix = pfstest_pg_str("0x");
    if (aux->known_width)
        aux->zpad = (int)pfstest_value_size(value) * HEX_CHARS_PER_BYTE;
    return value;
}

/* integer templates */

#define signed_printer_define(fname, type, str)                         \
    static void fname(pfstest_value_t *value,                           \
                      pfstest_reporter_t *reporter)                     \
    {                                                                   \
        type n = *(const type *)pfstest_value_data(value);              \
                                                                        \
        pfstest_reporter_print_pg_str(reporter, pfstest_pg_str(str " ")); \
        pfstest_reporter_print_int(reporter, (pfstest_intmax_t)n);      \
    } _pfstest_expect_semicolon

#define unsigned_printer_define(fname, type, str)               \
    static void fname(pfstest_value_t *value,                   \
                      pfstest_reporter_t *reporter)             \
    {                                                           \
        type n = *(const type *)pfstest_value_data(value);      \
        pfstest_reporter_print_pg_str(reporter,                 \
                                      pfstest_pg_str(str " ")); \
        print_unsigned(reporter, n, pfstest_value_aux(value));  \
    } _pfstest_expect_semicolon

#define primitive_value_define(name, type, printer, aux)            \
    pfstest_value_t *name(type n)                                   \
    {                                                               \
        type *data = pfstest_alloc(sizeof(n));                      \
        *data = n;                                                  \
                                                                    \
        return pfstest_value_new(printer, data, sizeof(n), aux);    \
    } _pfstest_expect_semicolon

#define printer_name(name) _pfstest_econcat(name, _printer)

#define signed_value_define(name, type, str)                        \
    signed_printer_define(printer_name(name), type, str);           \
    primitive_value_define(name, type, printer_name(name), NULL)

#define unsigned_value_define(name, type, str)              \
    unsigned_printer_define(printer_name(name), type, str); \
    primitive_value_define(name, type, printer_name(name),  \
                           default_unsigned_aux())

/* integers */

signed_value_define(pfstest_the_short, short, "the short");
signed_value_define(pfstest_the_int, int, "the int");
signed_value_define(pfstest_the_long, long, "the long");

unsigned_value_define(pfstest_the_ushort, unsigned short, "the ushort");
unsigned_value_define(pfstest_the_uint, unsigned int, "the uint");
unsigned_value_define(pfstest_the_ulong, unsigned long, "the ulong");

unsigned_value_define(pfstest_the_size, size_t, "the size_t");

/* the_enum */

struct enum_aux
{
    const pfstest_pg_ptr char *const pfstest_pg_ptr *name_map;
};

static void the_enum_printer(pfstest_value_t *value,
                             pfstest_reporter_t *reporter)
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

    return pfstest_value_new(the_enum_printer, data, sizeof(e), aux);
}

/* the_bool */

static void the_bool_printer(pfstest_value_t *value,
                             pfstest_reporter_t *reporter)
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

    return pfstest_value_new(the_bool_printer, data, sizeof(b), NULL);
}

/* the_char */

static void the_char_printer(pfstest_value_t *value,
                             pfstest_reporter_t *reporter)
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

    return pfstest_value_new(the_char_printer, data, sizeof(c), NULL);
}

/* the_string */

static void the_string_printer(pfstest_value_t *value,
                               pfstest_reporter_t *reporter)
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

    return pfstest_value_new(the_string_printer, data, strlen(s) + 1, NULL);
}

/* the_pointer */

static void the_pointer_printer(pfstest_value_t *value,
                                pfstest_reporter_t *reporter)
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
    return pfstest_value_new(the_pointer_printer, p, 0, NULL);
}

/* the_memory */

static void the_memory_printer(pfstest_value_t *value,
                               pfstest_reporter_t *reporter)
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

    return pfstest_value_new(the_memory_printer, data, size, NULL);
}

/* the_int_array */

static void the_int_array_printer(pfstest_value_t *value,
                                  pfstest_reporter_t *reporter)
{
    const int *data = (const int *)pfstest_value_data(value);
    size_t length = pfstest_value_size(value) / sizeof(data[0]);
    size_t i;

    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("{ "));

    for (i = 0; i < length; i++) {
        pfstest_value_print(pfstest_the_int(data[i]), reporter);
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
    return pfstest_value_new(the_int_array_printer, data, size, NULL);
}

#ifdef PFSTEST_HAS_STDINT

/* stdint templates */

static struct unsigned_aux *known_width_unsigned_aux(void)
{
    struct unsigned_aux *aux = default_unsigned_aux();
    aux->known_width = pfstest_true;
    return aux;
}

#define unsigned_stdint_value_define(name, type, str)       \
    unsigned_printer_define(printer_name(name), type, str); \
    primitive_value_define(name, type, printer_name(name),  \
                           known_width_unsigned_aux())

/* stdint values */

unsigned_stdint_value_define(pfstest_the_u8, uint8_t, "the uint8_t");
unsigned_stdint_value_define(pfstest_the_u16, uint16_t, "the uint16_t");
unsigned_stdint_value_define(pfstest_the_u32, uint32_t, "the uint32_t");
unsigned_stdint_value_define(pfstest_the_u64, uint64_t, "the uint64_t");

#endif  /* defined(PFSTEST_HAS_STDINT) */
