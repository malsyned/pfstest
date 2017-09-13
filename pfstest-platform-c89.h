#ifndef PFSTEST_PLATFORM_C89_H
#define PFSTEST_PLATFORM_C89_H

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
# define pfstest_strchr_nv strchr
# define PFSTEST_NORETURN

#endif /* !PFSTEST_PLATFORM_C89_H */
