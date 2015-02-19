# Simple makefile by Johannes Gustafsson
#
# inspired by http://aegis.sourceforge.net/auug97.pdf 
#
# 1. scans all *.cpp in src folder
# 2. generates dependency file for every source file, stored in obj folder
# 3. all object files are stored in obj folder
# 4. output binary are stored in bin folder

BINARY := prog

#CXX := g++
CXX := clang++
CXXFLAGS := -std=c++11 -Wall -Wno-format-security -g -m64
LDFLAGS := -lzmq -lczmq

RM := rm -f
RMDIR := rm -rf
MKDIR := mkdir -p

BINDIR := bin
SRCDIR := src
OBJDIR := obj

BIN := $(BINDIR)/$(BINARY)
SRC := $(shell find $(SRCDIR) -name '*.cpp')
OBJ := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))

.PHONY: default clean 

default: $(BIN)

clean:
	$(RMDIR) $(BINDIR)
	$(RMDIR) $(OBJDIR)

# link the program
$(BIN): $(OBJ)
	$(MKDIR) $(@D)
	$(CXX) -o $@ $(OBJ) $(LDFLAGS)

#compile
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# include dependencies
include $(OBJ:.o=.d)

# calculate include dependencies
$(OBJDIR)/%.d: $(SRCDIR)/%.cpp
	$(MKDIR) $(@D) 
	$(CXX) -MM  $< $(CXXFLAGS) | sed -e "s@^\(.*\)\.o:@$(@D)/\1.d $(@D)/\1.o:@" > $@
