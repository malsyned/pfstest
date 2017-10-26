#ifndef PFSTEST_PLATFORM_H
#define PFSTEST_PLATFORM_H

/*

PFSTest attempts to work out-of-the-box with any ANSI C compiler. In
order to do so, it makes usually-reliable guesses about platform
alignment requirements and the sizes of primitive types.

If you would like to override these guesses, a plaform port can
define:

PFSTEST_ALIGNMENT:

    The biggest alignment required by any type on your platform.

pfstest_uintptr_t:

    An unsigned integer type big enough to hold the value of any
    pointer on your platform. On C99 platforms, defaults to
    uintptr_t. On other platforms, defaults to size_t.

pfstest_intmax_t:

    The largest signed integer type on your platform. On C99
    platforms, defaults to intmax_t. On other platforms, defaults to
    long.

pfstest_uintmax_t:

    The largest unsigned integer type on your platform. On C99
    platforms, defaults to uintmax_t. On other platforms, defaults to
    unsigned pfstest_intmax_t.

PFSTEST_UINTMAX_MAX:

    The maximum value of the largest unsigned integer type on your
    platform. On C99 platforms, defaults to UINTMAX_MAX. On other
    platforms, defaults to ULONG_MAX.

====

PFSTest supports some optional features which are not standard to ANSI
C but which exist on some platforms and can be enabled for your
convenience if your platform supports them. A platform port header may
additionally define:

pfstest_constructor(name):

    If your compiler provides an extension that allows registering
    hooks to run at start-up, this should expand to the code which
    creates such a hook. On ports where this macro is not defined,
    register_test, register_before, and register_after will need to be
    called to set up the test environment before invoking pfstest.

PFSTEST_NORETURN:

    If your platform supports an annotation for functions to indicate
    that they should not return, define this macro to that annotation.

====

PFSTest supports Harvard architectures, and on those architectures it
attempts to use as little RAM as possible by storing strings and other
constant data in non-volatile memory. The various compilers for these
architectures use different mechanisms and syntax to support multiple
memory spaces, and much of the platform-specific configuration in this
module is designed to provide a common API for accessing the
non-volatile memory space.

In order to enable Harvard architecture support, a platform port
header must define:

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

pfstest_strcmp_nvnv(nv1, nv2):

    A function or macro which is a version of strcmp which compares
    two strings that reside in the non-volatile memory space.

pfstest_strcat_nv(ram, nv):

    A function or macro which is a version of strcat which
    concatenates a string which resides in the non-volatile memory
    space onto the end of a string which resides in RAM.

 */

/* Platforms with built-in support are guessed using compiler-specific
 * macro definitions. This auto-detection can be overridden in one of
 * two ways:
 *
 * 1) To force one of the built-in platforms, define PFSTEST_PLATFORM
 *    to one of the macros below.
 *
 * 2) To use a user-defined platform, create a platform port header
 *    that meets the above requirements and define
 *    PFSTEST_PLATFORM_HEADER to a string suitable for an #include
 *    directive.
 *
 *    Getting the quoting right when manually setting
 *    PFSTEST_PLATFORM_HEADER from the command line can be
 *    tricky. Here is the proper syntax for defining it in various
 *    environments: for double- and single-quoted CPPFLAGS under POSIX
 *    sh, and for defining a variable :
 *
 *     On a POSIX sh make command line, value in double-quotes:
 *     make CPPFLAGS="-DPFSTEST_PLATFORM_HEADER=\\\"pfstest-platform-user.h\\\""
 *
 *     On a POSIX sh make command line, value in single-quotes:
 *     make CPPFLAGS='-DPFSTEST_PLATFORM_HEADER=\"pfstest-platform-user.h\"'
 *
 *     In a Makefile:
 *     CPPFLAGS := -DPFSTEST_PLATFORM_HEADER=\"pfstest-platform-gcc.h\"
 */

#if defined(__STDC_VERSION__)
# if __STDC_VERSION__ >= 199901L
#  define PFSTEST_C99 1
# endif
# if __STDC_VERSION__ >= 201112L
#  define PFSTEST_C11 1
# endif
#endif

#include "pfstest-platform-detect.h"

/* Booleans */
#ifdef PFSTEST_C99
# include <stdbool.h>
# define pfstest_bool bool
# define pfstest_true true
# define pfstest_false false
#else
# define pfstest_bool unsigned char
# define pfstest_true 1
# define pfstest_false 0
#endif

/* Useful integer types */
#ifdef PFSTEST_C99
# include <stdint.h>
# define pfstest_uintptr_t uintptr_t
# define pfstest_intmax_t intmax_t
# define pfstest_uintmax_t uintmax_t
# define PFSTEST_UINTMAX_MAX UINTMAX_MAX
#else
# ifndef pfstest_uintptr_t
#  include <stddef.h>
#  define pfstest_uintptr_t size_t
# endif
# ifndef pfstest_intmax_t
#  define pfstest_intmax_t long
# endif
# ifndef pfstest_uintmax_t
#  define pfstest_uintmax_t unsigned pfstest_intmax_t
# endif
# ifndef PFSTEST_UINTMAX_MAX
#  include <limits.h>
#  define PFSTEST_UINTMAX_MAX ULONG_MAX
# endif
#endif

/* Alignment guessing */
#ifndef PFSTEST_ALIGNMENT

struct _pfstest_alignment_struct
{
    char c;
    union
    {
        char c;
        short s;
        long l;
#ifdef PFSTEST_C99
        long long ll;
#endif
        float f;
        double d;
        long double ld;
    } alignment_union;
};

# define PFSTEST_ALIGNMENT                                      \
    offsetof(struct _pfstest_alignment_struct, alignment_union)

#endif

/* Harvard architecture support */
#ifndef pfstest_nv
# define pfstest_nv
#endif
#ifndef pfstest_nv_ptr
# define pfstest_nv_ptr pfstest_nv
#endif
#ifndef pfstest_nv_string
# define pfstest_nv_string(string) string
#endif
#if !defined(pfstest_memcpy_nv) || !defined(pfstest_strcmp_nv)      \
    || !defined(pfstest_strcmp_nvnv) || !defined(pfstest_strcat_nv)
# include <string.h>
#endif
#ifndef pfstest_memcpy_nv
# define pfstest_memcpy_nv(ram, nv, size) memcpy(ram, nv, size)
#endif
#ifndef pfstest_strcmp_nv
# define pfstest_strcmp_nv(ram, nv) strcmp(ram, nv)
#endif
#ifndef pfstest_strcmp_nvnv
# define pfstest_strcmp_nvnv(nv1, nv2) strcmp(nv1, nv2)
#endif
#ifndef pfstest_strcat_nv
# define pfstest_strcat_nv(dest, src) strcat(dest, src)
#endif

/* mcc18 special snowflake */
#ifndef pfstest_c_assert
# include <assert.h>
# define pfstest_c_assert assert
#endif

/* Compiler hints */
#ifndef PFSTEST_NORETURN
# ifdef PFSTEST_C11
#  include <stdnoreturn.h>
#  define PFSTEST_NORETURN noreturn
# else
#  define PFSTEST_NORETURN
# endif
#endif

#endif /* !PFSTEST_PLATFORM_H */
