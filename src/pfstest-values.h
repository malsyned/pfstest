#ifndef PFSTEST_VALUES_H
#define PFSTEST_VALUES_H

#include <stddef.h>

#include "pfstest-value.h"

pfstest_value_t *pfstest_the_short(short s);
pfstest_value_t *pfstest_the_ushort(unsigned short u);
pfstest_value_t *pfstest_the_int(int i);
pfstest_value_t *pfstest_the_uint(unsigned int u);
pfstest_value_t *pfstest_the_bool(bool b);
pfstest_value_t *pfstest_the_char(char c);
pfstest_value_t *pfstest_the_string(char *s);
pfstest_value_t *pfstest_the_pointer(void *p);
pfstest_value_t *pfstest_the_memory(void *m, size_t size);

#ifndef PFSTEST_NOALIAS_the_short
# define the_short pfstest_the_short
#endif
#ifndef PFSTEST_NOALIAS_the_ushort
# define the_ushort pfstest_the_ushort
#endif
#ifndef PFSTEST_NOALIAS_the_int
# define the_int pfstest_the_int
#endif
#ifndef PFSTEST_NOALIAS_the_uint
# define the_uint pfstest_the_uint
#endif
#ifndef PFSTEST_NOALIAS_the_char
# define the_char pfstest_the_char
#endif
#ifndef PFSTEST_NOALIAS_the_bool
# define the_bool pfstest_the_bool
#endif
#ifndef PFSTEST_NOALIAS_the_string
# define the_string pfstest_the_string
#endif
#ifndef PFSTEST_NOALIAS_the_pointer
# define the_pointer pfstest_the_pointer
#endif
#ifndef PFSTEST_NOALIAS_the_memory
# define the_memory pfstest_the_memory
#endif

#endif /* !PFSTEST_VALUES_H */