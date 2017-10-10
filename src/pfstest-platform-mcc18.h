#ifndef PFSTEST_PLATFORM_MCC18_H
#define PFSTEST_PLATFORM_MCC18_H

#include <string.h>

/* stdbool.h */
typedef unsigned char bool;
# define false 0
# define true 1

/* stdint.h */
#include <limits.h>
typedef unsigned int uintptr_t;
typedef long intmax_t;
typedef unsigned long uintmax_t;
#define UINTMAX_MAX ULONG_MAX

/* stdlib.h */
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define _exit(n) do { while (1) ; } while (0)
#define exit _exit

/* assert.h */
#include <stdio.h>
#define assert(test)                                                    \
    if (!(test)) {                                                      \
        fputs(pfstest_nv_string(__FILE__ ":Assertion failed: " #test),  \
              stdout);                                                  \
        _exit(1);                                                       \
    }

# define pfstest_nv far rom
# define pfstest_nv_ptr pfstest_nv
# define pfstest_nv_string(string) ((const far rom char *)string)
# define pfstest_memcpy_nv(ram, nv, size) memcpypgm2ram(ram, nv, size)
# define pfstest_strcmp_nv(ram, nv) strcmppgm2ram(ram, nv)
# define pfstest_strcat_nv strcatpgm2ram
# define PFSTEST_NORETURN

int pfstest_strcmp_nvnv(const far rom char *s1, const far rom char *s2);

#endif /* !PFSTEST_PLATFORM_MCC18_H */
