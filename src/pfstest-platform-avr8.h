#ifndef PFSTEST_PLATFORM_AVR8_H
#define PFSTEST_PLATFORM_AVR8_H

#if defined(PFSTEST_AVR8_NAMED_ADDR_SPACES) && defined(__FLASH)

# define pfstest_pg __flash
# define pfstest_pg_ptr pfstest_pg
/* pfstest_pg_str adapted from avr-libc PSTR macro */
# define pfstest_pg_str(s) (__extension__({static const pfstest_pg char __c[] = (s); &__c[0];}))

# define pfstest_memcpy_pg(dest, src, n) _pfstest_memcpy_pg_avr(dest, src, n)
# define pfstest_strcmp_pg(s1, s2) _pfstest_strcmp_pg_avr(s1, s2)
# define pfstest_strcmp_pgpg(s1, s2) _pfstest_strcmp_pgpg_avr(s1, s2)
# define pfstest_strcat_pg(dest, src) _pfstest_strcat_pg_avr(dest, src)

void *_pfstest_memcpy_pg_avr(void *dest, const pfstest_pg_ptr void *src,
                             size_t n);
int _pfstest_strcmp_pg_avr(const char *s1, const pfstest_pg_ptr char *s2);
int _pfstest_strcmp_pgpg_avr(const pfstest_pg_ptr char *s1,
                             const pfstest_pg_ptr char *s2);
char *_pfstest_strcat_pg_avr(char *dest, const pfstest_pg_ptr char *src);

#else /* !defined(__FLASH) */

# include <avr/pgmspace.h>

# define pfstest_pg PROGMEM
# define pfstest_pg_ptr
# define pfstest_pg_str(string) PSTR(string)

/* This seems to use less ROM than calls to memcpy_P(ram, pg, size) */
#define pfstest_memcpy_pg(ram, pg, size)        \
    __extension__ ({                            \
        uint8_t *__p = (uint8_t *)ram;          \
        const pfstest_pg_ptr uint8_t *__pg =    \
            (const pfstest_pg_ptr uint8_t *)pg; \
        size_t __i;                             \
        for (__i = 0; __i < size; __i++) {      \
            *__p++ = pgm_read_byte(__pg++);     \
        }                                       \
        ram;                                    \
    })

# define pfstest_strcmp_pg(ram, pg) strcmp_P(ram, pg)
# define pfstest_strcat_pg(ram, pg) strcat_P(ram, pg)
# define pfstest_strcmp_pgpg(s1, s2) _pfstest_strcmp_pgpg_avr(s1, s2)

int _pfstest_strcmp_pgpg_avr(const char *s1, const char *s2);

#endif /* !defined(__FLASH) */

#include "pfstest-platform-gcc.h"

#endif /* !PFSTEST_PLATFORM_AVR8_H */
