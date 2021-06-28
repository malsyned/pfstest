# SPDX-License-Identifier: LGPL-2.1-or-later

# Do not include this file directly. Instead, Add it to
# MULTITARGET_PLUGINS to cause it to be included at the right point in
# multitarget.inc.mk

this-makefile := $(lastword $(MAKEFILE_LIST))

PYTHON ?= python
AUTOMOCK_SRC ?= $(dir $(this-makefile))../automock/automock.py
AUTOMOCK ?= $(PYTHON) $(AUTOMOCK_SRC)

mock-suffix = .mock

# $(call target-mock-reqs,target)
target-mock-reqs = $(call target-param,$1,MOCKS)

# $(call target-mock-req-basenames,target)
target-mock-req-basenames = $(basename $(call target-mock-reqs,$1))

# $(call target-mock-templates,target)
target-mock-templates =                                         \
    $(addsuffix $(mock-suffix),                                 \
                $(call target-buildpaths,$1,                    \
                       $(call target-mock-req-basenames,$1)))

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

# $(call class-mock-recipe-template,class,source-h)
define class-mock-recipe-template

    $$(call class-buildprefix,$1)%$$(mock-suffix).c \
    $$(call class-buildprefix,$1)%$$(mock-suffix).h \
    : $2 $$(AUTOMOCK_SRC) $$(makefile-list)
	@mkdir -p $$(dir $$@)
	$$(call class-cc,$1) $$(CFLAGS) $$(call class-cflags,$1)        \
	    $$(call class-cppflags,$1) $$(CPPFLAGS)                     \
	    $$(AUTOMOCK_CPPFLAGS) $$(call class-includes,$1)            \
	    $$(call class-automock-cppflags,$1) -E -o - $$<             \
	    | $$(AUTOMOCK) $$(AUTOMOCK_FLAGS)                           \
	      $$(call class-automock-flags,$1) $$< $$(basename $$@)

endef

define class-template +=

    $(call class-mock-recipe-template,$1,$$(SRCPREFIX)%.h)
    $(call class-mock-recipe-template,$1,%.h)

endef

clean-files += $(call targets-mock-files,$(TARGETS))

# Without this, GNU Make deletes mock C files after regenerating .d
# files, even though they're still needed
.SECONDARY: $(call targets-mock-files,$(TARGETS))
$(call target-d,$(TARGETS)): | $(call targets-mock-h,$(TARGETS))
