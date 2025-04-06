# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -I./src/cpp/include
LDFLAGS = 

# Directories
SRC_DIR = src
CPP_DIR = $(SRC_DIR)/cpp
BUILD_DIR = build
BIN_DIR = bin

# Files
SRCS = $(wildcard $(CPP_DIR)/*.cpp)
OBJS = $(patsubst $(CPP_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Executable
TARGET = $(BIN_DIR)/main

# Default target
all: $(TARGET)

# Link object files into executable
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Compile source files into object files
$(BUILD_DIR)/%.o: $(CPP_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean
