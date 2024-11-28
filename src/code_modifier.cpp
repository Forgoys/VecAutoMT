#include "code_modifier.h"

void CodeModifier::run(const MatchFinder::MatchResult &Result) {
    const auto *arrayExpr = Result.Nodes.getNodeAs<ArraySubscriptExpr>("arrayAccess");
    if (!arrayExpr)
        return;
        
    // 在数组访问前插入注释
    SourceLocation loc = arrayExpr->getBeginLoc();
    rewrite.InsertText(loc, "/* Array access modified */\n", true);
}

void ExtendedCodeModifier::addBoundsCheck(
    const ArraySubscriptExpr* Array, const ForStmt* Loop) {
    SourceLocation loc = Array->getBeginLoc();
    std::string check = "if (index >= size) return -1; /* 添加边界检查 */\n";
    rewrite.InsertText(loc, check, true);
}

void ExtendedCodeModifier::addOptimizationHint(const ArraySubscriptExpr* Array) {
    SourceLocation loc = Array->getBeginLoc();
    std::string hint = "/* 考虑使用SIMD指令优化 */\n";
    rewrite.InsertText(loc, hint, true);
}

void ExtendedCodeModifier::addParallelizationHint(const ForStmt* Loop) {
    SourceLocation loc = Loop->getBeginLoc();
    std::string hint = "/* 考虑使用OpenMP并行化 */\n";
    rewrite.InsertText(loc, hint, true);
}