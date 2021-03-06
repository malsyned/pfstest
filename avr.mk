# SPDX-License-Identifier: LGPL-2.1-or-later

include tests/selftestsrc.inc.mk
include util/automock-gcc.inc.mk

MCU = atmega128
F_CPU = 8000000

CC = avr-gcc
CFLAGS = -gdwarf-2 -Os $(WARN) -std=gnu99

WARN  = -Wall
WARN += -Wextra
WARN += -Werror
WARN += -Wwrite-strings
WARN += -Wsuggest-attribute=noreturn
WARN += -Wmissing-include-dirs
WARN += -Wswitch-default
WARN += -Wfloat-equal
WARN += -Wpointer-arith
WARN += -Wundef
WARN += -Wbad-function-cast
WARN += -Wcast-align
WARN += -Wjump-misses-init
WARN += -Wconversion
WARN += -Wlogical-op
WARN += -Wstrict-prototypes
WARN += -Wold-style-definition
WARN += -Wmissing-prototypes
WARN += -Wmissing-declarations
WARN += -Wvariadic-macros
WARN += -pedantic
WARN += -pedantic-errors
# avr-gcc uses this warning to warn about uninitialized program memory
# in addition to uninitialized automatics. The setup and teardown
# pointer system relies on uninitialized statics, so we have to
# disable this warning.
WARN += -Wno-uninitialized

MCUFLAGS = -mmcu=$(MCU)

CFLAGS += $(MCUFLAGS)
CFLAGS += -funsigned-char
CFLAGS += -funsigned-bitfields
CFLAGS += -fpack-struct
CFLAGS += -fshort-enums
CFLAGS += -ffunction-sections -fdata-sections

CFLAGS += -Wa,-adhlns=$(basename $@).lst

CPPFLAGS = -DF_CPU=$(F_CPU)UL
CPPFLAGS += -Iinclude
ifdef NAMED_ADDR_SPACES
CPPFLAGS += -DPFSTEST_AVR8_NAMED_ADDR_SPACES
endif
LDFLAGS = $(MCUFLAGS)
LDFLAGS += -Wl,-Map=$(basename $@).map,--cref
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -Wl,-u,vfprintf
LDLIBS = -lprintf_flt -lm

AUTOMOCK_CPPFLAGS = $(GCC_AUTOMOCK_CPPFLAGS)
AUTOMOCK_FLAGS = $(AVR_GCC_AUTOMOCK_FLAGS)

BIN_PATTERN = %.elf
BUILDPREFIX = avrbuild/

