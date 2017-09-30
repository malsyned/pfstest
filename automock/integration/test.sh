#!/bin/sh

set -e

HEADERPATH=${1:-all-headers.h}
MOCKROOT=${2:-mock-all-headers}

gcc -E -D"__attribute__(x)=" -D"__asm__(x)=" -D"__extension__=" $HEADERPATH \
    | ../automock.py -q __restrict -f __inline -t __builtin_va_list \
		     $HEADERPATH $MOCKROOT
