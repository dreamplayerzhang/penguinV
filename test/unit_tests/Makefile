
PWD := $(shell pwd)
LIB_DIR := $(PWD)/../../src
SRCS :=  \
	$(LIB_DIR)/FileOperation/bitmap.cpp \
	$(LIB_DIR)/penguinv/penguinv.cpp \
	$(LIB_DIR)/blob_detection.cpp \
	$(LIB_DIR)/function_pool.cpp \
	$(LIB_DIR)/image_function.cpp \
	$(LIB_DIR)/image_function_helper.cpp \
	$(LIB_DIR)/image_function_avx.cpp \
	$(LIB_DIR)/image_function_neon.cpp \
	$(LIB_DIR)/image_function_sse.cpp \
	$(LIB_DIR)/thread_pool.cpp \
	unit_tests.cpp \
	unit_test_bitmap.cpp \
	unit_test_blob_detection.cpp \
	unit_test_framework.cpp \
	unit_test_helper.cpp \
	unit_test_image_buffer.cpp \
	unit_test_image_function.cpp
TARGET := unit_tests

CXX := g++
LINKER := g++
INCDIRS := -I$(PWD) -I$(LIB_DIR)
LIBS := -pthread
CXXFLAGS := -std=c++11 -Wall -Wextra -Wstrict-aliasing -Wpedantic -Wconversion -O2 -march=native
BUILD_DIR=build
BIN := $(BUILD_DIR)/bin
OBJS := $(patsubst %.cpp,%.o,$(SRCS))
OBJFILES := $(addprefix $(BIN)/, $(foreach obj, $(OBJS), $(shell basename $(obj))))

all: $(BIN) $(TARGET)

$(BIN):
	@mkdir -p $(BIN)

$(TARGET): $(OBJFILES)
	$(LINKER) $^ -o $@ $(LIBS)

$(BIN)/%.o: ./%.cpp
	$(CXX) $(CXXFLAGS) $(INCDIRS) -c $< -o $@

$(BIN)/%.o: ./**/%.cpp
	$(CXX) $(CXXFLAGS) $(INCDIRS) -c $< -o $@

$(BIN)/%.o: $(LIB_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCDIRS) -c $< -o $@

$(BIN)/%.o: $(LIB_DIR)/**/%.cpp
	$(CXX) $(CXXFLAGS) $(INCDIRS) -c $< -o $@

run: $(TARGET)
	@echo "Running unit tests ..."
	@./$(TARGET)
clean:
	@rm -rf $(BUILD_DIR)
