# Makefile

include make.conf

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

# 源文件
SRCS := src/main.cpp src/array_matcher.cpp src/code_modifier.cpp src/preprocessor_config.cpp
OBJS := $(SRCS:.cpp=.o)

# 编译目标
.PHONY: all build run clean

all: build

build: bin/VecAutoMT

bin/VecAutoMT: $(OBJS)
	mkdir -p bin
	$(CLANG) $(TOOL_CLANG_FLAGS) -o $@ $^ $(TOOL_LINK_FLAGS)

%.o: %.cpp
	$(CLANG) $(TOOL_CLANG_FLAGS) -c -o $@ $<

run:
	export LD_LIBRARY_PATH=$(LLVM_HOME)/lib:$$LD_LIBRARY_PATH; \
	bin/VecAutoMT -locate test/sample.c -- $(DEV_INCLUDE_FLAGS) $(ARGS)

clean:
	rm -rf bin
	rm -f $(OBJS)