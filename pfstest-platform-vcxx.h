#ifndef PFSTEST_PLATFORM_VCXX_H
#define PFSTEST_PLATFORM_VCXX_H

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
# define pfstest_strcat_nv strcat
# define PFSTEST_NORETURN __declspec(noreturn)

# pragma section(".CRT$XCU",read)
# define pfstest_constructor(name)                                  \
    static void name(void);                                         \
    static __declspec(allocate(".CRT$XCU"))                         \
    void (* _pfstest_econcat(__pfstest_ctor__, name))(void) = name; \
    static void name(void)

#endif /* !PFSTEST_PLATFORM_VCXX_H */
