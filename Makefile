# Directories
SRC = src
BUILD = build
INCLUDE = include
LIB = lib

ALL_INCLUDES = -I./$(LIB) -I$(GLAD_INC) -I./$(INCLUDE) -I./$(IMGUI_DIR) -I./$(IMGUI_DIR)/backends -I./$(FLECS) -I./$(STB_IMAGE) -I./$(GLM)
GLAD = ./$(LIB)/glad
GLAD_INC = $(GLAD)/include
IMGUI_DIR = $(LIB)/imgui
FLECS = $(LIB)/flecs
STB_IMAGE = $(LIB)/stb_image
GLM = $(LIB)/glm

IMGUI_SOURCES := $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp $(IMGUI_DIR)/imgui_stdlib.cpp $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp

# Compiler and flags
CC = gcc
CXX = g++
CFLAGS = -Wall -O2 $(ALL_INCLUDES)
# CFLAGS = -Wall -ggdb -O3 $(INCLUDES)
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -pedantic $(ALL_INCLUDES)
# CXXFLAGS = -Wall -ggdb -O3 $(INCLUDES)
LDFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lassimp

# SHARED OBJECTS AND TARGETS  (Targets are executables)

# Shared objects by multiple executables
CPP_FILES := Shader.cpp Camera.cpp stb_image.cpp Mesh.cpp Model.cpp SimpleGui.cpp
OBJECTS := $(CPP_FILES:.cpp=.o) glad.o flecs.o stb_image.o imgui_impl_glfw.o imgui_impl_opengl3.o imgui_demo.o imgui_draw.o imgui_tables.o imgui_widgets.o imgui_stdlib.o imgui.o
OBJECTS := $(addprefix $(BUILD)/, $(OBJECTS))

# Targets
CPP_EXEC := hello_triangle.cpp
TARGETS_OBJ := $(CPP_EXEC:%.cpp=$(BUILD)/%.o)
TARGETS := $(TARGETS_OBJ:%.o=%)

# RECIPES
all: $(TARGETS)

# executables depend on shared objects
$(TARGETS): $(OBJECTS) 

# Link
# Secondary expansions allow to use the automatic variable $@ in the prerequisites list.
# https://www.gnu.org/software/make/manual/html_node/Secondary-Expansion.html
.SECONDEXPANSION:
$(TARGETS): $(OBJECTS) $$@.o 
	$(CXX) -o $@ $^ $(LDFLAGS)

# Compile objects
# Order of recipes matter. Recipe 2 has to be before recipe 3 to take into account .h prerrequisites. 

# recipe 1: Complie glad
$(BUILD)/glad.o: $(GLAD)/src/glad.c $(GLAD)/include/glad/glad.h | $(BUILD)
	$(CC) -c $(CFLAGS) -o $@ $< 

# imgui:
$(BUILD)/%.o:$(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD)/%.o:$(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

#flecs:
$(BUILD)/flecs.o: $(FLECS)/flecs.c $(FLECS)/flecs.h | $(BUILD)
	$(CC) -c $(CFLAGS) -std=gnu99 -o $@ $<

#stb_image:
$(BUILD)/stb_image.o: $(STB_IMAGE)/stb_image.cpp $(STB_IMAGE)/stb_image.h | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# recipe 2: compile objects - cpp files with header files
$(BUILD)/%.o: $(SRC)/%.cpp $(INCLUDE)/%.h | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

# recipe 3: compile executables - cpp files without header files
$(BUILD)/%.o: $(SRC)/%.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@    

# PHONY
.PHONY : all clean run-test

clean :
	rm -rf $(BUILD)

run-test: $(TARGETS)
	./$(BUILD)/hello_triangle

$(BUILD):
	mkdir -p $(BUILD)