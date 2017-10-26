#ifndef PFSTEST_PLATFORM_MCC18_H
#define PFSTEST_PLATFORM_MCC18_H

#include <string.h>

/* Missing from stdlib.h */
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define _exit(n) do { while (1) ; } while (0)
#define exit _exit

#include <stdio.h>
#define pfstest_c_assert(test)                                          \
    if (!(test)) {                                                      \
        fputs(pfstest_pg_string(__FILE__ ":Assertion failed: " #test),  \
              stdout);                                                  \
        _exit(1);                                                       \
    }

#define pfstest_pg far rom
#define pfstest_pg_string(string) ((const far rom char *)string)
#define pfstest_memcpy_pg(ram, pg, size) memcpypgm2ram(ram, pg, size)
#define pfstest_strcmp_pg(ram, pg) strcmppgm2ram(ram, pg)
#define pfstest_strcat_pg(dest, src) strcatpgm2ram(dest, src)
#define pfstest_strcmp_pg(s1, s2) _pfstest_strcmp_pgpg_mcc18(s1, s2)

int _pfstest_strcmp_pgpg_mcc18(const far rom char *s1,
                               const far rom char *s2);

#endif /* !PFSTEST_PLATFORM_MCC18_H */
