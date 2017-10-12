#ifndef PFSTEST_VALUES_H
#define PFSTEST_VALUES_H

#include <stddef.h>

#include "pfstest-value.h"

pfstest_value_t *pfstest_the_short(short n);
pfstest_value_t *pfstest_the_ushort(unsigned short n);
pfstest_value_t *pfstest_the_int(int n);
pfstest_value_t *pfstest_the_uint(unsigned int n);
pfstest_value_t *pfstest_the_long(long n);
pfstest_value_t *pfstest_the_ulong(unsigned long n);
pfstest_value_t *pfstest_the_enum(
    int e, const pfstest_nv_ptr char *const pfstest_nv_ptr *name_map);
pfstest_value_t *pfstest_the_bool(bool b);
pfstest_value_t *pfstest_the_char(char c);
pfstest_value_t *pfstest_the_string(const char *s);
pfstest_value_t *pfstest_the_pointer(const void *p);
pfstest_value_t *pfstest_the_memory(const void *m, size_t size);

pfstest_value_t *pfstest_the_int_array(const int *a, size_t length);

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
#ifndef PFSTEST_NOALIAS_the_long
# define the_long pfstest_the_long
#endif
#ifndef PFSTEST_NOALIAS_the_ulong
# define the_ulong pfstest_the_ulong
#endif
#ifndef PFSTEST_NOALIAS_the_enum
# define the_enum pfstest_the_enum
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
#ifndef PFSTEST_NOALIAS_the_int_array
# define the_int_array pfstest_the_int_array
#endif

#endif /* !PFSTEST_VALUES_H */
