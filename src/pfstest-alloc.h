#ifndef PFSTEST_ALLOC_H
#define PFSTEST_ALLOC_H

/** @file */

#include <stddef.h>

/** Allocate memory which will be freed at the end of the current test */
void *pfstest_alloc(size_t size);
void pfstest_alloc_free_frame(void);
void pfstest_alloc_frame_push(void);
void pfstest_alloc_frame_pop(void);

#ifndef PFSTEST_NOALIAS_test_alloc
/** @nonamespace_alias{PFSTEST_NOALIAS_test_alloc} */
# define test_alloc pfstest_alloc
#endif

#endif /* !PFSTEST_ALLOC_H */
