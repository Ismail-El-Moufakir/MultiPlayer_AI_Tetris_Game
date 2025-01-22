# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Project structure
TARGET = sfml-app
SRC_DIRS = ./Windows ./Buttons
SRC_FILES = $(wildcard $(addsuffix /*.cpp, $(SRC_DIRS)))
OBJ_FILES = $(SRC_FILES:.cpp=.o)

# Default target
all: $(TARGET)

# Build the target
$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(SFML_LIBS)

# Build object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean the build
clean:
	rm -f $(OBJ_FILES) $(TARGET)

# Rebuild the project
rebuild: clean all

# PHONY targets
.PHONY: all clean rebuild