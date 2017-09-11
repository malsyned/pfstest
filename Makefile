include Makefile.src.in

CC = cc

CFLAGS := -g -Og -Wall -Werror -MD -MP -pedantic -std=c89
LDFLAGS :=
CPPFLAGS :=
LDLIBS :=

.PHONY: all
all: tests register-tests.c

.PHONY: test
test: tests
	@echo ./tests $(ARGS); echo; \
	./tests $(ARGS); \
	retval=$$?; \
	if [ $$retval -eq 0 ]; then color=42; else color=41; fi; \
	echo "\033[$${color}m\n\033[00m"; \
	return $$retval

ALLOC_SRC := pfstest-alloc-malloc.c

SRC := $(COMMON_SRC) $(ALLOC_SRC) gcc-main.c

%.o: %.c $(MAKEFILE_LIST)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

%.i: %.c
	$(CC) $(CPPFLAGS) -E -o $@ $<

tests: $(SRC:%.c=%.o)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

register-tests.c: tests register-tests.c.header register-tests.c.footer
	cat register-tests.c.header > $@
	./$< -r >> $@
	cat register-tests.c.footer >> $@

.PHONY: clean
clean:
	rm -f *.o *.d *.i tests register-tests.c

-include $(wildcard *.d)
