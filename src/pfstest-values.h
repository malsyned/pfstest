#ifndef PFSTEST_VALUES_H
#define PFSTEST_VALUES_H

/** @file
 *
 * Box various primitive values for use with assert_that(), is(),
 * arg_that(), etc.
 *
 */

#include <stddef.h>

#include "pfstest-platform.h"
#include "pfstest-value.h"

/** Box a short */
pfstest_value_t *pfstest_the_short(short n);
/** Box an unsigned short */
pfstest_value_t *pfstest_the_ushort(unsigned short n);
/** Box an int */
pfstest_value_t *pfstest_the_int(int n);
/** Box an unsigned int */
pfstest_value_t *pfstest_the_uint(unsigned int n);
/** Box a long */
pfstest_value_t *pfstest_the_long(long n);
/** Box an unsigned long */
pfstest_value_t *pfstest_the_ulong(unsigned long n);
/** Box a size_t */
pfstest_value_t *pfstest_the_size(size_t size);
/** Box an enum
 *
 * @param e: The value to box, as an int
 * @param name_map: An ordered list of names for the enum's values
 *
 * @warning @p name_map has to have as many members as the highest
 * printable value of the enum in question, and so this boxer is not
 * ideal for sparsely populated enums such as those used as flag sets.
 */
pfstest_value_t *pfstest_the_enum(
    int e, const pfstest_pg_ptr char *const pfstest_pg_ptr *name_map);
/** Box a bool */
pfstest_value_t *pfstest_the_bool(pfstest_bool b);
/** Box a char */
pfstest_value_t *pfstest_the_char(char c);
/** Box a char*
 *
 * This char* will be printed as a null-terminated string in failure
 * messages. */
pfstest_value_t *pfstest_the_string(const char *s);
/** Box a pointer */
pfstest_value_t *pfstest_the_pointer(const void *p);
/** Box an arbitrary hunk of memory
 *
 * The memory will be printed as hex octets in failure messages. */
pfstest_value_t *pfstest_the_memory(const void *m, size_t size);

/** Box an int[] */
pfstest_value_t *pfstest_the_int_array(const int *a, size_t length);

/** Wrap unsigned primitive integers to cause them to be printed in
 * hexadecimal in failure messages.
 *
 * For example, @code the_uint(19) @endcode will be printed as @c "the
 * uint 19", but @code as_hex(the_uint(19)) @endcode will be printed
 * as @c "the uint 0x13".
 *
 * @warning Only use this wrapper with unsigned values built into
 * PFSTest. Using it with other values will not work and will probably
 * cause memory corrpution or a segfault.
 */
pfstest_value_t *pfstest_as_hex(pfstest_value_t *value);

#ifndef PFSTEST_NOALIAS_the_short
/** @nonamespace_alias{PFSTEST_NOALIAS_the_short} */
# define the_short pfstest_the_short
#endif
#ifndef PFSTEST_NOALIAS_the_ushort
/** @nonamespace_alias{PFSTEST_NOALIAS_the_ushort} */
# define the_ushort pfstest_the_ushort
#endif
#ifndef PFSTEST_NOALIAS_the_int
/** @nonamespace_alias{PFSTEST_NOALIAS_the_int} */
# define the_int pfstest_the_int
#endif
#ifndef PFSTEST_NOALIAS_the_uint
/** @nonamespace_alias{PFSTEST_NOALIAS_the_uint} */
# define the_uint pfstest_the_uint
#endif
#ifndef PFSTEST_NOALIAS_the_long
/** @nonamespace_alias{PFSTEST_NOALIAS_the_long} */
# define the_long pfstest_the_long
#endif
#ifndef PFSTEST_NOALIAS_the_ulong
/** @nonamespace_alias{PFSTEST_NOALIAS_the_ulong} */
# define the_ulong pfstest_the_ulong
#endif
#ifndef PFSTEST_NOALIAS_the_size
/** @nonamespace_alias{PFSTEST_NOALIAS_the_size} */
# define the_size pfstest_the_size
#endif
#ifndef PFSTEST_NOALIAS_the_enum
/** @nonamespace_alias{PFSTEST_NOALIAS_the_enum} */
# define the_enum pfstest_the_enum
#endif
#ifndef PFSTEST_NOALIAS_the_char
/** @nonamespace_alias{PFSTEST_NOALIAS_the_char} */
# define the_char pfstest_the_char
#endif
#ifndef PFSTEST_NOALIAS_the_bool
/** @nonamespace_alias{PFSTEST_NOALIAS_the_bool} */
# define the_bool pfstest_the_bool
#endif
#ifndef PFSTEST_NOALIAS_the_string
/** @nonamespace_alias{PFSTEST_NOALIAS_the_string} */
# define the_string pfstest_the_string
#endif
#ifndef PFSTEST_NOALIAS_the_pointer
/** @nonamespace_alias{PFSTEST_NOALIAS_the_pointer} */
# define the_pointer pfstest_the_pointer
#endif
#ifndef PFSTEST_NOALIAS_the_memory
/** @nonamespace_alias{PFSTEST_NOALIAS_the_memory} */
# define the_memory pfstest_the_memory
#endif
#ifndef PFSTEST_NOALIAS_the_int_array
/** @nonamespace_alias{PFSTEST_NOALIAS_the_int_array} */
# define the_int_array pfstest_the_int_array
#endif

#ifndef PFSTEST_NOALIAS_as_hex
/** @nonamespace_alias{PFSTEST_NOALIAS_as_hex} */
# define as_hex pfstest_as_hex
#endif

#if (PFSTEST_HAS_STDINT) || defined(__DOXYGEN__)
/** Box a uint8_t */
pfstest_value_t *pfstest_the_u8(uint8_t n);
/** Box a uint16_t */
pfstest_value_t *pfstest_the_u16(uint16_t n);
/** Box a uint32_t */
pfstest_value_t *pfstest_the_u32(uint32_t n);
/** Box a uint64_t */
pfstest_value_t *pfstest_the_u64(uint64_t n);

#ifndef PFSTEST_NOALIAS_the_u8
/** @nonamespace_alias{PFSTEST_NOALIAS_the_u8} */
# define the_u8 pfstest_the_u8
#endif
#ifndef PFSTEST_NOALIAS_the_u16
/** @nonamespace_alias{PFSTEST_NOALIAS_the_u16} */
# define the_u16 pfstest_the_u16
#endif
#ifndef PFSTEST_NOALIAS_the_u32
/** @nonamespace_alias{PFSTEST_NOALIAS_the_u32} */
# define the_u32 pfstest_the_u32
#endif
#ifndef PFSTEST_NOALIAS_the_u64
/** @nonamespace_alias{PFSTEST_NOALIAS_the_u64} */
# define the_u64 pfstest_the_u64
#endif
#endif  /* defined(PFSTEST_HAS_STDINT) */

#endif /* !PFSTEST_VALUES_H */
