#ifndef PFSTEST_PLATFORM_H
#define PFSTEST_PLATFORM_H

/*

PFSTest supports Harvard architectures, and on those architectures it
attempts to use as little RAM as possible by storing strings and other
constant data in non-volatile memory. The various compilers for these
architectures use different mechanisms and syntax to support multiple
memory spaces, and much of the platform-specific configuration in this
module is designed to provide a common API for accessing the
non-volatile memory space.

A platform port must define:
====

pfstest_nv:

    Expands to a keyword or keywords used by the compiler to specify
    that a data object should reside in the non-volatile memory space.
    Note that this is only valid when creating data objects. It may
    not be correct to use this as a general storage-class specifier to
    describe the location of the target of a pointer (for example, on
    avr-gcc). See also: pfstest_nv_ptr

pfstest_nv_ptr:

    Expands to a keyword or keywords used by the compiler to specify
    that a pointer should point to the non-volatile memory space. Note
    that this is only valid when creating pointers. It may not be
    correct to use this as a storage-class specifier for data objects
    themselves (for example, on avr-gcc). See also: pfstest_nv_ptr

pfstest_nv_string(string):

    Expands to a string constant which resides in the non-volatile
    memory space.

pfstest_memcpy_nv(ram, nv, size):

    A function or macro which is a version of memcpy which copies data
    from the nonvolatile memory space into RAM.

pfstest_strcmp_nv(ram, nv):

    A function or macro which is a version of strcmp which compares a
    string in RAM to one in the non-volatile memory space.

pfstest_strcmp_nvnv:

    A function or macro which is a version of strcmp which compares
    two strings that reside in the non-volatile memory space.

pfstest_printf_nv:

    A function or macro which works like printf() but expects the
    format string to reside in the non-volatile memory space.

pfstest_print_nv_string:

    A function or macro which prints a string that resides in the
    non-volatile memory space.

PFSTEST_NORETURN:

    Expands to a function decoration which tells the compiler that the
    function should not be expected to return. Define to nothing if
    your compiler does not support this.
    
A platform port may additionally define:
====

pfstest_constructor(name):

    If your compiler provides an extension that allows registering
    hooks to run at start-up, this should expand to the code which
    creates such a hook. On ports where this macro is not defined,
    register_test, register_before, and register_after will need to be
    called to set up the test environment before invoking pfstest.

 */

#if defined(__GNUC__) && defined(__AVR__)

# include <stdbool.h>
# include <stdint.h>
# include <assert.h>
# include <avr/pgmspace.h>

# define pfstest_nv PROGMEM
# define pfstest_nv_ptr
# define pfstest_nv_string(string) PSTR(string)
# define pfstest_memcpy_nv(ram, nv, size) memcpy_P(ram, nv, size)
# define pfstest_strcmp_nv(ram, nv) strcmp_P(ram, nv)
# define pfstest_printf_nv printf_P
# define pfstest_print_nv_string(string) fputs_P(string, stdout)
# define PFSTEST_NORETURN __attribute__((__noreturn__))
# define pfstest_constructor(name)                          \
    __attribute__((__constructor__)) static void name(void)

int pfstest_strcmp_nvnv(const char *s1, const char *s2);

#elif defined(__18CXX)

#include <string.h>

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

# define pfstest_nv far rom
# define pfstest_nv_ptr pfstest_nv
# define pfstest_nv_string(string) ((const far rom char *)string)
# define pfstest_memcpy_nv(ram, nv, size) memcpypgm2ram(ram, nv, size)
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
# include <string.h>

# define pfstest_nv 
# define pfstest_nv_ptr
# define pfstest_nv_string(string) string
# define pfstest_memcpy_nv(ram, nv, size) memcpy(ram, nv, size)
# define pfstest_strcmp_nv(ram, nv) strcmp(ram, nv)
# define pfstest_strcmp_nvnv strcmp
# define pfstest_printf_nv printf
# define pfstest_print_nv_string(string) fputs(string, stdout)
# define PFSTEST_NORETURN __attribute__((__noreturn__))
# define pfstest_constructor(name)                          \
    __attribute__((__constructor__)) static void name(void)

#endif

# define pfstest_print_int(i)                                   \
    pfstest_printf_nv(pfstest_nv_string("%d"), i)

#endif /* !PFSTEST_PLATFORM_H */
