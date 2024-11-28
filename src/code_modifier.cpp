#include "code_modifier.h"

void CodeModifier::run(const MatchFinder::MatchResult &Result)
{
    const auto *arrayExpr = Result.Nodes.getNodeAs<ArraySubscriptExpr>("arrayAccess");
    if (!arrayExpr)
        return;

    // 在数组访问前插入注释
    SourceLocation loc = arrayExpr->getBeginLoc();
    rewrite.InsertText(loc, "/* Array access modified */\n", true);
}

void ExtendedCodeModifier::addBoundsCheck(
    const ArraySubscriptExpr *Array, const ForStmt *Loop)
{
    SourceLocation loc = Array->getBeginLoc();

    // 获取数组名和索引变量名
    std::string arrayName;
    if (const auto *baseExpr = dyn_cast<DeclRefExpr>(Array->getBase()->IgnoreParenImpCasts()))
    {
        arrayName = baseExpr->getNameInfo().getAsString();
    }

    std::string indexName;
    if (const auto *idx = dyn_cast<DeclRefExpr>(Array->getIdx()->IgnoreParenImpCasts()))
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

void ExtendedCodeModifier::addOptimizationHint(const ArraySubscriptExpr *Array)
{
    SourceLocation loc = Array->getBeginLoc();
    std::string hint = "/* 考虑使用SIMD指令优化 */\n";
    rewrite.InsertText(loc, hint, true);
}

void ExtendedCodeModifier::addParallelizationHint(const ForStmt *Loop)
{
    SourceLocation loc = Loop->getBeginLoc();
    std::string hint = "/* 考虑使用OpenMP并行化 */\n";
    rewrite.InsertText(loc, hint, true);
}