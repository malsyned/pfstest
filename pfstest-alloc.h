#ifndef PFSTEST_ALLOC_H
#define PFSTEST_ALLOC_H

#include <stddef.h>

void *pfstest_alloc(size_t size);
void pfstest_free_all(void);

#ifndef PFSTEST_NOALIAS_test_alloc
# define test_alloc pfstest_alloc
#endif

#endif /* !PFSTEST_ALLOC_H */
