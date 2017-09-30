#!/bin/sh

set -e

HEADERPATH=${1:-all-headers.h}
MOCKROOT=${2:-mock-all-headers}

-t __builtin_va_list -f __inline -f __inline__ -q __const -q __restrict

gcc -E -D"__attribute__(x)=" -D"__asm__(x)=" -D"__extension__=" $HEADERPATH \
    | ../automock.py -t __builtin_va_list -f __inline -f __inline__ \
		     -q __const -q __restrict \
		     $HEADERPATH $MOCKROOT
