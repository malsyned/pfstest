#ifndef PFSTEST_MATCHERS_H
#define PFSTEST_MATCHERS_H

#include <stddef.h>

#include "pfstest-matcher.h"

pfstest_matcher_t *pfstest_is_the_short(short s);
pfstest_matcher_t *pfstest_is_the_ushort(unsigned short s);
pfstest_matcher_t *pfstest_is_the_int(int i);
pfstest_matcher_t *pfstest_is_the_uint(unsigned int u);
pfstest_matcher_t *pfstest_is_the_char(char c);
pfstest_matcher_t *pfstest_is_the_string(char *s);
pfstest_matcher_t *pfstest_is_the_pointer(void *p);
pfstest_matcher_t *pfstest_is_the_memory(void *m, size_t size);

#ifndef PFSTEST_NOALIAS_is_the_short
# define is_the_short pfstest_is_the_short
#endif
#ifndef PFSTEST_NOALIAS_is_the_ushort
# define is_the_ushort pfstest_is_the_ushort
#endif
#ifndef PFSTEST_NOALIAS_is_the_int
# define is_the_int pfstest_is_the_int
#endif
#ifndef PFSTEST_NOALIAS_is_the_uint
# define is_the_uint pfstest_is_the_uint
#endif
#ifndef PFSTEST_NOALIAS_is_the_char
# define is_the_char pfstest_is_the_char
#endif
#ifndef PFSTEST_NOALIAS_is_the_string
# define is_the_string pfstest_is_the_string
#endif
#ifndef PFSTEST_NOALIAS_is_the_pointer
# define is_the_pointer pfstest_is_the_pointer
#endif
#ifndef PFSTEST_NOALIAS_is_the_memory
# define is_the_memory pfstest_is_the_memory
#endif

#endif /* !PFSTEST_MATCHERS_H */
