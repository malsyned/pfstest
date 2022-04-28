# SPDX-License-Identifier: LGPL-2.1-or-later

GCC_AUTOMOCK_CPPFLAGS = -D"__attribute__(x)=" \
                        -D"__asm__(x)=" \
                        -D"__extension__=" \
                        -D"__volatile__(x)="

GCC_AUTOMOCK_FLAGS = -t __builtin_va_list \
                    -t _Float128 \
                    -t __float128 \
                    -f __inline \
                    -f __inline__ \
                    -q __const \
                    -q __restrict \
                    -q __restrict__

AVR_GCC_AUTOMOCK_FLAGS = $(GCC_AUTOMOCK_FLAGS) -q __flash
