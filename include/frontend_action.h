#ifndef FRONTEND_ACTION_H
#define FRONTEND_ACTION_H

#include "util.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "command_line_options.h"
#include "json_manager.h"
#include "diagnostic_tool.h"

// 外部声明的重写器
extern clang::Rewriter TheRewriter;

// 自定义前端Action类
class MyFrontendAction : public clang::ASTFrontendAction
{
protected:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
        clang::CompilerInstance &CI,
        llvm::StringRef InFile) override;

    bool BeginSourceFileAction(clang::CompilerInstance &CI) override;
};

class MyASTConsumer : public clang::ASTConsumer
{
private:
    clang::ast_matchers::MatchFinder matchFinder;

public:
    MyASTConsumer();

    void HandleTranslationUnit(clang::ASTContext &Context) override;
};

// 创建自定义前端Action工厂的辅助函数
std::unique_ptr<clang::tooling::FrontendActionFactory> createCustomFrontendActionFactory(
    MyDiagnosticConsumer *DiagConsumer);

#endif
