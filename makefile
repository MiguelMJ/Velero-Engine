# General
EXEC = main
CXX = g++
CXXFLAGS = -O3 -std=c++11 -Wpedantic -fmax-errors=3
CC = $(CXX)
CFLAGS = $(CXXFLAGS)
INCLUDES = -Iinclude -Idev
CPPFLAGS = $(INCLUDES)
LINKDIRS =
LDFLAGS = 
LIBS =
SRC_FILES = \
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
		
		
SRC_TARGET = main.cpp
OBJ_TARGET = main.o
			
OBJ += $(SRC_FILES:%.cpp=%.o)

# FMT
INCLUDES += -I${HOME}/src/fmt-6.2.1/include
LINKDIRS += -L${HOME}/src/fmt-6.2.1/build
LIBS += -lfmt
CFLAGS += -DLOGURU_USE_FMTLIB=1

# CPPFS
INCLUDES += -I${HOME}/src/cppfs/source/cppfs/include/\
			-I${HOME}/src/cppfs/build/source/cppfs/include/
LINKDIRS += -L${HOME}/src/cppfs/build
LDFLAGS += -Wl,-rpath=${HOME}/src/cppfs/build/
LIBS += -lcppfs

# LOGURU
OBJ += ${HOME}/src/loguru-2.1.0/loguru.o
INCLUDES += -I${HOME}/src/loguru-2.1.0/
LIBS += -lpthread -ldl
CFLAGS += -DLOGURU_THREADNAME_WIDTH=0 -DLOGURU_FILENAME_WIDTH=0

# SFML
LIBS += -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system


$(EXEC): $(OBJ) $(OBJ_TARGET)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LINKDIRS) $^ -o $(EXEC) $(LIBS) $(LDFLAGS) 
	@echo Build succesfull
all: $(EXEC)
	
clean:
	rm -f *.o src/*.o dev/.*o

%.test: $(OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LINKDIRS) $^ $*.cpp -o $@ $(LIBS) $(LDFLAGS)
