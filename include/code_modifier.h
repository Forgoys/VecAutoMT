#ifndef CODE_MODIFIER_H
#define CODE_MODIFIER_H

#include "util.h"
#include "clang/Rewrite/Core/Rewriter.h"

using namespace clang;
using namespace clang::ast_matchers;

// 代码修改器 - 基础版本
class CodeModifier : public clang::ast_matchers::MatchFinder::MatchCallback {
public:
    CodeModifier(clang::Rewriter& rewriter) : rewrite(rewriter) {}
    virtual void run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override;
    
protected:
    clang::Rewriter& rewrite;
};

// 代码修改器 - 扩展版本
class ExtendedCodeModifier : public CodeModifier {
public:
    ExtendedCodeModifier(clang::Rewriter& rewriter) : CodeModifier(rewriter) {}
    
    // 添加边界检查
    void addBoundsCheck(const clang::ArraySubscriptExpr* Array, const clang::ForStmt* Loop);
    // 添加优化提示
    void addOptimizationHint(const clang::ArraySubscriptExpr* Array);
    // 添加并行化提示
    void addParallelizationHint(const clang::ForStmt* Loop);
};

#endif // CODE_MODIFIER_H