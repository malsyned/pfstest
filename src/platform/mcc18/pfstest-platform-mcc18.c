#include "pfstest-platform-mcc18.h"

int _pfstest_strcmp_pgpg_mcc18(const far rom char *s1, const far rom char *s2)
{
    while (*s1 && *s1 == *s2)
        s1++, s2++;

    return (unsigned char)*s1 - (unsigned char)*s2;
}
