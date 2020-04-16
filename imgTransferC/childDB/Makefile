CXX      := g++
CXXFLAGS := -std=c++11 -pg
INC_PATH := `pkg-config --cflags opencv4`
LIBS     := 	`pkg-config --libs opencv4`
BUILD_DIR_NAME := build
BUILDDIR       := ../../$(BUILD_DIR_NAME)
CURR_DIR_NAME  := $(shell basename $(CURDIR))
SOURCEDIR := ./
SOURCES   := $(wildcard $(SOURCEDIR)/*.cpp)
OBJDIR    := $(BUILDDIR)/$(CURR_DIR_NAME)

OBJECTS := $(patsubst $(SOURCEDIR)/%.cpp,$(OBJDIR)/%.o,	$(SOURCES))
DEPENDS := $(patsubst $(SOURCEDIR)/%.cpp,$(OBJDIR)/%.d, $(SOURCES))

# ADD MORE WARNINGS!
WARNING := -Wall -Wextra

EXECUTABLE := openCVClient

# .PHONY means these rules get executed even if
# files of those names exist.
.PHONY: all clean

# The first rule is the default, ie. "make",
# "make all" and "make parking" mean the same
all: $(EXECUTABLE)

clean:
	$(RM) $(OBJECTS) $(DEPENDS) $(EXECUTABLE)

# Linking the executable from the object files
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(WARNING) $(CXXFLAGS) $(INC_PATH) $^ -o $@ $(LIBS)

-include $(DEPENDS)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SOURCEDIR)/%.cpp Makefile | $(OBJDIR)
	$(CXX) $(WARNING) $(CXXFLAGS) $(INC_PATH) -MMD -MP -c $< -o $@
