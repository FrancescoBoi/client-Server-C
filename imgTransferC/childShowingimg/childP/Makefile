CXX = g++

CXXFLAGS = -std=c++11 -pg
INC_PATH = `pkg-config --cflags /home/fra/Documents/openCV//openCV/build/lib/pkgconfig/opencv.pc` \
	`pkg-config --cflags /home/fra/Documents/openCV/SDL2-2.0.8/instDir/lib/pkgconfig/sdl2.pc` \
	`pkg-config --cflags /home/fra/Documents/openCV/jsoncpp/build/pkg-config/jsoncpp.pc` \
	-I /home/fra/Documents/openCV//poco/instDir/include/

#LIB_PATH = -L../cmake_bin_dir/lib/ ./gainput/build/lib -L../SDL2-2.0.8/build/ -L../SDL2-2.0.8/build/lib
LIBS = 	`pkg-config --libs /home/fra/Documents/openCV/openCV/build//lib/pkgconfig/opencv.pc` \
	`pkg-config --libs /home/fra/Documents/openCV/SDL2-2.0.8/instDir/lib/pkgconfig/sdl2.pc` \
	`pkg-config --libs /home/fra/Documents/openCV/jsoncpp/build/pkg-config/jsoncpp.pc` \
	-L/home/fra/Documents/openCV/poco/instDir/lib/ -lPocoNetd -lPocoUtild -lPocoFoundationd \

SOURCEDIR := ./
SOURCES := $(wildcard $(SOURCEDIR)/*.cpp)
OBJDIR=$(SOURCEDIR)/obj

OBJECTS := $(patsubst $(SOURCEDIR)/%.cpp,$(OBJDIR)/%.o,	$(SOURCES))
DEPENDS := $(patsubst $(SOURCEDIR)/%.cpp,$(OBJDIR)/%.d,$(SOURCES))

# ADD MORE WARNINGS!
WARNING := -Wall -Wextra

# .PHONY means these rules get executed even if
# files of those names exist.
.PHONY: all clean

# The first rule is the default, ie. "make",
# "make all" and "make parking" mean the same
all: openCV

clean:
	$(RM) $(OBJECTS) $(DEPENDS) openCV

# Linking the executable from the object files
openCV: $(OBJECTS)
	$(CXX) $(WARNING) $(CXXFLAGS) $(INC_PATH) $^ -o $@ $(LIBS)

-include $(DEPENDS)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SOURCEDIR)/%.cpp Makefile | $(OBJDIR)
	$(CXX) $(WARNING) $(CXXFLAGS) $(INC_PATH) -MMD -MP -c $< -o $@
