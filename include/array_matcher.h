#ifndef ARRAY_MATCHER_H
#define ARRAY_MATCHER_H

#include "util.h"

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;

// 数组访问信息
struct ArrayAccessInfo
{
    std::string arrayName;
    LocationInfo arrayLocation;
    LocationInfo forLoopLocation;
    std::string indexVarName;
    std::vector<LocationInfo> relatedCodeLocations;

    json toJson() const;
    static ArrayAccessInfo fromJson(const json &j);
};

// 数组匹配器 - 基础版本
class ArrayMatcher : public MatchFinder::MatchCallback
{
public:
    ArrayMatcher(json &output) : outputJson(output) {}
    virtual void run(const MatchFinder::MatchResult &Result) override;

protected:
    json &outputJson;

    // 验证是否是简单的自增
    bool isSimpleIncrement(const Stmt *Inc) const;
    // 验证数组索引是否来自循环变量
    bool isArrayIndexFromLoop(const ArraySubscriptExpr *Array, const VarDecl *LoopVar) const;
};

// 数组匹配器 - 扩展版本
class ExtendedArrayMatcher : public ArrayMatcher
{
public:
    ExtendedArrayMatcher(json &output) : ArrayMatcher(output) {}

    // 检查步进循环
    bool isSteppedIncrement(const Stmt *Inc, int &stepSize) const;
    // 检查多维数组
    bool isMultiDimensionalAccess(const ArraySubscriptExpr *Array) const;
    // 检查边界
    bool hasBoundsChecking(const ArraySubscriptExpr *Array, const ForStmt *Loop) const;
};

#endif // ARRAY_MATCHER_H