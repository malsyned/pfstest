#ifndef BETTER_PLATFORM_H
#define BETTER_PLATFORM_H

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
# define PFSTEST_C99 1
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
#  define pfstest_uintptr_t unsigned int
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

# define PFS_ALIGNMENT                                          \
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
# define PFSTEST_NORETURN
#endif

#endif /* !defined(BETTER_PLATFORM_H) */
