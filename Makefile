BINARY := prog

CXX := g++
CXXFLAGS := -std=c++11
#CXX := clang++
LDFLAGS := 

RM := rm -f
RMDIR := rm -rf

SRCDIR := src
OBJDIR := obj

SRC := $(shell find $(SRCDIR) -name '*.cpp')
OBJ := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))

# include the description for each module
#include $(patsubst %,%/module.mk,$(MODULES))

.PHONY: all clean setup

all: setup $(BINARY)

setup:
	mkdir -p $(OBJDIR)

clean:
	$(RM) prog
	$(RMDIR) $(OBJDIR)

# link the program
$(BINARY): $(OBJ)
	$(CXX) -o $@ $(OBJ) $(LDFLAGS)

#compile
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# include dependencies
include $(OBJ:.o=.d)

# calculate include dependencies
$(OBJDIR)/%.d: $(SRCDIR)/%.cpp
	mkdir -p `dirname $@` 
	sh depend.sh `dirname $@` $(CXXFLAGS) $< > $@
