BINARY := prog

CC := gcc
CXX := g++
CXXFLAGS += -std=c++11
#CXX := clang++
RM := rm -f
RMDIR := rm -rf

# extra libraries if required
LDFLAGS += 

# each module will add to this
SRCDIR := src
SRC := $(shell find $(SRCDIR) -name '*.cpp')
OBJDIR := build
OBJ := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))


# include the description for each module
include $(patsubst %,%/module.mk,$(MODULES))
# determine the object files
#OBJ := 	$(patsubst %.c,%.o, \
#	$(filter %.c,$(SRC))) \
#	$(patsubst %.cpp,%.o, \
#	$(filter %.cpp,$(SRC))) \
#	$(patsubst %.y,%.o, \
#	$(filter %.y,$(SRC)))

DEFS := $(OBJ:.o=.d)

.PHONY: all clean setup

all: setup $(BINARY)

setup:
	mkdir -p $(OBJDIR)

# link the program
$(BINARY): $(OBJ)
	$(CXX) -o $@ $(OBJ) $(LDFLAGS)

# include the C include dependencies
include $(OBJ:.o=.d)

#$(OBJDIR)/%.o: $(SRCDIR)/%.c
#	echo $<
#	echo $@
#	$(CXX) $(CXXFLAGS) -c $< -o $@


# calculate C include dependencies
$(OBJDIR)/%.d: $(SRCDIR)/%.c
	mkdir -p `dirname $@` 
	sh depend.sh `dirname $@` `dirname $<` $@ $(CFLAGS) $< > $@

#%.d: %.cpp
$(OBJDIR)/%.d: $(SRCDIR)/%.cpp
	mkdir -p `dirname $@` 
	sh depend.sh `dirname $@` `dirname $<` $@ $(CFLAGS) $< > $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	$(RM) prog
	$(RMDIR) $(OBJDIR)

