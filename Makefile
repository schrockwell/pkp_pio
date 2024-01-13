# Define compiler and flags
CXX = g++
CXXFLAGS = -Wall -g -I include

# Define the source files and the target
SRC = test/main.cpp src/pkp.cpp
OBJ = $(SRC:.cpp=.o)
BUILD_DIR = build
TARGET = test_runner

# Default target
all: $(TARGET)

# Rule to create the target executable
$(TARGET): $(OBJ)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/$(TARGET) $(OBJ)

# Rule to compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to run the tests
test: $(TARGET)
	./$(BUILD_DIR)/$(TARGET)

# Rule to clean up
clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all test clean
