include Makefile.src.in

CC = cc

OPT := g
WARN := -Wall -Wextra -Werror -Wwrite-strings \
        -Wsuggest-attribute=noreturn -Wmissing-include-dirs \
        -Wswitch-default -Wfloat-equal -Wpointer-arith -Wundef \
        -Wunused-macros -Wbad-function-cast -Wcast-qual -Wcast-align \
        -Wjump-misses-init -Wconversion -Wlogical-op -Wstrict-prototypes \
        -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations \
        -Wvariadic-macros -Wunsuffixed-float-constants \
        -pedantic -pedantic-errors
CFLAGS := -g -O$(OPT) $(WARN) -MD -MP -std=c89
LDFLAGS :=
CPPFLAGS :=
LDLIBS :=

.PHONY: all
all: tests register-tests.c

.PHONY: test
test: tests
	@echo ./tests $(ARGS); echo; \
	MALLOC_CHECK_=1 ./tests $(ARGS); \
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
