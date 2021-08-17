/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include "pfstest.h"
#include "pfstest-cli.h"

#if defined(_WIN32)
# include <windows.h>
#endif

static void enable_ansi_color(void)
{
#if defined(_WIN32)
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (handle != INVALID_HANDLE_VALUE) {
        DWORD mode = 0;
        if (GetConsoleMode(handle, &mode)) {
            mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(handle, mode);
        }
    }
#endif
}

int main(int argc, char *argv[])
{
    enable_ansi_color();
    return pfstest_main(argc, argv);
}
