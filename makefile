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
SRC_FILES = $(EXEC).cpp\
		src/RenderSystem.cpp\
		src/DynamicLayer.cpp\
		src/LightingLayer.cpp\
		src/StaticLayer.cpp\
		src/LightSource.cpp\
		src/geutil.cpp\
		src/Random.cpp\
		src/EventSystem.cpp\
		src/AssetSystem.cpp\
		src/EntityComponent.cpp\
		\
		

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
# Custom output functions
#
define print_info
	@echo "\033[1;38;2;250;250;50m$(1)\033[0m"
endef
define print_success
	@echo "\033[1;38;2;50;250;50m$(1)\033[0m"
endef

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
	$(call print_success,$< ready)
	
$(DBGEXEC): $(DBGOBJ)
	$(call print_info,Building $@)
	$(CXX) $(CXXFLAGS) $(DBGCFLAGS) $^ -o $(DBGEXEC) $(LINKAGE)

$(DBGDIR)/%.o: %.cpp
	$(call print_info,Building $@)
	$(CXX) -c $(CXXFLAGS) $(DBGCFLAGS) -o $@ $<

#
# Release rules
#
release: $(RELEXEC)
	$(call print_success,$< ready)
	
$(RELEXEC): $(RELOBJ)
	$(call print_info,Building $@)
	$(CXX) $(CXXFLAGS) $(RELCFLAGS) $^ -o $(RELEXEC) $(LINKAGE)

$(RELDIR)/%.o: %.cpp
	$(call print_info,Building $@)
	$(CXX) -c $(CXXFLAGS) $(RELCFLAGS) -o $@ $<
	
#
# Other rules
#
prep:
	@mkdir -p $(DBGDIR)/src
	@mkdir -p $(DBGDIR)/dev
	@mkdir -p $(DBGDIR)/tests
	@mkdir -p $(RELDIR)/src
	@mkdir -p $(RELDIR)/dev
	@mkdir -p $(RELDIR)/tests

remake: clean all

clean:
	rm -f -r $(RELDIR) $(DBGDIR)
	