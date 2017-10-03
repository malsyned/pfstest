GCC_MOCK_CPPFLAGS = -D"__attribute__(x)=" \
                    -D"__asm__(x)=" \
                    -D"__extension__="

GCC_AUTOMOCK_ARGS = -t __builtin_va_list \
                    -f __inline \
                    -f __inline__ \
                    -q __const \
                    -q __restrict

AVR_GCC_AUTOMOCK_ARGS = $(GCC_AUTOMOCK_ARGS) -q __flash
