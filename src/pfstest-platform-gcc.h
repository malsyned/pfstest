#ifndef PFSTEST_PLATFORM_GCC_H
#define PFSTEST_PLATFORM_GCC_H

# define PFS_ALIGNMENT __BIGGEST_ALIGNMENT__
# define PFSTEST_NORETURN __attribute__((__noreturn__))
# define pfstest_constructor(name)                          \
    __attribute__((__constructor__)) static void name(void)

/* Necessary to build stdint self-tests even with -std=c89 */
# include <stdint.h>
#define PFSTEST_HAS_STDINT 1

#endif /* !PFSTEST_PLATFORM_GCC_H */
