#ifndef PFSTEST_STDINT_MATCHERS_H
#define PFSTEST_STDINT_MATCHERS_H

#include <stdint.h>
#include "pfstest-matcher.h"

pfstest_matcher_t *pfstest_is_the_u8(uint8_t u8);
pfstest_matcher_t *pfstest_is_the_u16(uint16_t u16);

#ifndef PFSTEST_NOALIAS_is_the_u8
# define is_the_u8 pfstest_is_the_u8
#endif
#ifndef PFSTEST_NOALIAS_is_the_u16
# define is_the_u16 pfstest_is_the_u16
#endif

#endif /* !defined(PFSTEST_STDINT_MATCHERS_H) */
