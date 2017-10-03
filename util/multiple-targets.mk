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

# $(call target-src,target)
target-src = $(call target-param,$1,SRC) $(SRC)

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
target-includes = $(addprefix -I,$(sort $(dir $(call target-src,$1))))

# $(call target-exec-name,target)
target-exec-name = $(subst %,$1,$(EXEC_PATTERN))

# $(call targets-exec-names,target...)
targets-exec-names = $(foreach target,$1,$(call target-exec-name,$(target)))

.PHONY: targets
targets: $(call targets-exec-names,$(TARGETS))

# $(eval $(call target-template,$(target)))
define target-template

    $(call target-exec-name,$1): $$(call target-obj,$1)
	$$(call target-cc,$1) $$(LDFLAGS) $$(call target-ldflags,$1) $$^ \
	    $$(LDLIBS) $$(call target-ldlibs,$1) -o $$@

    $$(call target-buildprefix,$1)%.o: %.c $$(MAKEFILE_LIST)
	@mkdir -p $$(dir $$@)
	$$(call target-cc,$1) $$(CFLAGS) $$(call target-cflags,$1) \
	    $$(call target-includes,$1) $$(CPPFLAGS) $(call target-cppflags,$1) -c -o $$@ $$<

    $$(call target-buildprefix,$1)%.i: %.c $$(MAKEFILE_LIST)
	@mkdir -p $$(dir $$@)
	$$(call target-cc,$1) $$(call target-includes,$1) $$(CPPFLAGS) $$(call target-cppflags,$1) \
	    -E -o $$@ $$<

    $$(call target-buildprefix,$1)%.d: %.c $$(MAKEFILE_LIST)
	@mkdir -p $$(dir $$@)
	$$(call target-cc,$1) $$(call target-includes,$1) $$(CPPFLAGS) $$(call target-cppflags,$1) \
	    -MM -MG -MP -MT "$$(@) $$(@:%d=%o)" -o $$@ $$<

endef

all-templates =                                         \
    $(foreach target,$(TARGETS),                        \
              $(call target-template,$(target)))

$(eval $(all-templates))

clean-targets:
	rm -f $(call targets-exec-names,$(TARGETS)) \
	  $(sort $(call target-obj,$(TARGETS)) \
	         $(call target-i,$(TARGETS)) \
	         $(call target-d,$(TARGETS)))

.PHONY: none
none:
	@:

.PHONY: dump-templates
dump-templates:
	$(info $(all-templates))
	@:

-include $(call target-d,$(TARGETS))
