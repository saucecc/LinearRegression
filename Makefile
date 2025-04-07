# # Compiler and flags
# CXX = g++
# CXXFLAGS = -Wall -Wextra -I./src/cpp/include
# LDFLAGS = 

# # Directories
# SRC_DIR = src
# CPP_DIR = $(SRC_DIR)/cpp
# BUILD_DIR = build
# BIN_DIR = bin

# # Files
# SRCS = $(wildcard $(CPP_DIR)/*.cpp)
# HDRS = $(wildcard $(CPP_DIR)/*.h)
# OBJS = $(patsubst $(CPP_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# # Executable
# TARGET = $(BIN_DIR)/main $(BIN_DIR)/matrixtest

# # Default target
# all: $(TARGET)

# # Link object files into executable
# $(TARGET): $(OBJS)
# 	@mkdir -p $(BIN_DIR)
# 	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# # Compile source files into object files
# $(BUILD_DIR)/%.o: $(CPP_DIR)/%.cpp
# 	@mkdir -p $(BUILD_DIR)
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# main: main.cpp $(SRCS) $(HDRS)
# 	$(CXX) $(CXXFLAGS) -o main main.cpp $(SRCS)

# matrixtest: matrixtest.cpp $(SRCS) $(HDRS)
# 	$(CXX) $(CXXFLAGS) -o matrixtest matrixtest.cpp $(SRCS)

# # Clean build artifacts
# clean:
# 	rm -rf $(BUILD_DIR) $(BIN_DIR)

# .PHONY: all clean

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -I./src/cpp/include
LDFLAGS =

# Directories
SRC_DIR = src
CPP_DIR = $(SRC_DIR)/cpp
BUILD_DIR = build
BIN_DIR = bin

# Shared sources (everything except files with main functions)
SHARED_SRCS = $(filter-out $(CPP_DIR)/main.cpp $(CPP_DIR)/test.cpp, $(wildcard $(CPP_DIR)/*.cpp))
SHARED_OBJS = $(patsubst $(CPP_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SHARED_SRCS))

# Individual executables
MAIN_BIN = $(BIN_DIR)/main
TEST_BIN = $(BIN_DIR)/test

# Default target
all: $(MAIN_BIN) $(TEST_BIN)

# main executable
$(MAIN_BIN): $(BUILD_DIR)/main.o $(SHARED_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# matrixtest executable
$(TEST_BIN): $(BUILD_DIR)/test.o $(SHARED_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Compile all .cpp files to .o files
$(BUILD_DIR)/%.o: $(CPP_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean
