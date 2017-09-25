#include "pfstest-platform-avr8.h"

#if defined(PFSTEST_AVR8_NAMED_ADDR_SPACES) && defined(__FLASH)

void *pfstest_memcpy_nv(void *dest, const pfstest_nv_ptr void *src, size_t n)
{
    char *d = dest;
    const pfstest_nv char *s = src;
    size_t i;

    for (i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

int pfstest_strcmp_nv(const char *s1, const pfstest_nv_ptr char *s2)
{
    for (; *s1 == *s2; s1++, s2++) {
        if (*s1 == '\0')
            return 0;
    }
    return *s1 - *s2;
}

int pfstest_strcmp_nvnv(const pfstest_nv_ptr char *s1,
                        const pfstest_nv_ptr char *s2)
{
    for (; *s1 == *s2; s1++, s2++) {
        if (*s1 == '\0')
            return 0;
    }
    return *s1 - *s2;
}

char *pfstest_strcat_nv(char *dest, const pfstest_nv_ptr char *src)
{
    char *d = dest;
    const pfstest_nv char *s = src;

    while (*d != '\0')
        d++;
    while ((*d++ = *s++) != '\0')
        ;

    return dest;
}

const pfstest_nv_ptr char *pfstest_strchr_nv(const pfstest_nv_ptr char *s,
                                             int c)
{
    while (*s != (char)c) {
        if (!*s++)
            return NULL;
    }
    return s;
}

#else /* !defined(__FLASH) */

int pfstest_strcmp_nvnv(const char *s1, const char *s2)
{
    char c1, c2;

    while (c1 = pgm_read_byte(s1), c2 = pgm_read_byte(s2), c1 && c1 == c2)
        s1++,s2++;

    return (unsigned char)c1 - (unsigned char)c2;
}

#endif /* !defined(__FLASH) */
