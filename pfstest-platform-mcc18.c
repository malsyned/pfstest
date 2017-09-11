#include "pfstest-platform-mcc18.h"

int pfstest_strcmp_nvnv(const far rom char *s1, const far rom char *s2)
{
    while (*s1 && *s1 == *s2)
        s1++, s2++;

    return (unsigned char)*s1 - (unsigned char)*s2;
}
