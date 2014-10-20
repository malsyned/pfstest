include Makefile.src.in

CFLAGS := -g -O0 -Wall -Werror -MD -MP -pedantic -std=c89

.PHONY: all
all: core-tests tests

.PHONY: test-core
test-core: core-tests
	@./core-tests -v | diff -u expected-output -
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

.PHONY: clean
clean:
	rm -f *.o *.d tests core-tests

-include $(wildcard *.d)
