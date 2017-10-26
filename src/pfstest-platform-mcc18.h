#ifndef PFSTEST_PLATFORM_MCC18_H
#define PFSTEST_PLATFORM_MCC18_H

#include <string.h>

/* Missing from stdlib.h */
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define _exit(n) do { while (1) ; } while (0)
#define exit _exit

#include <stdio.h>
#define pfstest_c_assert(test)                                                    \
    if (!(test)) {                                                      \
        fputs(pfstest_nv_string(__FILE__ ":Assertion failed: " #test),  \
              stdout);                                                  \
        _exit(1);                                                       \
    }

#define pfstest_nv far rom
#define pfstest_nv_ptr pfstest_nv
#define pfstest_nv_string(string) ((const far rom char *)string)
#define pfstest_memcpy_nv(ram, nv, size) memcpypgm2ram(ram, nv, size)
#define pfstest_strcmp_nv(ram, nv) strcmppgm2ram(ram, nv)
#define pfstest_strcat_nv(dest, src) strcatpgm2ram(dest, src)
#define pfstest_strcmp_nv(s1, s2) _pfstest_strcmp_nvnv_mcc18(s1, s2)

int _pfstest_strcmp_nvnv_mcc18(const far rom char *s1,
                               const far rom char *s2);

#endif /* !PFSTEST_PLATFORM_MCC18_H */
