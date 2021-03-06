/* SPDX-License-Identifier: LGPL-2.1-or-later */

#ifndef PFSTEST_PLATFORM_DETECT_H
#define PFSTEST_PLATFORM_DETECT_H

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


#if defined(PFSTEST_PLATFORM) && !defined(PFSTEST_PLATFORM_HEADER)

# if PFSTEST_PLATFORM == PFSTEST_PLATFORM_AVR8
#  define PFSTEST_PLATFORM_HEADER "pfstest-platform-avr8.h"
# elif PFSTEST_PLATFORM == PFSTEST_PLATFORM_MCC18
#  define PFSTEST_PLATFORM_HEADER "pfstest-platform-mcc18.h"
# elif PFSTEST_PLATFORM == PFSTEST_PLATFORM_GCC
#  define PFSTEST_PLATFORM_HEADER "pfstest-platform-gcc.h"
# elif PFSTEST_PLATFORM == PFSTEST_PLATFORM_VCXX
#  define PFSTEST_PLATFORM_HEADER "pfstest-platform-vcxx.h"
# elif PFSTEST_PLATFORM != PFSTEST_PLATFORM_C89
#  error Platform not detected. Define PFSTEST_PLATFORM or PFSTEST_PLATFORM_HEADER
# endif

#endif  /* !defined(PFSTEST_PLATFORM_HEADER) */


#if defined(PFSTEST_PLATFORM_HEADER)
# include PFSTEST_PLATFORM_HEADER
#endif

#endif /* !defined(PFSTEST_PLATFORM_DETECT_H) */
