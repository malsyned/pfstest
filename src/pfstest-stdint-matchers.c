#include "pfstest-stdint-matchers.h"

#include "pfstest-stdint-values.h"
#include "pfstest-equality.h"

pfstest_matcher_t *pfstest_is_the_u8(uint8_t u8)
{
    return pfstest_matcher_new(pfstest_equality_printer,
                               pfstest_equality_test,
                               the_u8(u8));
}

pfstest_matcher_t *pfstest_is_the_u16(uint16_t u16)
{
    return pfstest_matcher_new(pfstest_equality_printer,
                               pfstest_equality_test,
                               the_u16(u16));
}
