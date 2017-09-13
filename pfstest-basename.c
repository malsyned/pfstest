#include "pfstest-basename.h"

/* It would make more sense to use strrchr for this, rather than repeated
* calls to strchr, but mcc18 doesn't have strrchrpgm. */
static const pfstest_nv_ptr char *basename_helper(
    const pfstest_nv_ptr char *file, int pathsep)
{
    const pfstest_nv_ptr char *basename = file;
    const pfstest_nv_ptr char *found = file;

    while (found != NULL) {
        found = pfstest_strchr_nv(basename, pathsep);
        if (found != NULL) {
            basename = found + 1;
        }
    }

    return basename;
}

const pfstest_nv_ptr char *pfstest_basename(const pfstest_nv_ptr char *file)
{
    const pfstest_nv_ptr char *deunixed = basename_helper(file, '/');
    const pfstest_nv_ptr char *dedossed = basename_helper(deunixed, '\\');
    return dedossed;
}
