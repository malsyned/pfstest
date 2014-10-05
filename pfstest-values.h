#ifndef PFSTEST_VALUES_H
#define PFSTEST_VALUES_H

#include <stddef.h>
#include <stdint.h>

#include "pfstest-value.h"

pfstest_value_t *the_int(intmax_t i);
pfstest_value_t *the_char(char c);
pfstest_value_t *the_string(char *s);
pfstest_value_t *the_pointer(void *p);
pfstest_value_t *the_memory(void *m, size_t size);

#endif /* !PFSTEST_VALUES_H */
