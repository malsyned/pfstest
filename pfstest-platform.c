#include "pfstest-platform.h"

#if defined(__GNUC__) && defined(__AVR__)

int pfstest_strcmp_nvnv(const char *s1, const char *s2) 
{
    char c1, c2;

    while (c1 = pgm_read_byte(s1), c2 = pgm_read_byte(s2), c1 && c1 == c2)
        s1++,s2++;

    return (unsigned char)c1 - (unsigned char)c2;
}

#endif  /* defined(__GNUC__) && defined(__AVR__) */

enum { work_around_iso_c_empty_translation_unit_prohibition };
