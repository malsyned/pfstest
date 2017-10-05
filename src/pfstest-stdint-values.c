#include "pfstest-stdint-values.h"

#include "pfstest-alloc.h"

static void the_u8_printer(pfstest_reporter_t *reporter,
                           pfstest_value_t *value)
{
    uint8_t u8 = *(const uint8_t *)pfstest_value_data(value);

    pfstest_reporter_print_nv_string(
        reporter, pfstest_nv_string("the uint8_t "));
    pfstest_reporter_print_uint(reporter, (uintmax_t)u8, 10, 0);
}

pfstest_value_t *pfstest_the_u8(uint8_t u8)
{
    uint8_t *data = pfstest_alloc(sizeof(u8));
    *data = u8;

    return pfstest_value_new(the_u8_printer, data, sizeof(*data));
}

static void the_u16_printer(pfstest_reporter_t *reporter,
                            pfstest_value_t *value)
{
    uint16_t u16 = *(const uint16_t *)pfstest_value_data(value);

    pfstest_reporter_print_nv_string(
        reporter, pfstest_nv_string("the uint16_t "));
    pfstest_reporter_print_uint(reporter, (uintmax_t)u16, 10, 0);
}

pfstest_value_t *pfstest_the_u16(uint16_t u16)
{
    uint16_t *data = pfstest_alloc(sizeof(u16));
    *data = u16;

    return pfstest_value_new(the_u16_printer, data, sizeof(*data));
}
