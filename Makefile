# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network
INCLUDE_DIR = ./include

# Project structure
TARGET = sfml-app
SRC_FILES = Env.cpp Home.cpp Network.cpp main.cpp ./Buttons/RectButton.cpp ./Buttons/Button.cpp
OBJ_FILES = $(SRC_FILES:.cpp=.o)

# Default target
all: $(TARGET)

# Build the target
$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $^ -o $@ $(SFML_LIBS)

# Build object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Clean the build
clean:
	rm -f $(OBJ_FILES) $(TARGET)

# Rebuild the project
rebuild: clean all

# PHONY targets
.PHONY: all clean rebuild
