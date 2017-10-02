#ifndef PFSTEST_STDINT_VALUES_H
#define PFSTEST_STDINT_VALUES_H

#include <stdint.h>
#include "pfstest-value.h"

pfstest_value_t *pfstest_the_u8(uint8_t u8);
pfstest_value_t *pfstest_the_u16(uint16_t u16);

#ifndef PFSTEST_NOALIAS_the_u8
# define the_u8 pfstest_the_u8
#endif
#ifndef PFSTEST_NOALIAS_the_u16
# define the_u16 pfstest_the_u16
#endif

#endif /* !defined(PFSTEST_STDINT_VALUES_H) */
