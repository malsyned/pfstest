#ifndef PFSTEST_PLATFORM_AVR8_H
#define PFSTEST_PLATFORM_AVR8_H

#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#if defined(PFSTEST_AVR8_NAMED_ADDR_SPACES) && defined(__FLASH)

# define pfstest_nv __flash
# define pfstest_nv_ptr pfstest_nv
/* pfstest_nv_string adapted from avr-libc PSTR macro */
# define pfstest_nv_string(s) (__extension__({static const pfstest_nv char __c[] = (s); &__c[0];}))

# define pfstest_memcpy_nv(dest, src, n) _pfstest_memcpy_nv_avr(dest, src, n)
# define pfstest_strcmp_nv(s1, s2) _pfstest_strcmp_nv_avr(s1, s2)
# define pfstest_strcmp_nvnv(s1, s2) _pfstest_strcmp_nvnv_avr(s1, s2)
# define pfstest_strcat_nv(dest, src) _pfstest_strcat_nv_avr(dest, src)

void *_pfstest_memcpy_nv_avr(void *dest, const pfstest_nv_ptr void *src,
                             size_t n);
int _pfstest_strcmp_nv_avr(const char *s1, const pfstest_nv_ptr char *s2);
int _pfstest_strcmp_nvnv_avr(const pfstest_nv_ptr char *s1,
                             const pfstest_nv_ptr char *s2);
char *_pfstest_strcat_nv_avr(char *dest, const pfstest_nv_ptr char *src);

#else /* !defined(__FLASH) */

# include <avr/pgmspace.h>

# define pfstest_nv PROGMEM
# define pfstest_nv_ptr
# define pfstest_nv_string(string) PSTR(string)

/* This seems to use less ROM than calls to memcpy_P(ram, nv, size) */
#define pfstest_memcpy_nv(ram, nv, size)        \
    __extension__ ({                            \
        uint8_t *__p = (uint8_t *)ram;          \
        const pfstest_nv_ptr uint8_t *__nv =    \
            (const pfstest_nv_ptr uint8_t *)nv; \
        size_t __i;                             \
        for (__i = 0; __i < size; __i++) {      \
            *__p++ = pgm_read_byte(__nv++);     \
        }                                       \
        ram;                                    \
    })

# define pfstest_strcmp_nv(ram, nv) strcmp_P(ram, nv)
# define pfstest_strcat_nv(ram, nv) strcat_P(ram, nv)
# define pfstest_strcmp_nvnv(s1, s2) _pfstest_strcmp_nvnv_avr(s1, s2)

int _pfstest_strcmp_nvnv_avr(const char *s1, const char *s2);

#endif /* !defined(__FLASH) */

#include "pfstest-platform-gcc.h"

#endif /* !PFSTEST_PLATFORM_AVR8_H */
