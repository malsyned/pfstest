# SPDX-License-Identifier: LGPL-2.1-or-later

GCC_AUTOMOCK_CPPFLAGS = -D"__attribute__(x)=" \
                        -D"__asm__(x)=" \
                        -D"__extension__="

GCC_AUTOMOCK_FLAGS = -t __builtin_va_list \
                    -f __inline \
                    -f __inline__ \
                    -q __const \
                    -q __restrict

AVR_GCC_AUTOMOCK_FLAGS = $(GCC_AUTOMOCK_FLAGS) -q __flash
