#ifndef PFSTEST_MATCHERS_H
#define PFSTEST_MATCHERS_H

#include <stddef.h>
#include <stdint.h>

#include "pfstest-matcher.h"

pfstest_matcher_t *is_the_int(intmax_t i);
pfstest_matcher_t *is_the_char(char c);
pfstest_matcher_t *is_the_string(char *s);
pfstest_matcher_t *is_the_pointer(void *p);
pfstest_matcher_t *is_the_memory(void *m, size_t size);

#endif /* !PFSTEST_MATCHERS_H */
