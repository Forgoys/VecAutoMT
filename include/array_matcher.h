#ifndef ARRAY_MATCHER_H
#define ARRAY_MATCHER_H

#include <clang/AST/RecursiveASTVisitor.h>

#include "util.h"
using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;

// 数组操作类型
enum class ArrayOpType
{
    READ, // 读取操作
    WRITE, // 写入操作
    READ_WRITE // 读写操作
};

// 扩展的数组计算信息
struct ArrayComputeInfo
{
    std::string expression; // 计算表达式
    ArrayOpType opType; // 操作类型
    LocationInfo location; // 位置信息
    std::vector<std::string> dependentVars; // 依赖的其他变量
    [[nodiscard]] std::string toString() const
    {
        std::string opTypeStr;
        switch (opType) {
            case ArrayOpType::READ:
                opTypeStr = "READ";
                break;
            case ArrayOpType::WRITE:
                opTypeStr = "WRITE";
                break;
            case ArrayOpType::READ_WRITE:
                opTypeStr = "READ_WRITE";
                break;
        }

        std::string deps = "[";
        for (const auto &var: dependentVars) {
            if (deps.length() > 1) {
                deps += ", ";
            }
            deps += var;
        }
        deps += "]";

        return "ArrayComputeInfo {\n"
               "  expression: '" + expression + "',\n"
               "  opType: " + opTypeStr + ",\n"
               "  location: " + location.toString() + ",\n"
               "  dependentVars: " + deps + "\n"
               "}";
    }
};

// 扩展的数组访问信息
struct ArrayAccessInfo
{
    std::string arrayName;
    LocationInfo arrayLocation;
    LocationInfo forLoopLocation;
    std::string indexVarName;
    std::vector<ArrayComputeInfo> computeInfos; // 所有相关的计算

    // 循环信息
    struct
    {
        int64_t start;
        int64_t end;
        int64_t step;
        bool isConstBound;

        [[nodiscard]] std::string toString() const
        {
            return "LoopInfo { "
                   + std::string("start: ") + std::to_string(start) + ", "
                   + std::string("end: ") + std::to_string(end) + ", "
                   + std::string("step: ") + std::to_string(step) + ", "
                   + std::string("isConstBound: ") + (isConstBound ? "true" : "false") + " }";
        }
    } loopInfo;

    json toJson() const;

    static ArrayAccessInfo fromJson(const json &j);

    [[nodiscard]] std::string toString() const
    {
        std::string computeInfoStr = "[\n";
        for (const auto &computeInfo: computeInfos) {
            computeInfoStr += "    " + computeInfo.toString() + ",\n";
        }
        computeInfoStr += "  ]";

        return "ArrayAccessInfo {\n"
               "  arrayName: '" + arrayName + "',\n"
               "  arrayLocation: " + arrayLocation.toString() + ",\n"
               "  forLoopLocation: " + forLoopLocation.toString() + ",\n"
               "  indexVarName: '" + indexVarName + "',\n"
               "  computeInfos: " + computeInfoStr + ",\n"
               "  loopInfo: " + loopInfo.toString() + "\n"
               "}" + "\n";
    }
};

class ArrayMatcher : public MatchFinder::MatchCallback
{
public:
    ArrayMatcher(json &output) : outputJson(output)
    {
    }

    virtual void run(const MatchFinder::MatchResult &Result) override;

protected:
    json &outputJson;

    // 计算访问器 - 用于收集数组计算信息
    class ComputeVisitor : public RecursiveASTVisitor<ComputeVisitor>
    {
    public:
        ComputeVisitor(const std::string &arrayName,
                       SourceManager &SM,
                       const LangOptions &langOpts,
                       const std::string &funcName);

        bool VisitBinaryOperator(BinaryOperator *op);

        std::vector<ArrayComputeInfo> computeInfos;

    private:
        std::string arrayName;
        SourceManager &SM;
        const LangOptions &langOpts;
        std::string funcName;

        bool containsArray(const Expr *expr, const std::string &arrayName) const;

        ArrayOpType determineOpType(BinaryOperator *op) const;

        LocationInfo getLocation(BinaryOperator *op) const;

        void collectDependenciesImpl(const Expr *expr, std::vector<std::string> &deps) const;

        std::vector<std::string> collectDependencies(const Expr *expr) const;
    };

    // 处理locate模式
    void handleLocateMode(const MatchFinder::MatchResult &Result);

    // 处理restore模式
    void handleRestoreMode(const MatchFinder::MatchResult &Result);

    // 处理modify模式
    void handleModifyMode(const MatchFinder::MatchResult &Result)
    {
    };

    // 验证循环结构
    bool isValidLoop(const ForStmt *Loop) const;

    static std::string getSourceText(const Stmt *stmt, SourceManager &SM, const LangOptions &langOpts);

    std::string getCurrentFunctionName(const MatchFinder::MatchResult &Result);

    void collectArrayComputeInfo(const ArraySubscriptExpr *Array,
                                 const ForStmt *Loop,
                                 ArrayAccessInfo &Info,
                                 const MatchFinder::MatchResult &Result);

    // 分析数组操作类型
    ArrayOpType analyzeArrayOpType(const ArraySubscriptExpr *Array) const;

    // 获取循环信息
    bool getLoopInfo(const ForStmt *Loop, ArrayAccessInfo &Info) const;

private:
    // 新增：分析循环变量初始化
    bool analyzeLoopInit(const ForStmt *Loop, ArrayAccessInfo &Info);

    // 新增：分析循环条件
    bool analyzeLoopCondition(const ForStmt *Loop, ArrayAccessInfo &Info);

    // 新增：分析循环步进
    bool analyzeLoopIncrement(const ForStmt *Loop, ArrayAccessInfo &Info);

    // 新增：收集数组相关计算信息
    void collectArrayComputations(const Stmt *stmt,
                                  const std::string &arrayName,
                                  std::vector<ArrayComputeInfo> &computeInfos,
                                  SourceManager &SM,
                                  const LangOptions &langOpts,
                                  const std::string &funcName);
};

#endif // ARRAY_MATCHER_H
