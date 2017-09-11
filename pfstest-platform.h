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

pfstest_strcat_nv(ram, nv):

    A function or macro which is a version of strcat which
    concatenates a string which resides in the non-volatile memory
    space onto the end of a string which resides in RAM.

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

A platform port must define or include headers that define:
====

    Ideally, your platform has enough C99 support to provide
    stdbool.h, stdint.h, assert.h, and stdio.h. If that is the case,
    then a platform port should simply include those. Otherwise, it
    must define the following macros, typedefs, and functions itself:

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

#if defined(__GNUC__) && defined(__AVR__)

#include "pfstest-platform-avr8.h"

#elif defined(__18CXX)

#include "pfstest-platform-mcc18.h"

#elif defined(__GNUC__) || defined(__clang__)

#include "pfstest-platform-gcc.h"

#elif defined(_MSC_VER)

#include "pfstest-platform-vcxx.h"

#else  /* Unrecognized platform, fall back to ANSI C89 */

#include "pfstest-platform-c89.h"

#endif  /* Unrecognized platform */

#endif /* !PFSTEST_PLATFORM_H */
