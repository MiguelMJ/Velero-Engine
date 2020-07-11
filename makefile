#
# General
#
EXEC = main
CXX = g++
CXXFLAGS = -std=c++11 -fmax-errors=3 $(INCLUDES) $(LINKDIRS)
CC = $(CXX)
CFLAGS = $(CXXFLAGS)

INCLUDES = -Iinclude -Idev
LINKDIRS =
LDFLAGS = 
LIBS =
LINKAGE = $(LIBS) $(LDFLAGS)
TARGET = main
SRC_FILES = $(TARGET).cpp\
		src/RenderSystem.cpp\
		src/DynamicLayer.cpp\
		src/LightingLayer.cpp\
		src/StaticLayer.cpp\
		src/LightSource.cpp\
		src/geutil.cpp\
		src/Random.cpp\
		src/EventSystem.cpp\
		src/AssetSystem.cpp\
		\
		dev/Sprite.cpp\
		dev/EntityComponent.cpp\


			
OBJ += $(SRC_FILES:%.cpp=%.o)

#
# Debug build settings
#
DBGDIR = debug
DBGEXEC = $(DBGDIR)/$(EXEC)
DBGOBJ = $(addprefix $(DBGDIR)/, $(OBJ))
DBGCFLAGS = -g -O0 -DDEBUG

#
# Release build settings
#
RELDIR = release
RELEXEC = $(RELDIR)/$(EXEC)
RELOBJ = $(addprefix $(RELDIR)/, $(OBJ))
RELCFLAGS = -O3 -DNDEBUG

#
# Libraries
#

# FMT
INCLUDES += -I${HOME}/src/fmt-6.2.1/include
LINKDIRS += -L${HOME}/src/fmt-6.2.1/build
LIBS += -lfmt
CXXFLAGS += -DLOGURU_USE_FMTLIB=1

# CPPFS
INCLUDES += -I${HOME}/src/cppfs/source/cppfs/include/\
			-I${HOME}/src/cppfs/build/source/cppfs/include/
LINKDIRS += -L${HOME}/src/cppfs/build
LDFLAGS += -Wl,-rpath=${HOME}/src/cppfs/build/
LIBS += -lcppfs

# LOGURU
# OBJ += ${HOME}/src/loguru-2.1.0/loguru.o # loguru.cpp included in geutils.cpp
INCLUDES += -I${HOME}/src/loguru-2.1.0/
LIBS += -lpthread -ldl
CXXFLAGS += -DLOGURU_THREADNAME_WIDTH=0 -DLOGURU_FILENAME_WIDTH=0

# SFML
LIBS += -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system


#
# Rules
#
.PHONY: all clean debug prep release remake

# Default build
all: prep release

#
# Debug rules
#
debug: $(DBGEXEC)

$(DBGEXEC): $(DBGOBJ)
	$(CXX) $(CXXFLAGS) $(DBGCFLAGS) $^ -o $(DBGEXEC) $(LINKAGE)

$(DBGDIR)/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(DBGCFLAGS) -o $@ $<

#
# Release rules
#
release: $(RELEXEC)

$(RELEXEC): $(RELOBJ)
	$(CXX) $(CXXFLAGS) $(RELCFLAGS) $^ -o $(RELEXEC) $(LINKAGE)

$(RELDIR)/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(RELCFLAGS) -o $@ $<
	
	
$(EXEC): $(OBJ) $(OBJ_TARGET)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LINKDIRS) $^ -o $(EXEC) $(LIBS) $(LDFLAGS) 
	@echo Build succesfull
	
#
# Other rules
#
prep:
	@mkdir -p $(DBGDIR)/src
	@mkdir -p $(DBGDIR)/dev
	@mkdir -p $(RELDIR)/src
	@mkdir -p $(RELDIR)/dev

remake: clean all

clean:
	rm -f $(RELEXEC) $(RELOBJ) $(DBGEXE) $(DBGOBJ)
	
	
clean-test:
	rm -f tests/*.test
%.test: $(OBJ) clean-test
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LINKDIRS) $(OBJ) $*.cpp -o $@ $(LIBS) $(LDFLAGS)
