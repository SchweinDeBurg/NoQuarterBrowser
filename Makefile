SHELL ?= /bin/bash
CMAKE ?= cmake
RM := rm --force --recursive
MKDIR := mkdir --parents

THISDIR := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
BUILDDIR := $(THISDIR)/Build

all: $(BUILDDIR)/Makefile
	@ $(MAKE) -C $(BUILDDIR)

$(BUILDDIR)/Makefile:
	@ $(MKDIR) $(BUILDDIR)
	@ $(CMAKE) -S $(THISDIR) -B $(BUILDDIR) -G "Unix Makefiles"

distclean:
	@  $(MKDIR) $(BUILDDIR)
	@  $(CMAKE) -S $(THISDIR) -B $(BUILDDIR) -G "Unix Makefiles" >/dev/null 2>&1
	@- $(MAKE) --silent -C $(BUILDDIR) clean
	@- $(RM) $(BUILDDIR)/CMakeCache.txt
	@- $(RM) $(BUILDDIR)/CMakeFiles
	@- $(RM) $(BUILDDIR)/cmake_install.cmake
	@- $(RM) $(BUILDDIR)/Makefile

ifeq ($(findstring distclean,$(MAKECMDGOALS)),)
$(MAKECMDGOALS): $(BUILDDIR)/Makefile
	@ $(MAKE) -C $(BUILDDIR) $(MAKECMDGOALS)
endif
