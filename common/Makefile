CC = gcc

SOURCEDIR := ./
SOURCES := $(wildcard $(SOURCEDIR)/*.c)
OBJDIR=$(SOURCEDIR)/obj

OBJECTS := $(patsubst $(SOURCEDIR)/%.c,$(OBJDIR)/%.o, $(SOURCES))
DEPENDS := $(patsubst $(SOURCEDIR)/%.c,$(OBJDIR)/%.d, $(SOURCES))

# ADD MORE WARNINGS!
WARNING := -Wall -Wextra

all: $(OBJECTS)

$(OBJDIR)/%.o: $(SOURCEDIR)/%.c Makefile | $(OBJDIR)
	$(CC) $(WARNING) -MMD -MP -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)
