#ifndef FRONTEND_ACTION_H
#define FRONTEND_ACTION_H

#include <array_matcher.h>

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

extern json inputJson;
extern json outputJson;
extern clang::Rewriter TheRewriter;

// 自定义前端Action类
class MyFrontendAction : public clang::ASTFrontendAction
{
protected:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) override;

    // 新的初始化方法
    bool BeginSourceFileAction(clang::CompilerInstance &CI) override;
};

class MyASTConsumer : public clang::ASTConsumer
{
private:
    MatchFinder matchFinder;
public:
    MyASTConsumer() = default;
    MyASTConsumer(json &inputJson, json &outputJson);
    void HandleTranslationUnit(clang::ASTContext &Context) override;
};

// 创建自定义前端Action工厂的辅助函数
std::unique_ptr<clang::tooling::FrontendActionFactory> createCustomFrontendActionFactory();

#endif