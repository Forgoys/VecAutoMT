#ifndef PREPROCESSOR_CONFIG_H
#define PREPROCESSOR_CONFIG_H

#include "util.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

// 声明外部命令行选项的引用
extern llvm::cl::opt<bool> &getLocateMode();
extern llvm::cl::opt<bool> &getRestoreMode();
extern llvm::cl::opt<bool> &getModifyMode();

// 预处理器配置类
class PreprocessorConfig
{
public:
    static void configure(clang::CompilerInstance &CI);
};

#endif // PREPROCESSOR_CONFIG_H