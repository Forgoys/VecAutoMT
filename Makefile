# Makefile

include make.conf

# 获取CPU核心数，用于并行编译
NPROCS = $(shell nproc || sysctl -n hw.ncpu || echo 2)
MAKEFLAGS += -j$(NPROCS)

HTHREADS_INCLUDE_PATH:=${HTHREADS_ROOT_PATH}/include
DEV_CC_INCLUDE_PATH:=${DEV_CC_ROOT_PATH}/include

DEV_INCLUDE_PATH:=${DEV_CC_INCLUDE_PATH} ${HTHREADS_INCLUDE_PATH}
DEV_INCLUDE_FLAGS:=$(foreach i, ${DEV_INCLUDE_PATH}, -I${i})

# 设置本地 LLVM 和 Clang 的路径
LLVM_HOME := /opt/homebrew/opt/llvm

# 使用本地的 clang++
CLANG := $(LLVM_HOME)/bin/clang++
CLANG_FLAGS := -std=c++14 -fno-color-diagnostics -fno-rtti

# 使用本地的 llvm-config
LLVM_CONFIG := $(LLVM_HOME)/bin/llvm-config
LLVM_CXXFLAGS := $(shell $(LLVM_CONFIG) --cxxflags)
LLVM_LDFLAGS := $(shell $(LLVM_CONFIG) --link-shared --ldflags)
LLVM_LIBS := $(shell $(LLVM_CONFIG) --link-shared --libs)
LLVM_SYSTEM_LIBS := $(shell $(LLVM_CONFIG) --system-libs)
LLVM_INCLUDEDIR := $(shell $(LLVM_CONFIG) --includedir)

# Clang 的包含目录
CLANG_INCLUDEDIR := $(LLVM_HOME)/include

# 使用 libclang-cpp 库
CLANG_LIBS := -lclang-cpp

# 合并所有选项
TOOL_CLANG_FLAGS := $(CLANG_FLAGS) $(LLVM_CXXFLAGS) -I$(CLANG_INCLUDEDIR) -Iinclude
TOOL_LINK_FLAGS := -L$(LLVM_HOME)/lib -Wl,-rpath,$(LLVM_HOME)/lib $(LLVM_LDFLAGS) $(LLVM_LIBS) $(CLANG_LIBS) $(LLVM_SYSTEM_LIBS)

# 在运行时指定资源目录
ARGS := -resource-dir=$(LLVM_HOME)/lib/clang/18

# 源文件和目标文件
SRC_DIR := src
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(SRCS:%.cpp=%.o)

# 输入输出文件路径
TEST_DIR := test
SRC_FILE := $(TEST_DIR)/sample.c
LOCATE_OUTPUT := $(TEST_DIR)/locate_output.json
RESTORE_OUTPUT := $(TEST_DIR)/restore_output.json
MODIFY_OUTPUT := $(TEST_DIR)/modified.c

# Run mode and arguments
.PHONY: all build run run-locate run-restore run-modify clean

all: build

build: bin/VecAutoMT

bin/VecAutoMT: $(OBJS)
	@mkdir -p bin
	$(CLANG) $(TOOL_CLANG_FLAGS) -o $@ $^ $(TOOL_LINK_FLAGS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CLANG) $(TOOL_CLANG_FLAGS) -c $< -o $@

# 将run目标分解为具体的运行模式
run-locate: build
	@mkdir -p $(TEST_DIR)
	export LD_LIBRARY_PATH=$(LLVM_HOME)/lib:$$LD_LIBRARY_PATH; \
	bin/VecAutoMT -locate $(SRC_FILE) -output $(LOCATE_OUTPUT) -- $(DEV_INCLUDE_FLAGS) $(ARGS)

run-restore: build
	@mkdir -p $(TEST_DIR)
	export LD_LIBRARY_PATH=$(LLVM_HOME)/lib:$$LD_LIBRARY_PATH; \
	bin/VecAutoMT -restore $(SRC_FILE) -input $(LOCATE_OUTPUT) -output $(RESTORE_OUTPUT) -- $(DEV_INCLUDE_FLAGS) $(ARGS)

run-modify: build
	@mkdir -p $(TEST_DIR)
	export LD_LIBRARY_PATH=$(LLVM_HOME)/lib:$$LD_LIBRARY_PATH; \
	bin/VecAutoMT -modify $(SRC_FILE) -input $(RESTORE_OUTPUT) -output $(MODIFY_OUTPUT) -- $(DEV_INCLUDE_FLAGS) $(ARGS)

# 使用run目标来调用具体的运行模式
run:
	@if [ "$(filter locate,$(MAKECMDGOALS))" ]; then \
		$(MAKE) run-locate; \
	elif [ "$(filter restore,$(MAKECMDGOALS))" ]; then \
		$(MAKE) run-restore; \
	elif [ "$(filter modify,$(MAKECMDGOALS))" ]; then \
		$(MAKE) run-modify; \
	else \
		echo "Please specify a mode: make run locate|restore|modify"; \
		exit 1; \
	fi

# Additional targets to allow 'make run MODE'
locate restore modify:
	@:

clean:
	rm -rf bin
	rm -f $(OBJS)
	rm -f $(LOCATE_OUTPUT) $(RESTORE_OUTPUT) $(MODIFY_OUTPUT)