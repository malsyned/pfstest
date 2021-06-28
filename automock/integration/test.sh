#!/bin/sh
# SPDX-License-Identifier: LGPL-2.1-or-later

set -e

HEADERPATH=${1:-all-headers.h}
MOCKROOT=${2:-mock-all-headers}

gcc -E -D"__attribute__(x)=" -D"__asm__(x)=" -D"__extension__=" $HEADERPATH \
    | ../automock.py -t __builtin_va_list -f __inline -f __inline__ \
		     -q __const -q __restrict \
                     -t _Float32 -t _Float64 -t _Float128 \
                     -t _Float32x -t _Float64x \
		     $HEADERPATH $MOCKROOT
