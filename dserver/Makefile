CC = gcc

INC_PATH = -I../common/

SOURCEDIR := ./
SOURCES := $(wildcard $(SOURCEDIR)/*.c)
OBJDIR  :=./obj
OBJECTS := $(patsubst $(SOURCEDIR)/%.c,$(OBJDIR)/%.o, $(SOURCES))
DEPENDS := $(patsubst $(SOURCEDIR)/%.c,$(OBJDIR)/%.d, $(SOURCES))

COMMONDIR     := ../common
SOURCESCOMMON := $(wildcard $(COMMONDIR)/*.c)
OBJDIRCOMMON  := $(COMMONDIR)/obj
OBJECTSCOMMON := $(patsubst $(COMMONDIR)/%.c,$(OBJDIRCOMMON)/%.o, $(SOURCESCOMMON))
DEPENDSCOMMON := $(patsubst $(COMMONDIR)/%.c,$(OBJDIRCOMMON)/%.d, $(SOURCESCOMMON))

# ADD MORE WARNINGS!
WARNING := -Wall -Wextra

# OBJS_LOC is in current working directory,
EXECUTABLE := ../server
# .PHONY means these rules get executed even if
# files of those names exist.
.PHONY: all clean

# The first rule is the default, ie. "make",
# "make all" and "make parking" mean the same
all: $(EXECUTABLE)

clean:
	$(RM) $(OBJECTS) $(DEPENDS) $(EXECUTABLE)

# Linking the executable from the object files
# $^   # "src.c src.h" (all prerequisites)
$(EXECUTABLE): $(OBJECTS) $(OBJECTSCOMMON)
	$(CC) $(WARNING) $^ -o $@

-include $(DEPENDS) $(DEPENDSCOMMON)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SOURCEDIR)/%.c Makefile | $(OBJDIR)
	$(CC) $(WARNING) -MMD -MP -c $(INC_PATH) $< -o $@

$(OBJDIRCOMMON):
	mkdir -p $(OBJDIRCOMMON)

$(OBJDIRCOMMON)/%.o: $(COMMONDIR)/Makefile | $(OBJDIRCOMMON)
	+$(MAKE) -C $(COMMONDIR)
