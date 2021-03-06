/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include "pfstest-platform-avr8.h"

#if defined(PFSTEST_AVR8_NAMED_ADDR_SPACES) && defined(__FLASH)

void *_pfstest_memcpy_pg_avr(void *dest, const pfstest_pg_ptr void *src,
                             size_t n)
{
    char *d = dest;
    const pfstest_pg char *s = src;
    size_t i;

    for (i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

int _pfstest_strcmp_pg_avr(const char *s1, const pfstest_pg_ptr char *s2)
{
    for (; *s1 == *s2; s1++, s2++) {
        if (*s1 == '\0')
            return 0;
    }
    return *s1 - *s2;
}

int _pfstest_strcmp_pgpg_avr(const pfstest_pg_ptr char *s1,
                             const pfstest_pg_ptr char *s2)
{
    for (; *s1 == *s2; s1++, s2++) {
        if (*s1 == '\0')
            return 0;
    }
    return *s1 - *s2;
}

char *_pfstest_strcat_pg_avr(char *dest, const pfstest_pg_ptr char *src)
{
    char *d = dest;
    const pfstest_pg char *s = src;

    while (*d != '\0')
        d++;
    while ((*d++ = *s++) != '\0')
        ;

    return dest;
}

#else /* !defined(__FLASH) */

int _pfstest_strcmp_pgpg_avr(const char *s1, const char *s2)
{
    char c1, c2;

    while (c1 = pgm_read_byte(s1), c2 = pgm_read_byte(s2), c1 && c1 == c2)
        s1++,s2++;

    return (unsigned char)c1 - (unsigned char)c2;
}

#endif /* !defined(__FLASH) */
