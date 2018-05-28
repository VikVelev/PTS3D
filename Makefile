TARGET_EXEC ?= pts3d
BUILD_DIR ?= build
SRC_DIRS ?= src

SRCS := $(shell find $(SRC_DIRS) -name *.cpp)
CXX := clang
CXXFLAGS := 

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP -lstdc++ -lm

pts3d: $(SRCS)
	@echo "Building..."
	$(CXX) src/main/main.cpp -o $(BUILD_DIR)/$(TARGET_EXEC) $(CPPFLAGS)
	@chmod +X $(BUILD_DIR)/$(TARGET_EXEC)
	@echo "Cleaning up..."	
	$(RM) $(BUILD_DIR)/$(TARGET_EXEC).d

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)
