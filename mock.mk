# -*- Makefile -*-

MOCK_CPPFLAGS := -D"__attribute__(x)=" \
                 -D"__asm__(x)=" \
                 -D"__extension__="

AUTOMOCK_ARGS := -t __builtin_va_list \
                 -f __inline \
                 -f __inline__ \
                 -q __const \
                 -q __restrict

AUTOMOCK ?= automock/automock.py

mock-targets = $(MOCKPREFIX)%-mock.c $(MOCKPREFIX)%-mock.h

# Without this, GNU Make deletes mock C files after regenerating .d
# files, even though they're still needed
.PRECIOUS: $(mock-targets)

mock-inc = $(addprefix -I,$(MOCKPREFIX))

$(mock-targets): %.h $(MAKEFILE_LIST)
	mkdir -p $(dir $@)
	$(CC) $(mock-inc) $(MOCK_CPPFLAGS) $(CPPFLAGS) -E -o - $< \
	  | $(AUTOMOCK) $(AUTOMOCK_ARGS) $< $(basename $@)
