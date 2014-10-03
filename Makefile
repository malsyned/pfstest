include Makefile.src.in

CFLAGS := -g -O0 -Wall -Werror -MD -MP

.PHONY: all
all: sample-tests

.PHONY: run
run: sample-tests
	echo ; ./sample-tests

SRC :=  $(COMMON_SRC) main.c

sample-tests: $(SRC:%.c=%.o)

.PHONY: clean
clean:
	rm -f *.o *.d sample-tests

-include $(wildcard *.d)
