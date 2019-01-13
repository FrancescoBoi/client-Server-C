CXX = gcc

#CXXFLAGS = -std=c++11
#INC_PATH = `pkg-config --cflags ../openCV/build/lib/pkgconfig/opencv.pc` \
#	`pkg-config --cflags ../SDL2-2.0.8/instDir/lib/pkgconfig/sdl2.pc` \
#	`pkg-config --cflags ../jsoncpp/build/pkg-config/jsoncpp.pc` \
#	-I ../poco/instDir/include/

#LIBS = 	`pkg-config --libs ../openCV/build//lib/pkgconfig/opencv.pc` \
#	`pkg-config --libs ../SDL2-2.0.8/instDir/lib/pkgconfig/sdl2.pc` \
#	`pkg-config --libs ../jsoncpp/build/pkg-config/jsoncpp.pc` \
#	-L../poco/instDir/lib/ -lPocoNetd -lPocoUtild -lPocoFoundationd \

SOURCEDIR := ./
SOURCES := $(wildcard $(SOURCEDIR)/*.c)
OBJDIR= ./obj
OBJECTS := $(patsubst $(SOURCEDIR)/%.c,$(OBJDIR)/%.o, $(SOURCES))
DEPENDS := $(patsubst $(SOURCEDIR)/%.c,$(OBJDIR)/%.d, $(SOURCES))

COMMONDIR := ../common
SOURCESCOMMON := $(wildcard $(COMMONDIR)/*.c)
OBJDIRCOMMON := $(COMMONDIR)/obj
OBJECTSCOMMON := $(patsubst $(COMMONDIR)/%.c,$(OBJDIRCOMMON)/%.o, $(SOURCESCOMMON))
DEPENDSCOMON  := $(patsubst $(COMMONDIR)/%.c,$(OBJDIRCOMMON)/%.d, $(SOURCESCOMMON))

# ADD MORE WARNINGS!
WARNING := -Wall -Wextra

EXECUTABLE := ../client

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
$(EXECUTABLE): $(OBJECTSCOMMON) $(OBJECTS)
	$(CXX) $(WARNING) $^ -o $@
	#$(CXX) $(WARNING) $(CXXFLAGS) $(INC_PATH) $^ -o $@ $(LIBS)

-include $(DEPENDS) $(DEPENDSCOMON)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SOURCEDIR)/%.c Makefile | $(OBJDIR)
	$(CXX) $(WARNING) -MMD -MP -c $< -o $@

$(OBJDIRCOMMON):
	mkdir -p $(OBJDIRCOMMON)

$(OBJECTSCOMMON): $(SOURCESCOMMON) ../common/Makefile | $(OBJDIRCOMMON)
	$(CC) $(WARNING) -MMD -MP -c $< -o $@
