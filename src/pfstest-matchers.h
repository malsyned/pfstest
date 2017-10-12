#ifndef PFSTEST_MATCHERS_H
#define PFSTEST_MATCHERS_H

#include <stddef.h>

#include "pfstest-matcher.h"

pfstest_matcher_t *pfstest_is_the_short(short s);
pfstest_matcher_t *pfstest_is_the_ushort(unsigned short s);
pfstest_matcher_t *pfstest_is_the_int(int i);
pfstest_matcher_t *pfstest_is_the_uint(unsigned int u);
pfstest_matcher_t *pfstest_is_the_enum(int e,
                                       const pfstest_nv_ptr char **name_map);
pfstest_matcher_t *pfstest_is_the_char(char c);
pfstest_matcher_t *pfstest_is_the_bool(bool b);
pfstest_matcher_t *pfstest_is_the_string(const char *s);
pfstest_matcher_t *pfstest_is_the_pointer(const void *p);
pfstest_matcher_t *pfstest_is_the_memory(const void *m, size_t size);
pfstest_matcher_t *pfstest_matches_the_nv_string(
    const pfstest_nv_ptr char *s);

pfstest_matcher_t *pfstest_int_members_match(pfstest_matcher_t *first, ...);

pfstest_matcher_t *_pfstest_is_anything(void);
#define pfstest_is_anything (_pfstest_is_anything())

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
#ifndef PFSTEST_NOALIAS_is_the_enum
# define is_the_enum pfstest_is_the_enum
#endif
#ifndef PFSTEST_NOALIAS_is_the_bool
# define is_the_bool pfstest_is_the_bool
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
#ifndef PFSTEST_NOALIAS_matches_the_nv_string
# define matches_the_nv_string pfstest_matches_the_nv_string
#endif
#ifndef PFSTEST_NOALIAS_int_members_match
# define int_members_match pfstest_int_members_match
#endif
#ifndef PFSTEST_NOALIAS_is_anything
# define is_anything pfstest_is_anything
#endif

#endif /* !PFSTEST_MATCHERS_H */
