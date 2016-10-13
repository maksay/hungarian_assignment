CC := g++
CFLAGS := -O2 -Wall -m64 -g -MMD

# Folders
SRCDIR := src
BUILDDIR := build
TARGETDIR := bin
VERBOSE := 1

# Code Lists
SRCEXT := cc
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
TARGETS := $(patsubst $(SRCDIR)/%,$(TARGETDIR)/%,$(SOURCES:.$(SRCEXT)=))

all: $(TARGETDIR)/hungarian_solver $(TARGETDIR)/test

$(TARGETDIR)/hungarian_solver: $(BUILDDIR)/hungarian_solver.o
	@if [ $(VERBOSE) = 1 ]; then echo "Linking $<..."; fi
	@mkdir -p $(TARGETDIR)
	@$(CC) -o $@ $^

$(TARGETDIR)/test: $(BUILDDIR)/test.o
	@if [ $(VERBOSE) = 1 ]; then echo "Linking $<..."; fi
	@mkdir -p $(TARGETDIR)
	@$(CC) -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@if [ $(VERBOSE) = 1 ]; then echo "Compiling $<..."; fi
	@mkdir -p $(BUILDDIR)
	@$(CC) $(CFLAGS) -c -o $@ $<

-include $(BUILDDIR)/*.d

.SECONDARY: $(OBJECTS)

clean:
	@if [ $(VERBOSE) = 1 ]; then echo "Cleaning..."; fi
	@$(RM) -r $(BUILDDIR) $(TARGETDIR) input.txt output.txt

.PHONY: all clean
