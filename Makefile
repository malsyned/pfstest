include Makefile.src.in

CFLAGS := -g -O0 -Wall -Werror -MD -MP -pedantic -std=c89

.PHONY: all
all: core-tests tests register-core-tests.c register-tests.c

.PHONY: test-core
test-core: core-tests
	@./core-tests -v | diff -u expected-output -
	@./core-tests -r | diff -u expected-output-register -
	@echo PASS

.PHONY: test
test: tests
	echo ; ./tests $(ARGS)

SRC := $(COMMON_SRC) main.c

CORE_SRC := $(CORE_COMMON_SRC) main.c

core-tests: $(CORE_SRC:%.c=%.o)
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

tests: $(SRC:%.c=%.o)
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

register-core-tests.c: core-tests register-tests.c.header register-tests.c.footer
	cat register-tests.c.header > $@
	./$< -r >> $@
	cat register-tests.c.footer >> $@

register-tests.c: tests register-tests.c.header register-tests.c.footer
	cat register-tests.c.header > $@
	./$< -r >> $@
	cat register-tests.c.footer >> $@

.PHONY: clean
clean:
	rm -f *.o *.d tests core-tests register-core-tests.c register-tests.c

-include $(wildcard *.d)
