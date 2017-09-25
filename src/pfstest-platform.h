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

A platform port header must define:
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

pfstest_strcat_nv(ram, nv):

    A function or macro which is a version of strcat which
    concatenates a string which resides in the non-volatile memory
    space onto the end of a string which resides in RAM.

pfstest_strchr_nv(nv, int):

    A function or macro which is a version of strchr which
    searches a string which resides in the non-volatile memory
    space for the specified character.

PFSTEST_NORETURN:

    Expands to a function decoration which tells the compiler that the
    function should not be expected to return. Define to nothing if
    your compiler does not support this.
    
A platform port header may additionally define:
====

pfstest_constructor(name):

    If your compiler provides an extension that allows registering
    hooks to run at start-up, this should expand to the code which
    creates such a hook. On ports where this macro is not defined,
    register_test, register_before, and register_after will need to be
    called to set up the test environment before invoking pfstest.

A platform port header must define or include headers that define:
====

    Ideally, your platform has enough C99 support to provide
    stdbool.h, stdint.h, assert.h, and stdio.h. If that is the case,
    then a platform port header should simply include those.
    Otherwise, it must define the following macros, typedefs, and
    functions itself:

bool
false
true

    Compatible with C99 stdbool.h

uintptr_t
intmax_t
uintmax_t
UINTMAX_MAX

    Compatible with C99 stdint.h

assert

    Compatible with C89 assert.h

fflush

    Compatible with C89 stdio.h

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

#define PFSTEST_PLATFORM_C89   1
#define PFSTEST_PLATFORM_GCC   2
#define PFSTEST_PLATFORM_VCXX  3
#define PFSTEST_PLATFORM_AVR8  4
#define PFSTEST_PLATFORM_MCC18 5


#if !defined(PFSTEST_PLATFORM) && !defined(PFSTEST_PLATFORM_NOAUTO)

# if defined(__GNUC__) && defined(__AVR__)
#  define PFSTEST_PLATFORM PFSTEST_PLATFORM_AVR8
# elif defined(__18CXX)
#  define PFSTEST_PLATFORM PFSTEST_PLATFORM_MCC18
# elif defined(__GNUC__) || defined(__clang__)
#  define PFSTEST_PLATFORM PFSTEST_PLATFORM_GCC
# elif defined(_MSC_VER)
#  define PFSTEST_PLATFORM PFSTEST_PLATFORM_VCXX
# else  /* Unrecognized platform, fall back to ANSI C89 */
#  define PFSTEST_PLATFORM PFSTEST_PLATFORM_C89
# endif

#endif /* !defined(PFSTEST_PLATFORM) && !defined(PFSTEST_PLATFORM_NOAUTO) */


#if !defined(PFSTEST_PLATFORM_HEADER)

# if PFSTEST_PLATFORM == PFSTEST_PLATFORM_AVR8
#  define PFSTEST_PLATFORM_HEADER "pfstest-platform-avr8.h"
# elif PFSTEST_PLATFORM == PFSTEST_PLATFORM_MCC18
#  define PFSTEST_PLATFORM_HEADER "pfstest-platform-mcc18.h"
# elif PFSTEST_PLATFORM == PFSTEST_PLATFORM_GCC
#  define PFSTEST_PLATFORM_HEADER "pfstest-platform-gcc.h"
# elif PFSTEST_PLATFORM == PFSTEST_PLATFORM_VCXX
#  define PFSTEST_PLATFORM_HEADER "pfstest-platform-vcxx.h"
# elif PFSTEST_PLATFORM == PFSTEST_PLATFORM_C89
#  define PFSTEST_PLATFORM_HEADER "pfstest-platform-c89.h"
# else
#  error Platform not detected. Define PFSTEST_PLATFORM or PFSTEST_PLATFORM_HEADER
# endif

#endif  /* !defined(PFSTEST_PLATFORM_HEADER) */


#include PFSTEST_PLATFORM_HEADER

#endif /* !PFSTEST_PLATFORM_H */
