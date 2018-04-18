this-makefile := $(lastword $(MAKEFILE_LIST))

# $(call target-param-name,target,param)
target-param-name = $1_$2

# $(call target-param,target,param)
target-param = $($(call target-param-name,$1,$2))

# $(call target-class,target)
target-class = $(call target-param,$1,CLASS)

classes = $(sort $(foreach target,$(TARGETS),$(call target-class,$(target))))

# $(call class-param-name,class,param)
class-param-name = $1_$2

# $(call class-param,class,param)
class-param = $($(call class-param-name,$1,$2))

targets-in-default-class = $(foreach target,$(TARGETS),                 \
                             $(if $(call target-class,$(target)),       \
                                  ,                                     \
                                  $(target)))

# $(call targets-in-defined-class,class)
targets-in-defined-class =              \
    $(foreach target,$(TARGETS),        \
              $(if $(filter $1,$(call target-class,$(target))),$(target)))

# $(call targets-in-class,class)
# supports empty class name
targets-in-class = $(if $1,$(call targets-in-defined-class,$1), \
                           $(targets-in-default-class))

# $(call class-buildprefix,class)
class-buildprefix = $(or $(call class-param,$1,BUILDPREFIX), \
                          $(BUILDPREFIX))

# $(call target-buildprefix,target)
target-buildprefix = $(call class-buildprefix,$(call target-class,$1))

# $(call target-mocks,target)
target-mock-reqs = $(call target-param,$1,MOCKS)

mock-suffix = .mock

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

# $(call class-cc,class)
class-cc = $(or $(call class-param,$1,CC),$(CC))

# $(call class-cflags,class)
class-cflags = $(call class-param,$1,CFLAGS)

# $(call class-cppflags,class)
class-cppflags = $(call class-param,$1,CPPFLAGS)

# $(call class-ldflags,class)
class-ldflags = $(call class-param,$1,LDFLAGS)

# $(call class-ldlibs,class)
class-ldlibs = $(call class-param,$1,LDLIBS)

# $(call class-automock-cppflags,class)
class-automock-cppflags = $(call class-param,$1,AUTOMOCK_CPPFLAGS)

# $(call class-automock-flags,class)
class-automock-flags = $(call class-param,$1,AUTOMOCK_FLAGS)

# $(call target-cc,target)
target-cc = $(call class-cc,$(call target-class,$1))

# $(call target-ldflags,target)
target-ldflags = $(call class-ldflags,$(call target-class,$1))

# $(call target-ldlibs,target)
target-ldlibs = $(call class-ldlibs,$1,$(call target-class,$1))

# $(call target-src,target)
target-src = $(call target-param,$1,SRC) $(call target-mock-src,$1) $(SRC)

# $(call targets-src,targets...)
targets-src = $(foreach target,$1,$(call target-src,$(target)))

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

# $(call class-includes,class)
class-includes =                                                        \
    $(addprefix -I,$(sort $(dir $(call targets-src,                     \
                                       $(call targets-in-class,$1)))))

# $(call target-exec-name,target)
target-exec-name = $(subst %,$1,$(EXEC_PATTERN))

# $(call targets-exec-names,target...)
targets-exec-names = $(foreach target,$1,$(call target-exec-name,$(target)))

PYTHON ?= python
AUTOMOCK_SRC ?= $(dir $(this-makefile))../automock/automock.py
AUTOMOCK ?= $(PYTHON) $(AUTOMOCK_SRC)

.PHONY: targets
targets: $(call targets-exec-names,$(TARGETS))

# $(eval $(call target-template,$(target)))
define target-template

    $$(call target-exec-name,$1): $$(call target-obj,$1)
	$$(call target-cc,$1) $$(LDFLAGS) $$(call target-ldflags,$1) $$^ \
	    $$(LDLIBS) $$(call target-ldlibs,$1) -o $$@

endef

define class-template

    $$(call class-buildprefix,$1)%.o: %.c $$(MAKEFILE_LIST)
	@mkdir -p $$(dir $$@)
	$$(call class-cc,$1) $$(CFLAGS) $$(call class-cflags,$1)        \
	    $$(call class-includes,$1) $$(CPPFLAGS)                     \
	    $$(call class-cppflags,$1) -c -o $$@ $$<

    $$(call class-buildprefix,$1)%.i: %.c $$(MAKEFILE_LIST)
	@mkdir -p $$(dir $$@)
	$$(call class-cc,$1) $$(CFLAGS) $$(call class-cflags,$1)        \
	    $$(call class-includes,$1) $$(CPPFLAGS)                     \
	    $$(call class-cppflags,$1) -E -o $$@ $$<

    $$(call class-buildprefix,$1)%.d: %.c $$(MAKEFILE_LIST)
	@mkdir -p $$(dir $$@)
	$$(call class-cc,$1) $$(CFLAGS) $$(call class-cflags,$1)        \
	    $$(call class-includes,$1) $$(CPPFLAGS)                     \
	    $$(call class-cppflags,$1) -MM -MP                          \
	    -MT "$$(@) $$(@:%d=%o) $$(@:%d=%i)" -o $$@ $$<

    $$(call class-buildprefix,$1)%$$(mock-suffix).c \
    $$(call class-buildprefix,$1)%$$(mock-suffix).h \
    : %.h $$(AUTOMOCK_SRC) $$(MAKEFILE_LIST)
	@mkdir -p $$(dir $$@)
	$$(call class-cc,$1) $$(CFLAGS) $$(call class-cflags,$1)        \
	    $$(call class-includes,$1) $$(CPPFLAGS)                     \
	    $$(call class-cppflags,$1) $$(AUTOMOCK_CPPFLAGS)            \
	    $$(call class-automock-cppflags,$1) -E -o - $$<             \
	    | $$(AUTOMOCK) $$(AUTOMOCK_FLAGS)                           \
	      $$(call class-automock-flags,$1) $$< $$(basename $$@)

endef

target-templates =                                      \
    $(foreach target,$(TARGETS),                        \
              $(call target-template,$(target)))

class-templates =                               \
    $(call class-template,)                     \
    $(foreach class,$(classes),                 \
              $(call class-template,$(class)))

all-templates =         \
    $(target-templates) \
    $(class-templates)

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
