#ifndef PFSTEST_MATCHERS_H
#define PFSTEST_MATCHERS_H

#include <stdint.h>

#include "pfstest-matcher.h"

pfstest_matcher_t *is_the_int(intmax_t i);
pfstest_matcher_t *is_the_char(char c);

#endif /* !PFSTEST_MATCHERS_H */
