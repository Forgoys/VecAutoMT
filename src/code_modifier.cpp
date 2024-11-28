#include "code_modifier.h"

void CodeModifier::run(const clang::ast_matchers::MatchFinder::MatchResult &Result)
{
    const auto *arrayExpr = Result.Nodes.getNodeAs<clang::ArraySubscriptExpr>("arrayAccess");
    if (!arrayExpr)
        return;

    // 获取原始代码的属性和宏
    clang::SourceManager &SM = Result.Context->getSourceManager();
    const clang::LangOptions &LangOpts = Result.Context->getLangOpts();

    // 在插入新代码时，保持原有的属性和宏定义
    clang::SourceLocation loc = arrayExpr->getBeginLoc();

    // 获取完整的源代码，包括属性
    clang::CharSourceRange range = clang::CharSourceRange::getCharRange(
        arrayExpr->getBeginLoc(),
        arrayExpr->getEndLoc());
    std::string originalCode = clang::Lexer::getSourceText(range, SM, LangOpts).str();

    // 修改代码时保持原有属性
    rewrite.InsertText(loc, "/* Array access modified */\n" + originalCode, true);
}

void ExtendedCodeModifier::addBoundsCheck(
    const clang::ArraySubscriptExpr *Array, const clang::ForStmt *Loop)
{
    clang::SourceLocation loc = Array->getBeginLoc();

    // 获取数组名和索引变量名
    std::string arrayName;
    if (const auto *baseExpr = llvm::dyn_cast<clang::DeclRefExpr>(Array->getBase()->IgnoreParenImpCasts()))
    {
        arrayName = baseExpr->getNameInfo().getAsString();
    }

    std::string indexName;
    if (const auto *idx = llvm::dyn_cast<clang::DeclRefExpr>(Array->getIdx()->IgnoreParenImpCasts()))
    {
        indexName = idx->getNameInfo().getAsString();
    }

    // 生成适当的边界检查代码
    std::string check = "/* 边界检查 */\n";
    check += "if (" + indexName + " >= sizeof(" + arrayName + ")/sizeof(" +
             arrayName + "[0])) {\n";
    check += "    /* 根据函数返回类型处理错误情况 */\n";
    check += "    errno = ERANGE;\n";
    check += "    return;\n";
    check += "}\n";

    rewrite.InsertText(loc, check, true);
}

void ExtendedCodeModifier::addOptimizationHint(const clang::ArraySubscriptExpr *Array)
{
    clang::SourceLocation loc = Array->getBeginLoc();
    std::string hint = "/* 考虑使用SIMD指令优化 */\n";
    rewrite.InsertText(loc, hint, true);
}

void ExtendedCodeModifier::addParallelizationHint(const clang::ForStmt *Loop)
{
    clang::SourceLocation loc = Loop->getBeginLoc();
    std::string hint = "/* 考虑使用OpenMP并行化 */\n";
    rewrite.InsertText(loc, hint, true);
}