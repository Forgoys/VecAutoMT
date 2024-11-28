# VecAutoMT - Auto Vectorization Tool for MT-3000

VecAutoMT 是一个基于 LLVM/Clang 的代码分析工具，用于分析和处理 C/C++ 代码中的数组访问模式。该工具主要用于识别和处理可能适合向量化优化的代码片段。

## 功能特性

该工具提供三种主要工作模式：

1. **定位模式 (Locate)**
   - 识别代码中的数组访问模式
   - 分析 for 循环中的数组索引使用
   - 输出详细的位置信息（文件、函数、行号）
   - 生成结构化的 JSON 分析报告

2. **恢复模式 (Restore)**
   - 基于之前生成的 JSON 分析报告
   - 重新定位代码中的关键位置
   - 支持代码修改后的重新分析

3. **修改模式 (Modify)**
   - 自动添加代码优化提示
   - 插入边界检查
   - 添加向量化和并行化建议

## 前置要求

- LLVM/Clang (推荐使用 14.0.0 或更高版本)
- C++14 或更高版本的编译器
- Make 构建工具

## 安装

1. 克隆仓库：
```bash
git clone https://github.com/Forgoys/VecAutoMT
cd VecAutoMT
```

2. 配置环境：
编辑 `make.conf` 文件，设置 LLVM 安装路径：
```makefile
LLVM_PATH = /path/to/your/llvm
```

3. 编译工具：
```bash
make
```

## 使用方法

### 基本用法

```bash
./bin/VecAutoMT [options] <source-file> -- [compiler-options]
```

### 命令行选项

- `-locate`: 启用定位模式
- `-restore`: 启用恢复模式
- `-modify`: 启用修改模式
- `-output <file>`: 指定输出文件
- `-input <file>`: 指定输入 JSON 文件（恢复模式需要）

### 使用示例

1. 定位模式分析：
```bash
./bin/VecAutoMT -locate -output analysis.json test.c -- -I/path/to/includes
```

2. 基于分析结果恢复：
```bash
./bin/VecAutoMT -restore -input analysis.json test.c -- -I/path/to/includes
```

3. 添加优化提示：
```bash
./bin/VecAutoMT -modify -output modified.c test.c -- -I/path/to/includes
```

## 输出格式

### JSON 输出格式（定位模式）

```json
[
  {
    "filename": "test.c",
    "function": "example_function",
    "forLoop": {
      "startLine": 10,
      "endLine": 15,
      "sourceText": "for(int i = 0; i < n; i++)"
    },
    "arrays": [
      {
        "name": "array",
        "indexVar": "i",
        "line": 12,
        "sourceText": "array[i] = value"
      }
    ]
  }
]
```

## 支持的分析模式

工具当前支持以下分析模式：

1. 数组访问模式：
   - 简单的循环索引访问
   - 带步长的数组访问
   - 多维数组访问

2. 循环特征：
   - 单变量自增循环
   - 边界检查识别
   - 循环步长分析

## 限制和注意事项

- 目前仅支持 C/C++ 源代码分析
- 分析结果依赖于代码的具体形式
- 某些复杂的数组访问模式可能无法被正确识别
- 修改模式下的代码变更需要人工审查
