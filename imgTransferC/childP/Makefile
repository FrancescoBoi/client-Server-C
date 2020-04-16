CXX      := g++
CXXFLAGS := -std=c++11 -pg
COMMONDIR_NAME := common
COMMONDIR      := ../../$(COMMONDIR_NAME)
INC_PATH := `pkg-config --cflags opencv4` -I$(COMMONDIR)
LIBS     := `pkg-config --libs opencv4`
BUILD_DIR_NAME := build
BUILDDIR       := ../../$(BUILD_DIR_NAME)
CURR_DIR_NAME  := $(shell basename $(CURDIR))
SOURCEDIR := ./
SOURCES   := $(wildcard $(SOURCEDIR)/*.cpp)
OBJDIR    := $(BUILDDIR)/$(CURR_DIR_NAME)

OBJECTS := $(patsubst $(SOURCEDIR)/%.cpp,$(OBJDIR)/%.o,	$(SOURCES))
DEPENDS := $(patsubst $(SOURCEDIR)/%.cpp,$(OBJDIR)/%.d, $(SOURCES))
OBJDIRCOMMON   := $(BUILDDIR)/$(COMMONDIR_NAME)
LIBNAME        := libcommon.a
LIBDIR         := $(COMMONDIR)/lib
COMMONLIB      := $(LIBDIR)/$(LIBNAME)
.PHONY: buildlib clean all

# ADD MORE WARNINGS!
WARNING := -Wall -Wextra

EXECUTABLE := openCV
# .PHONY means these rules get executed even if
# files of those names exist.
.PHONY: all clean

# The first rule is the default, ie. "make",
# "make all" and "make parking" mean the same
all: $(EXECUTABLE)

clean:
	$(RM) $(OBJECTS) $(DEPENDS) $(EXECUTABLE)

# Linking the executable from the object files
$(EXECUTABLE): $(OBJECTS) $(COMMONLIB)
	$(CXX) $(WARNING) $(CXXFLAGS) $(INC_PATH) $^ -o $@ $(LIBS)

-include $(DEPENDS)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SOURCEDIR)/%.cpp Makefile | $(OBJDIR)
	$(CXX) $(WARNING) $(CXXFLAGS) $(INC_PATH) -MMD -MP -c $< -o $@
$(OBJDIRCOMMON):
	mkdir -p $(OBJDIRCOMMON)

$(COMMONLIB): FORCE
	+$(MAKE) -C $(COMMONDIR)

FORCE:
