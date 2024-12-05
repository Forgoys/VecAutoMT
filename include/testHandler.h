#ifndef TESTHANDLER_H
#define TESTHANDLER_H

#include <clang/Basic/SourceManager.h>

#include "util.h"
#include "preprocessor_config.h"
#include "array_matcher.h"

    using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;
extern llvm::cl::opt<bool> LocateMode;
extern llvm::cl::opt<bool> RestoreMode;
extern llvm::cl::opt<bool> ModifyMode;
extern json outputJson;
namespace testHandler{

// 用于处理匹配到的节点
class ForLoopHandler : public MatchFinder::MatchCallback
{
public:
    ForLoopHandler()
    {
    }

    virtual void run(const MatchFinder::MatchResult &Result)
    {
        // 从匹配结果中取出名为 "forLoop" 的 AST 节点
        if (const ForStmt *FS = Result.Nodes.getNodeAs<ForStmt>("forLoop")) {
            /*
            - SourceManager 是 Clang 的源码管理器，用于处理源码文件的位置和内容。
            - 通过它可以获取源码的具体位置（如行号、列号）以及源码文本。
            */
            SourceManager &SM = *Result.SourceManager;

            // 获取循环的源码
            SourceLocation StartLoc = FS->getBeginLoc(); // 获取 for 循环的起始位置（包括 for 关键字）
            SourceLocation EndLoc = FS->getEndLoc(); // 获取 for 循环的结束位置（通常是右大括号 }）

            // 调整 EndLoc，包含整个循环
            /*
            基于 SourceManager 和语言选项（LangOptions），找到给定位置 EndLoc 所对应的标记的真正结束位置。
            对于右大括号 }，它会返回 } 之后的下一个位置
            用 FS->getEndLoc() 提取代码范围，可能无法获取完整的循环代码，因为右大括号后可能还有换行符或注释未被包含
            */
            EndLoc = Lexer::getLocForEndOfToken(EndLoc, 0, SM, LangOptions());

            // CharSourceRange 表示一段源码的范围（从起点到终点）。
            CharSourceRange CharRange = CharSourceRange::getCharRange(StartLoc, EndLoc);
            // 提取字符范围内的源码文本
            std::string LoopText = Lexer::getSourceText(CharRange, SM, LangOptions()).str();

            // 输出循环的位置和源码
            llvm::outs() << "Found a for loop at "
                    << SM.getFilename(StartLoc) << ":"
                    << SM.getSpellingLineNumber(StartLoc) << "\n";
            llvm::outs() << LoopText << "\n\n";

            // 示例：在循环前插入注释
            // Rewrite.InsertText(StartLoc, "// This is a for loop\n", true, true);
        }
    }

private:

};

// 自定义的 ASTConsumer
/*
自定义的 MyASTConsumer 类是 Clang 的一个 AST 消费者，其职责是在编译单元（即整个源文件）完成 AST 树构建后，
调用 HandleTranslationUnit() 方法。
*/
class MyASTConsumer : public ASTConsumer
{
public:
    MyASTConsumer(json &outputJson) : handlerForForLoop(), matcher() {
        auto forStmtMatcher = forStmt(
            hasLoopInit(declStmt(hasSingleDecl(varDecl().bind("loopVar")))),
            hasBody(hasDescendant(arraySubscriptExpr().bind("arrayAccess"))),
            hasAncestor(functionDecl().bind("containingFunction")))
        .bind("forLoop");

        matcher.addMatcher(forStmtMatcher, new ArrayMatcher(outputJson));  // 使用定义的matcher
    }

    /*
    - 遍历整个 AST 树，并根据之前注册的匹配规则（如 forStmt()）查找符合条件的节点。
    - 对每个匹配到的节点调用 HandlerForForLoop.run() 进行处理。
    */
    void HandleTranslationUnit(ASTContext &Context) override
    {
        matcher.matchAST(Context);
    }

private:
    ForLoopHandler handlerForForLoop;
    MatchFinder matcher;
};

// 自定义的 FrontendAction
class MyFrontendAction : public ASTFrontendAction
{
public:
    MyFrontendAction()
    {
    }

    // 在 Clang 完成当前源文件的处理时调用，用于收尾操作，例如打印处理的文件名或输出修改后的代码
    void EndSourceFileAction() override
    {
        SourceManager &SM = getCompilerInstance().getSourceManager();
        // SourceManager::getFileEntryForID(FileID)返回指向 FileEntry 的指针，FileEntry 包含文件的元信息（如文件路径）
        llvm::errs() << "** Finished processing: "
                << SM.getFileEntryForID(SM.getMainFileID())->getName() << "\n";
    }

    // 在 Clang 开始处理源文件时调用，用于创建一个自定义的 ASTConsumer 实例（MyASTConsumer），并将 Rewriter 传递给它
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file) override
    {
        /*
        获取当前编译器实例的 SourceManager, 获取当前编译器实例的语言选项（LangOptions）
        将 SourceManager 和语言选项设置到 Rewriter 中
        */
        // 创建一个智能指针（std::unique_ptr），这里用于创建 MyASTConsumer 实例
        if (!getModifyMode()) {
            // 只在非修改模式下应用预处理配置
            PreprocessorConfig::configure(CI);
            llvm::outs() << "preprocess config done.\n";
        }

        // 创建一个智能指针（std::unique_ptr），这里用于创建 MyASTConsumer 实例
        return std::make_unique<MyASTConsumer>(outputJson);
    }

private:

};
}

#endif //TESTHANDLER_H
