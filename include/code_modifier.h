#ifndef CODE_MODIFIER_H
#define CODE_MODIFIER_H

#include "util.h"
#include "clang/Rewrite/Core/Rewriter.h"

// 代码修改器 - 基础版本
class CodeModifier : public MatchFinder::MatchCallback {
public:
    CodeModifier(Rewriter& rewriter) : rewrite(rewriter) {}
    virtual void run(const MatchFinder::MatchResult &Result) override;
    
protected:
    Rewriter& rewrite;
};

// 代码修改器 - 扩展版本
class ExtendedCodeModifier : public CodeModifier {
public:
    ExtendedCodeModifier(Rewriter& rewriter) : CodeModifier(rewriter) {}
    
    // 添加边界检查
    void addBoundsCheck(const ArraySubscriptExpr* Array, const ForStmt* Loop);
    // 添加优化提示
    void addOptimizationHint(const ArraySubscriptExpr* Array);
    // 添加并行化提示
    void addParallelizationHint(const ForStmt* Loop);
};

#endif // CODE_MODIFIER_H