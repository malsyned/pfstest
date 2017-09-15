#ifndef PFSTEST_PLATFORM_AVR8_H
#define PFSTEST_PLATFORM_AVR8_H

# include <stdbool.h>
# include <stdint.h>
# include <assert.h>
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
# define pfstest_strchr_nv(nv, c) strchr_P(nv, c)
# define PFSTEST_NORETURN __attribute__((__noreturn__))
# define pfstest_constructor(name)                          \
    __attribute__((__constructor__)) static void name(void)

int pfstest_strcmp_nvnv(const char *s1, const char *s2);

#endif /* !PFSTEST_PLATFORM_AVR8_H */
