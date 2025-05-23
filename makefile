# sfml-app: main.o
# 	g++ main.o -o sfml-app -L/home/nhlanhla/Documents/AudioSeedGenerator/lib -lsfml-graphics -lsfml-window -lsfml-system
#
# main.o: main.cpp
# 	g++ -c -o main.o main.cpp -I/home/nhlanhla/Documents/AudioSeedGenerator/include

# Compiler

EXE = main
CXX := g++

# Include directories
INCL_DIR = ./include
IMPLOT_DIR = ./imgui/implot
INCLUDES := -I$(INCL_DIR) gl.c

SOURCES = main.cpp
SOURCES += $(INCL_DIR)/imgui_demo.cpp $(INCL_DIR)/imgui.cpp $(INCL_DIR)/imgui_draw.cpp $(INCL_DIR)/imgui_tables.cpp $(INCL_DIR)/imgui_widgets.cpp
SOURCES += $(INCL_DIR)/imgui_impl_glfw.cpp $(INCL_DIR)/imgui_impl_opengl3.cpp $(INCL_DIR)/UseImGui.cpp
SOURCES += $(IMPLOT_DIR)/implot.cpp $(IMPLOT_DIR)/implot_items.cpp $(IMPLOT_DIR)/implot_demo.cpp


# Libraries we need to link
LIB_DIR = ./lib
LIBS := -L$(LIB_DIR) -lGL -lglfw -lsfml-window -lsfml-graphics -lsfml-system -lsfml-audio
DebugFlag = -g

# HACK: Dont forget to export LD_LIBRARY_PATH=./lib before running exe becuase dynamic linker dont know where to find SFML libraries

main: main.cpp
	g++ $(SOURCES) -o $(EXE) $(INCLUDES) $(LIBS) $(DebugFlag)
debug: main
	@curl http://127.0.0.1:5550/api/runtime/run
