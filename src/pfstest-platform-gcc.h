#ifndef PFSTEST_PLATFORM_GCC_H
#define PFSTEST_PLATFORM_GCC_H

# define PFS_ALIGNMENT __BIGGEST_ALIGNMENT__
# define PFSTEST_NORETURN __attribute__((__noreturn__))
# define pfstest_constructor(name)                          \
    __attribute__((__constructor__)) static void name(void)

#endif /* !PFSTEST_PLATFORM_GCC_H */
