# Do not include this file directly. Instead, Add it to TARGET_PLUGINS
# to cause it to be included at the right point in test-targets.inc.mk

this-makefile := $(lastword $(MAKEFILE_LIST))

PYTHON ?= python
AUTOMOCK_SRC ?= $(dir $(this-makefile))../automock/automock.py
AUTOMOCK ?= $(PYTHON) $(AUTOMOCK_SRC)

mock-suffix = .mock

# $(call target-mocks,target)
target-mock-reqs = $(call target-param,$1,MOCKS)

# $(call target-mock-templates,target)
target-mock-templates =                                                 \
    $(addsuffix $(mock-suffix),                                         \
                $(addprefix $(call target-buildprefix,$1),              \
                            $(basename $(call target-mock-reqs,$1))))

# $(call target-mock-src,target)
target-mock-src = $(addsuffix .c,$(call target-mock-templates,$1))

# $(call target-mock-h,target)
target-mock-h = $(addsuffix .h,$(call target-mock-templates,$1))

# $(call targets-mock-h,target...)
targets-mock-h = $(foreach target,$1,$(call target-mock-h,$(target)))

# $(call target-mock-files,target)
target-mock-files = $(call target-mock-src,$1) $(call target-mock-h,$1)

# $(call targets-mock-files,target...)
targets-mock-files = $(foreach target,$1,$(call target-mock-files,$(target)))

# $(call class-automock-cppflags,class)
class-automock-cppflags = $(call class-param,$1,AUTOMOCK_CPPFLAGS)

# $(call class-automock-flags,class)
class-automock-flags = $(call class-param,$1,AUTOMOCK_FLAGS)

target-src +=  $(call target-mock-src,$1)

define class-template +=

    $$(call class-buildprefix,$1)%$$(mock-suffix).c \
    $$(call class-buildprefix,$1)%$$(mock-suffix).h \
    : %.h $$(AUTOMOCK_SRC) $$(makefile-list)
	@mkdir -p $$(dir $$@)
	$$(call class-cc,$1) $$(CFLAGS) $$(call class-cflags,$1)        \
	    $$(call class-includes,$1) $$(CPPFLAGS)                     \
	    $$(call class-cppflags,$1) $$(AUTOMOCK_CPPFLAGS)            \
	    $$(call class-automock-cppflags,$1) -E -o - $$<             \
	    | $$(AUTOMOCK) $$(AUTOMOCK_FLAGS)                           \
	      $$(call class-automock-flags,$1) $$< $$(basename $$@)

endef

clean-targets: clean-mock-files

.PHONY: clean-mock-files
clean-mock-files:
	rm -f $(call targets-mock-files,$(TARGETS))

# Without this, GNU Make deletes mock C files after regenerating .d
# files, even though they're still needed
.SECONDARY: $(call targets-mock-files,$(TARGETS))
$(call target-d,$(TARGETS)): | $(call targets-mock-h,$(TARGETS))
