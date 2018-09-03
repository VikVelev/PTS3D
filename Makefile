TARGET_EXEC ?= pts3d
BUILD_DIR ?= build
SRC_DIRS ?= src

SRCS := $(shell find $(SRC_DIRS) -name *.cpp)
CXX := g++
CXXFLAGS := 

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP -lstdc++ -lm `pkg-config --libs --cflags sdl2` -fopenmp -std=c++17

pts3d: $(SRCS)
	@echo "Building..."
	$(CXX) src/main/main.cpp -o $(BUILD_DIR)/$(TARGET_EXEC) $(CPPFLAGS)
	@chmod +X $(BUILD_DIR)/$(TARGET_EXEC)
	@echo "Cleaning up..."	
	$(RM) $(BUILD_DIR)/$(TARGET_EXEC).d

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

debug: $(SRCS)
	@echo "Building Debug..."
	$(CXX) src/main/main.cpp -o $(BUILD_DIR)/$(TARGET_EXEC) $(CPPFLAGS) -g
	@chmod +X $(BUILD_DIR)/$(TARGET_EXEC)
	@echo "Cleaning up..."	
	$(RM) $(BUILD_DIR)/$(TARGET_EXEC).d
