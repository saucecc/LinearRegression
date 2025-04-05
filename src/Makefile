# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -I./src/lib/include
LDFLAGS = 

# Directories
SRC_DIR = src
LIB_DIR = src/lib
BUILD_DIR = build
BIN_DIR = bin

# Files
MAIN_SRC = main.cpp
LIB_SRCS = $(wildcard $(LIB_DIR)/*.cpp)
LIB_OBJS = $(patsubst $(LIB_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(LIB_SRCS))
MAIN_OBJ = $(BUILD_DIR)/main.o

# Executable
TARGET = $(BIN_DIR)/main

# Default target
all: $(TARGET)

# Link main and library objects into executable
$(TARGET): $(MAIN_OBJ) $(LIB_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Compile main source
$(BUILD_DIR)/main.o: $(MAIN_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile library sources
$(BUILD_DIR)/%.o: $(LIB_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean