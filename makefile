# makefile - build system for llvm-ks
#
#
# @author: Cade Brown <cade@kscript.org>


# -*- Config -*-

# kscript directory
KS             ?= /usr/local

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

mod_SO         := libksm_llvm.so



# -*- Rules -*-

.PHONY: default all clean


default: $(mod_SO)

all: $(mod_SO)

clean:
	rm -f $(wildcard $(src_O) $(mod_SO))

$(mod_SO): $(src_O)
	$(CXX) $(FPIC) \
		$^ \
		$(LDFLAGS) -shared -o $@

%.o: %.cc $(src_HH)
	$(CXX) $(CXXFLAGS) -Iinclude -fPIC -c -o $@ $<