SRC = $(wildcard src/core/*.c) \
      $(wildcard src/matchers/*.c) \
      $(wildcard src/mock/*.c) \
      $(wildcard src/fp/*.c) \
      $(wildcard src/platform/avr8/*.c) \
      $(wildcard src/platform/avr8/tools/*.c) \
      src/alloc/pfstest-alloc-malloc.c \
      src/reporters/pfstest-reporter-xml.c \
      src/main/avr-main.c

self-tests-avr_SRC = $(SELFTEST_SRC) \
                     $(wildcard src/cli/*.c) \
                     src/reporters/pfstest-reporters-standard.c
self-tests-avr_MOCKS = $(SELFTEST_MOCKS)

TARGETS = self-tests-avr

MULTITARGET_PLUGINS = util/automock.inc.mk
include util/multitarget.inc.mk

clean-files += $(call targets-files,$(TARGETS),.lst) self-tests-avr.map

all: sizes

.PHONY: sizes
sizes: | targets
	@for elf in $(call targets-bin-names,$(TARGETS)); \
	do \
	    if test -f $${elf}; \
	    then \
	        echo; \
	        avr-size --mcu=$(MCU) --format=avr $${elf} 2>/dev/null; \
	    fi; \
	done

SIMULAVR=$$HOME/local/bin/simulavr
.PHONY: test
test: targets
	@set -e; \
	for elf in $(call targets-bin-names,$(TARGETS)); \
	do \
	    echo ; \
	    $(SIMULAVR) --device $(MCU) --cpufrequency $(F_CPU) -f $${elf} -W 0x20,- -T exit ; \
	done


# TODO:
# debugging and programming
# These kinds of things: hex eep fuse lock lss sym
# %.hex: %.elf
# 	avr-objcopy -O $(FORMAT) -R .eeprom -R .fuse -R .lock -R .signature $< $@
# %.eep: %.elf
# 	-avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" \
# 	--change-section-lma .eeprom=0 --no-change-warnings -O $(FORMAT) $< $@ || exit 0
# # Search ioxxx.h to decide what fuse hex files to build.
# io.h-dump := $(shell echo | $(CC) -mmcu=$(MCU) -include avr/io.h -dM -E -)
# ifneq (,$(findstring FUSE_MEMORY_SIZE 1,$(io.h-dump)))
#   FUSE_TARGETS := $(TARGET).fuse
# else ifneq (,$(findstring FUSE_MEMORY_SIZE 2,$(io.h-dump)))
#   FUSE_TARGETS := $(TARGET).lfuse $(TARGET).hfuse
# else ifneq (,$(findstring FUSE_MEMORY_SIZE 3,$(io.h-dump)))
#   FUSE_TARGETS := $(TARGET).lfuse $(TARGET).hfuse $(TARGET).efuse
# else
#   FUSE_TARGETS := $(patsubst FUSEBYTE%;,$(TARGET).fuse%,$(filter FUSEBYTE%;,$(io.h-dump)))
# endif
# %.fuse: %.elf
# 	-avr-objcopy -j .fuse --set-section-flags=.fuse="alloc,load" \
# 	--change-section-lma .fuse=0 --no-change-warnings -O $(FORMAT) $< $@ || exit 0
# %.lfuse: %.elf
# 	-avr-objcopy -j .fuse -i 3 -b 0 --set-section-flags=.fuse="alloc,load" \
# 	--change-section-lma .fuse=0 --no-change-warnings -O $(FORMAT) $< $@ || exit 0
# %.hfuse: %.elf
# 	-avr-objcopy -j .fuse -i 3 -b 1 --set-section-flags=.fuse="alloc,load" \
# 	--change-section-lma .fuse=0 --no-change-warnings -O $(FORMAT) $< $@ || exit 0
# %.efuse: %.elf
# 	-avr-objcopy -j .fuse -i 3 -b 2 --set-section-flags=.fuse="alloc,load" \
# 	--change-section-lma .fuse=0 --no-change-warnings -O $(FORMAT) $< $@ || exit 0
# %.fuse0: %.elf
# 	-avr-objcopy -j .fuse -i 256 -b 0 --set-section-flags=.fuse="alloc,load" \
# 	--change-section-lma .fuse=0 --no-change-warnings -O $(FORMAT) $< $@ || exit 0
# %.fuse1: %.elf
# 	-avr-objcopy -j .fuse -i 256 -b 1 --set-section-flags=.fuse="alloc,load" \
# 	--change-section-lma .fuse=0 --no-change-warnings -O $(FORMAT) $< $@ || exit 0
# %.fuse2: %.elf
# 	-avr-objcopy -j .fuse -i 256 -b 2 --set-section-flags=.fuse="alloc,load" \
# 	--change-section-lma .fuse=0 --no-change-warnings -O $(FORMAT) $< $@ || exit 0
# %.fuse3: %.elf
# 	-avr-objcopy -j .fuse -i 256 -b 3 --set-section-flags=.fuse="alloc,load" \
# 	--change-section-lma .fuse=0 --no-change-warnings -O $(FORMAT) $< $@ || exit 0
# %.fuse4: %.elf
# 	-avr-objcopy -j .fuse -i 256 -b 4 --set-section-flags=.fuse="alloc,load" \
# 	--change-section-lma .fuse=0 --no-change-warnings -O $(FORMAT) $< $@ || exit 0
# %.fuse5: %.elf
# 	-avr-objcopy -j .fuse -i 256 -b 5 --set-section-flags=.fuse="alloc,load" \
# 	--change-section-lma .fuse=0 --no-change-warnings -O $(FORMAT) $< $@ || exit 0
# %.fuse6: %.elf
# 	-avr-objcopy -j .fuse -i 256 -b 6 --set-section-flags=.fuse="alloc,load" \
# 	--change-section-lma .fuse=0 --no-change-warnings -O $(FORMAT) $< $@ || exit 0
# %.fuse7: %.elf
# 	-avr-objcopy -j .fuse -i 256 -b 7 --set-section-flags=.fuse="alloc,load" \
# 	--change-section-lma .fuse=0 --no-change-warnings -O $(FORMAT) $< $@ || exit 0
# %.lock: %.elf
# 	-avr-objcopy -j .lock --set-section-flags=.fuse="alloc,load" \
# 	--change-section-lma .lock=0 --no-change-warnings -O $(FORMAT) $< $@ || exit 0
# %.lss: %.elf
# 	avr-objdump -h -S -z $< > $@
# %.sym: %.elf
# 	avr-nm -n $< > $@
