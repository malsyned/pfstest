#ifndef PFSTEST_PLATFORM_H
#define PFSTEST_PLATFORM_H

#if defined(__GNUC__) && defined(__AVR__)

# include <avr/pgmspace.h>

# define pfstest_nv_string_decl(name) const char name[] PROGMEM
# define pfstest_nv_string(string) PSTR(string)
# define PFSTEST_PRINV "S"
# define pfstest_strcmp_nv(ram, nv) strcmp_P(ram, nv)
# define pfstest_printf_nv printf_P
#else  /* !defined(__GNUC__) || !defined(__AVR__) */

#define PFSTEST_NV_UNSUPPORTED

# define pfstest_nv_string_decl(name) const char name[]
# define pfstest_nv_string(string) string
# define PFSTEST_PRINV "s"
# define pfstest_strcmp_nv(ram, nv) strcmp(ram, nv)
# define pfstest_printf_nv printf

#endif

#endif /* !PFSTEST_PLATFORM_H */
