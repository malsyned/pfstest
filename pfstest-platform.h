#ifndef PFSTEST_PLATFORM_H
#define PFSTEST_PLATFORM_H

#if defined(__GNUC__) && defined(__AVR__)

# include <stdbool.h>
# include <stdint.h>
# include <assert.h>
# include <avr/pgmspace.h>

# define pfstest_nv_string_decl(name) const char name[] PROGMEM
# define pfstest_nv_string(string) PSTR(string)
# define pfstest_nv_str_ptr(name) PGM_P name
# define pfstest_strcmp_nv(ram, nv) strcmp_P(ram, nv)
# define pfstest_printf_nv printf_P
# define pfstest_print_nv_string(string) fputs_P(string, stdout)
# define PFSTEST_NORETURN __attribute__((__noreturn__))
# define pfstest_constructor(name)              \
    __attribute__((__constructor__))            \
    static void name(void)

int pfstest_strcmp_nvnv(const char *s1, const char *s2);

#elif defined(__18CXX)

/* stdbool.h */
typedef unsigned char bool;
# define false 0
# define true 1

/* stdint.h */
typedef int intptr_t;

/* stdlib.h */
#define exit(n) do { while (1) ; } while (0)

/* assert.h */
#include <stdio.h>
#define assert(test)                                                    \
    if (!(test)) {                                                      \
        pfstest_print_nv_string(                                        \
            pfstest_nv_string(__FILE__ ":Assertion failed: " #test));   \
        exit(1);                                                        \
    }

# define pfstest_nv_string_decl(name) const far rom char name[]
# define pfstest_nv_str_ptr(name) const far rom char *name
# define pfstest_nv_string(string) ((const far rom char *)string)
# define pfstest_strcmp_nv(ram, nv) strcmppgm2ram(ram, nv)
# define pfstest_strcmp_nvnv strcmppgm
# define pfstest_printf_nv printf
# define pfstest_print_nv_string(string) fputs(string, stdout)
# define PFSTEST_NORETURN

# define fflush

#else  /* Unrecognized platform */

# include <stdbool.h>
# include <stdint.h>
# include <assert.h>

# define pfstest_nv_string_decl(name) const char name[]
# define pfstest_nv_string(string) string
# define pfstest_nv_str_ptr(name) const char *name
# define pfstest_strcmp_nv(ram, nv) strcmp(ram, nv)
# define pfstest_strcmp_nvnv strcmp
# define pfstest_printf_nv printf
# define pfstest_print_nv_string(string) fputs(string, stdout)
# define PFSTEST_NORETURN __attribute__((__noreturn__))
# define pfstest_constructor(name)              \
    __attribute__((__constructor__))            \
    static void name(void)

#endif

# define pfstest_print_int(i)                                   \
    pfstest_printf_nv(pfstest_nv_string("%d"), i)

#endif /* !PFSTEST_PLATFORM_H */
