CC = gcc
COMMONDIR_NAME := common
COMMONDIR      := ../$(COMMONDIR_NAME)
INC_PATH       := -I$(COMMONDIR)
BUILD_DIR_NAME := build
BUILDDIR       := ../$(BUILD_DIR_NAME)
CURR_DIR_NAME  := $(shell basename $(CURDIR))
SOURCEDIR      := ./
SOURCES        := $(wildcard $(SOURCEDIR)/*.c)
OBJDIR         := $(BUILDDIR)/$(CURR_DIR_NAME)
OBJECTS        := $(patsubst $(SOURCEDIR)/%.c,$(OBJDIR)/%.o, $(SOURCES))
DEPENDS        := $(patsubst $(SOURCEDIR)/%.c,$(OBJDIR)/%.d, $(SOURCES))
OBJDIRCOMMON   := $(BUILDDIR)/$(COMMONDIR_NAME)
LIBNAME        := libcommon.a
LIBDIR         := $(COMMONDIR)/lib
COMMONLIB      := $(LIBDIR)/$(LIBNAME)
.PHONY: buildlib clean all

# ADD MORE WARNINGS!
WARNING := -Wall -Wextra

EXECUTABLE := ../clientVidConnectionless
# .PHONY means these rules get executed even if
# files of those names exist.
.PHONY: all clean

# The first rule is the default, ie. "make",
# "make all" and "make parking" mean the same
all: $(EXECUTABLE)

clean:
	$(RM) $(OBJECTS) $(DEPENDS) $(EXECUTABLE)
	+$(MAKE) -C $(COMMONDIR) clean

# Linking the executable from the object files
$(EXECUTABLE): $(OBJECTS) $(COMMONLIB)
	$(CC) $(WARNING) $^ -o $@

-include $(DEPENDS)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(OBJDIR)/%.o: $(SOURCEDIR)/%.c Makefile | $(OBJDIR)
	$(CC) $(WARNING) -MMD -MP -c $(INC_PATH) $< -o $@

$(OBJDIRCOMMON):
	mkdir -p $(OBJDIRCOMMON)

$(COMMONLIB): FORCE
	+$(MAKE) -C $(COMMONDIR)

FORCE:
