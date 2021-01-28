# makefile - build system for llvm-ks
#
#
# @author: Cade Brown <cade@kscript.org>


# -*- Config -*-

# kscript module name
NAME           := llvm

# kscript directory
KS             ?= /usr/local

# install path
PREFIX         := /usr/local
DESTDIR        ?= 
TODIR          := $(DESTDIR)$(PREFIX)

# Add from the LLVM configuration
CXXFLAGS       += `llvm-config --cxxflags`
LDFLAGS        += -Wl,-znodelete `llvm-config core native --link-static --ldflags --libs` -ldl -lm

# Add from the kscript configuration
CXXFLAGS       += -I$(KS)/include
LDFLAGS        += -L$(KS)/lib

# DEBUG
#CXXFLAGS += -g


# -*- Files -*-

src_CC         := $(wildcard src/*.cc)
src_HH         := $(wildcard include/*.hh)

src_O          := $(patsubst %.cc,%.o,$(src_CC))


# -*- Output -*-

# output shared object file
mod_SO         := ksm_$(NAME).so


# -*- Rules -*-

.PHONY: default all clean install uninstall FORCE


default: $(mod_SO)

all: $(mod_SO)

clean: FORCE
	rm -f $(wildcard $(src_O) $(mod_SO))

install: FORCE
	install -d $(TODIR)/lib/ks/pkgs/$(NAME)
	install -m 664 $(mod_SO) $(TODIR)/lib/ks/pkgs/$(NAME)/$(notdir $(mod_SO))
	strip $(TODIR)/lib/ks/pkgs/$(NAME)/$(notdir $(mod_SO))

uninstall: FORCE
	rm -rf $(TODIR)/lib/ks/pkgs/$(NAME)

FORCE:

$(mod_SO): $(src_O)
	$(CXX) $(FPIC) \
		$^ \
		$(LDFLAGS) -shared -o $@

%.o: %.cc $(src_HH)
	$(CXX) $(CXXFLAGS) -Iinclude -fPIC -c -o $@ $<


