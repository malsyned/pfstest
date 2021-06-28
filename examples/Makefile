SRCPREFIX = ../
BIN_PATTERN = build/%-runner
BUILDPREFIX = build/
OUTPUTPREFIX = output/
CPPFLAGS = -I$(SRCPREFIX)include

SRC = $(wildcard $(SRCPREFIX)src/core/*.c) \
      $(wildcard $(SRCPREFIX)src/reporters/*.c) \
      $(wildcard $(SRCPREFIX)src/matchers/*.c) \
      $(wildcard $(SRCPREFIX)src/mock/*.c) \
      $(wildcard $(SRCPREFIX)src/fp/*.c) \
      $(wildcard $(SRCPREFIX)src/cli/*.c) \
      $(SRCPREFIX)src/alloc/pfstest-alloc-malloc.c \
      $(SRCPREFIX)src/main/gcc-main.c

quickstart1_SRC = quickstart/1/quickstart.c
quickstart1v_SRC = $(quickstart1_SRC)
quickstart1v_ARGS = -v

quickstart2_SRC = quickstart/2/quickstart.c

quickstart3f_SRC = quickstart/3/quickstart.c quickstart/3f/square.c
quickstart3_SRC = quickstart/3/quickstart.c quickstart/3/square.c
quickstart3_ARGS = -v

quickstart4f_CPPFLAGS = -Iquickstart/4
quickstart4f_SRC = quickstart/4/quickstart.c \
                   quickstart/3/square.c quickstart/4f/cube.c

quickstart4_SRC = quickstart/4/quickstart.c \
                  quickstart/3/square.c quickstart/4/cube.c

TARGETS = quickstart1 quickstart1v quickstart2 quickstart3f quickstart3 \
          quickstart4f quickstart4

MULTITARGET_PLUGINS = $(SRCPREFIX)util/test-runners.inc.mk
include $(SRCPREFIX)util/multitarget.inc.mk

.DEFAULT_GOAL := example-output

target-args = $(call target-param,$1,ARGS)

.PHONY: example-output
example-output: $(call targets-bin-names,$(TARGETS))
	@mkdir -p output
	$(foreach target,$(TARGETS),                         \
	          ./$(call target-bin-name,$(target))        \
                  $(call target-args,$(target))              \
                  | sed 's/[a-zA-Z0-9_/-]*\///g'             \
                  > $(OUTPUTPREFIX)$(target).txt || true; )

clean-files += $(foreach target,$(TARGETS),$(OUTPUTPREFIX)$(target).txt)
all-buildprefixes += $(OUTPUTPREFIX)
