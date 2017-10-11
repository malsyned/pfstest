this-makefile := $(lastword $(MAKEFILE_LIST))

# $(call target-param-name,target,param)
target-param-name = $1_$2

# $(call target-param,target,param)
target-param = $($(call target-param-name,$1,$2))

# $(call target-class,target)
target-class = $(call target-param,$1,CLASS)

# $(call target-class-param-name,target,param)
target-class-param-name = $(call target-class,$1)_$2

# $(call target-class-param,target,param)
target-class-param = $($(call target-class-param-name,$1,$2))

# $(call target-buildprefix,target)
target-buildprefix = $(or $(call target-class-param,$1,BUILDPREFIX), \
                          $(BUILDPREFIX))

# $(call target-mocks,target)
target-mock-reqs = $(call target-param,$1,MOCKS)

mock-suffix = .mock

# $(call target-mock-templates,target)
target-mock-templates = \
    $(addsuffix $(mock-suffix), \
                $(addprefix $(call target-buildprefix,$1), \
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

# $(call target-cc,target)
target-cc = $(or $(call target-class-param,$1,CC),$(CC))

# $(call target-cflags,target)
target-cflags = $(call target-class-param,$1,CFLAGS)

# $(call target-cppflags,target)
target-cppflags = $(call target-class-param,$1,CPPFLAGS)

# $(call target-ldflags,target)
target-ldflags = $(call target-class-param,$1,LDFLAGS)

# $(call target-ldlibs,target)
target-ldlibs = $(call target-class-param,$1,LDLIBS)

# $(call target-automock-cppflags,target)
target-automock-cppflags = $(call target-class-param,$1,AUTOMOCK_CPPFLAGS)

# $(call target-automock-flags,target)
target-automock-flags = $(call target-class-param,$1,AUTOMOCK_FLAGS)

# $(call target-src,target)
target-src = $(call target-param,$1,SRC) $(call target-mock-src,$1) $(SRC)

# $(call target-output,target,file...,extension)
target-output = $(addprefix $(call target-buildprefix,$1), \
                              $(patsubst %.c,%$3,$2))

# $(call target-files,target,extension)
target-files = $(call target-output,$1,$(call target-src,$1),$2)

# $(call targets-files,target...,extension)
targets-files = $(foreach target,$1,$(call target-files,$(target),$2))

# $(call target-obj,target...)
target-obj = $(call targets-files,$1,.o)

# $(call target-i,target...)
target-i = $(call targets-files,$1,.i)

# $(call target-d,target...)
target-d = $(call targets-files,$1,.d)

# $(call target-includes,target)
target-includes = $(addprefix -I, $(sort $(dir $(call target-src,$1))))

# $(call target-exec-name,target)
target-exec-name = $(subst %,$1,$(EXEC_PATTERN))

# $(call targets-exec-names,target...)
targets-exec-names = $(foreach target,$1,$(call target-exec-name,$(target)))

AUTOMOCK ?= $(dir $(this-makefile))../automock/automock.py

.PHONY: targets
targets: $(call targets-exec-names,$(TARGETS))

# $(eval $(call target-template,$(target)))
define target-template

    $$(call target-exec-name,$1): $$(call target-obj,$1)
	$$(call target-cc,$1) $$(LDFLAGS) $$(call target-ldflags,$1) $$^ \
	    $$(LDLIBS) $$(call target-ldlibs,$1) -o $$@

    $$(call target-buildprefix,$1)%.o: %.c $$(MAKEFILE_LIST)
	@mkdir -p $$(dir $$@)
	$$(call target-cc,$1) $$(CFLAGS) $$(call target-cflags,$1) \
	    $$(call target-includes,$1) $$(CPPFLAGS) $(call target-cppflags,$1) -c -o $$@ $$<

    $$(call target-buildprefix,$1)%.i: %.c $$(MAKEFILE_LIST)
	@mkdir -p $$(dir $$@)
	$$(call target-cc,$1) $$(CFLAGS) $$(call target-cflags,$1) $$(call target-includes,$1) $$(CPPFLAGS) $$(call target-cppflags,$1) \
	    -E -o $$@ $$<

    $$(call target-buildprefix,$1)%.d: %.c $$(MAKEFILE_LIST)
	@mkdir -p $$(dir $$@)
	$$(call target-cc,$1) $$(CFLAGS) $$(call target-cflags,$1) $$(call target-includes,$1) $$(CPPFLAGS) $$(call target-cppflags,$1) \
	    -MM -MP -MT "$$(@) $$(@:%d=%o) $$(@:%d=%i)" -o $$@ $$<

    $$(call target-buildprefix,$1)%$$(mock-suffix).c \
    $$(call target-buildprefix,$1)%$$(mock-suffix).h : %.h $$(MAKEFILE_LIST)
	@mkdir -p $$(dir $$@)
	$$(call target-cc,$1) $$(CFLAGS) $$(call target-cflags,$1) $$(call target-includes,$1) $$(CPPFLAGS) $$(call target-cppflags,$1) \
	  $$(AUTOMOCK_CPPFLAGS) $$(call target-automock-cppflags,$1) -E -o - $$< \
	  | $$(AUTOMOCK) $$(AUTOMOCK_FLAGS) $$(call target-automock-flags,$1) $$< $$(basename $$@)

endef

all-templates =                                         \
    $(foreach target,$(TARGETS),                        \
              $(call target-template,$(target)))

$(eval $(all-templates))

clean-targets:
	rm -f $(call targets-exec-names,$(TARGETS)) \
	  $(sort $(call target-obj,$(TARGETS)) \
	         $(call target-i,$(TARGETS)) \
	         $(call target-d,$(TARGETS)) \
	         $(call targets-mock-files,$(TARGETS)))

# Without this, GNU Make deletes mock C files after regenerating .d
# files, even though they're still needed
.SECONDARY: $(call targets-mock-files,$(TARGETS))
$(call target-d,$(TARGETS)): | $(call targets-mock-h,$(TARGETS))

.PHONY: none
none:
	@:

.PHONY: dump-templates
dump-templates:
	$(info $(all-templates))
	@:

# Don't include .d files if a command-line goal contains "clean"
ifeq ($(findstring clean,$(MAKECMDGOALS)),)
-include $(call target-d,$(TARGETS))
endif
