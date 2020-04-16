CC = gcc

CURRENT_DIR := $(shell basename $(CURDIR))
SOURCEDIR   := ./
SOURCES     := $(wildcard $(SOURCEDIR)/*.c)
OBJDIR      := ../build/$(CURRENT_DIR)
LIBDIR      := $(SOURCEDIR)/lib
LIBNAME     := libcommon.a
COMMONLIB   := $(LIBDIR)/$(LIBNAME)

OBJECTS     := $(patsubst $(SOURCEDIR)/%.c,$(OBJDIR)/%.o, $(SOURCES))
DEPENDS     := $(patsubst $(SOURCEDIR)/%.c,$(OBJDIR)/%.d, $(SOURCES))

.PHONY: all clean

# ADD MORE WARNINGS!
WARNING := -Wall -Wextra

all: $(COMMONLIB)

$(LIBDIR):
	mkdir -p $(LIBDIR)

clean:
	$(RM) $(OBJECTS) $(DEPENDS) $(COMMONLIB)

-include $(DEPENDS)

$(OBJDIR)/%.o: $(SOURCEDIR)/%.c Makefile | $(OBJDIR)
	$(CC) $(WARNING) -MMD -MP -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)


$(COMMONLIB): $(OBJECTS) | $(LIBDIR)
	$(AR) $(ARFLAGS) $@ $^
