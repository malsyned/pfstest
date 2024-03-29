# SPDX-License-Identifier: LGPL-2.1-or-later

# $(call reverse,list)
reverse = $(if $(1),$(call reverse,$(wordlist 2,$(words $(1)),$(1)))) $(firstword $(1))

# $(call parent,path)
parent = $(dir $(patsubst %/,%,$(1)))

# $(call parent-walk,path)
# Return $(1) and all parent paths of $(1)
parent-walk = $(if $(filter-out ./,$(1)),$(1) $(call parent-walk,$(call parent,$(1))))

# $(call parents,paths)
parents = $(foreach file,$(1),$(call parent-walk,$(call parent,$(file))))

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

# $(call targets-buildprefix,target...)
targets-buildprefix = $(foreach target,$1,$(call target-buildprefix,$(target)))

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

# $(call target-cc,target)
target-cc = $(call class-cc,$(call target-class,$1))

# $(call target-ldflags,target)
target-ldflags = $(call class-ldflags,$(call target-class,$1))

# $(call target-ldlibs,target)
target-ldlibs = $(call class-ldlibs,$1,$(call target-class,$1))

# $(call target-src,target)
target-src = $(call target-param,$1,SRC) $(SRC)

# $(call targets-src,targets...)
targets-src = $(foreach target,$1,$(call target-src,$(target)))

# $(call rmprefix,prefix,files...)
rmprefix = $(patsubst $(1)%,%,$2)

# $(call rm-srcprefix,files...)
rm-srcprefix = $(call rmprefix,$(SRCPREFIX),$1)

# $(call target-buildpaths,target,files...)
target-buildpaths = $(addprefix $(call target-buildprefix,$1), \
                                $(call rm-srcprefix,$2))

# $(call target-output,target,file...,extension)
target-output = $(patsubst %.c,%$3,$(call target-buildpaths,$1,$2))

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

makefile-list = $(filter-out $(call target-d,$(TARGETS)),$(MAKEFILE_LIST))

class-includes =                                                          \
    $(addprefix -I,                                                       \
      $(patsubst %/,%,                                                    \
        $(sort $(dir $(call targets-src,$(call targets-in-class,$1))))))

# $(call target-bin-name,target)
target-bin-name = $(subst %,$1,$(BIN_PATTERN))

# $(call targets-bin-names,target...)
targets-bin-names = $(foreach target,$1,$(call target-bin-name,$(target)))

# $(eval $(call target-template,$(target)))
define target-template

    $$(call target-bin-name,$1): $$(call target-obj,$1)
	$$(call target-cc,$1) $$(LDFLAGS) $$(call target-ldflags,$1) $$^ \
	    $$(LDLIBS) $$(call target-ldlibs,$1) -o $$@

endef

# $(call class-object-recipe-template,class,source-c)
define class-object-recipe-template

    $$(call class-buildprefix,$1)%.o: $2 $$(makefile-list)
	@mkdir -p $$(dir $$@)
	$$(call class-cc,$1) $$(CFLAGS) $$(call class-cflags,$1)        \
	    $$(CPPFLAGS) $$(call class-includes,$1)                     \
	    $$(call class-cppflags,$1) -c -o $$@ $$<

    $$(call class-buildprefix,$1)%.i: $2 $$(makefile-list)
	@mkdir -p $$(dir $$@)
	$$(call class-cc,$1) $$(CFLAGS) $$(call class-cflags,$1)        \
	    $$(call class-cppflags,$1) $$(CPPFLAGS)                     \
	    $$(call class-includes,$1) -E -o $$@ $$<

    $$(call class-buildprefix,$1)%.d: $2 $$(makefile-list)
	@mkdir -p $$(dir $$@)
	$$(call class-cc,$1) $$(CFLAGS) $$(call class-cflags,$1)        \
	    $$(call class-includes,$1) $$(CPPFLAGS)                     \
	    $$(call class-cppflags,$1) -MM -MP                          \
	    -MT "$$(@) $$(@:%d=%o) $$(@:%d=%i)" -o $$@ $$<

endef

define class-template

    $(call class-object-recipe-template,$1,$$(SRCPREFIX)%.c)
    $(call class-object-recipe-template,$1,%.c)

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

# NOTE: Plugins should append to clean-files, not to clean-dirs,
#       clean-files-minimal, or clean-dirs-minimal
clean-files = $(call targets-bin-names,$(TARGETS)) \
              $(addsuffix .exe, $(call targets-bin-names,$(TARGETS))) \
              $(call target-obj,$(TARGETS)) \
              $(call target-i,$(TARGETS)) \
              $(call target-d,$(TARGETS))

clean-dirs = $(call reverse,$(sort $(call parents,$(clean-files))))

clean-files-minimal = $(wildcard $(sort $(clean-files)))

all-buildprefixes = $(call targets-buildprefix,$(TARGETS))

clean-dirs-minimal = $(filter $(addsuffix %,$(all-buildprefixes)), \
                     $(wildcard $(clean-dirs)))

# MULTITARGET_PLUGINS:
#
# This file aims to be a generic, flexible way to build multiple
# targets in the same make process. Projects might want to augment its
# behavior with project-specific features. For example, pfstest
# automock automatically generates some of the C source files for
# different targets.
#
# Due to dependency issues between variable and rule definitions, it
# may not be possible get the desired behavior by including an
# additional make fragment either before or after this file. For those
# cases, MULTITARGET_PLUGINS can be used to inject another Makefile
# fragment within this one, between the variables and rule
# definitions.
include $(MULTITARGET_PLUGINS)

$(eval $(all-templates))

.PHONY: all
all: targets
.DEFAULT_GOAL := all

.PHONY: targets
targets: $(call targets-bin-names,$(TARGETS))

.PHONY: clean
clean:
	$(if $(clean-files-minimal),rm -f $(clean-files-minimal))
	$(if $(clean-dirs-minimal),rmdir $(clean-dirs-minimal))

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
