#include "array_matcher.h"
#include <fstream>

void ArrayMatcher::run(const MatchFinder::MatchResult &Result)
{
    const auto *arrayExpr = Result.Nodes.getNodeAs<ArraySubscriptExpr>("arrayAccess");
    const auto *forStmt = Result.Nodes.getNodeAs<ForStmt>("forLoop");
    const auto *loopVar = Result.Nodes.getNodeAs<VarDecl>("loopVar");
    const auto *func = Result.Nodes.getNodeAs<FunctionDecl>("containingFunction");

    if (!arrayExpr || !forStmt || !loopVar || !func)
        return;

    // 验证循环条件
    if (!isSimpleIncrement(forStmt->getInc()))
        return;

    // 验证数组访问
    if (!isArrayIndexFromLoop(arrayExpr, loopVar))
        return;

    // 获取源码管理器
    SourceManager &SM = *Result.SourceManager;

    // 收集位置信息
    ArrayAccessInfo info;
    info.arrayName = arrayExpr->getBase()->getType().getAsString();
    info.indexVarName = loopVar->getNameAsString();

    // 填充位置信息
    auto fillLocationInfo = [&SM](const Stmt *stmt, const std::string &funcName) -> LocationInfo
    {
        SourceLocation start = stmt->getBeginLoc();
        SourceLocation end = stmt->getEndLoc();

        LocationInfo loc;
        loc.filename = SM.getFilename(start).str();
        loc.functionName = funcName;
        loc.startLine = SM.getSpellingLineNumber(start);
        loc.endLine = SM.getSpellingLineNumber(end);

        // 获取源代码文本
        loc.sourceText = Lexer::getSourceText(
                             CharSourceRange::getCharRange(start, Lexer::getLocForEndOfToken(end, 0, SM, LangOptions())),
                             SM,
                             LangOptions())
                             .str();

        return loc;
    };

    // 获取数组位置信息
    info.arrayLocation = fillLocationInfo(arrayExpr, func->getNameAsString());

    // 获取循环位置信息
    info.forLoopLocation = fillLocationInfo(forStmt, func->getNameAsString());

    // 收集相关代码位置
    // 包括数组声明、循环变量声明等
    if (const auto *arrayType = arrayExpr->getBase()->getType()->getAsArrayType())
    {
        // 获取数组声明
        if (const auto *baseExpr = arrayExpr->getBase()->IgnoreParenImpCasts())
        {
            if (const auto *declRef = dyn_cast<DeclRefExpr>(baseExpr))
            {
                if (const auto *varDecl = dyn_cast<VarDecl>(declRef->getDecl()))
                {
                    info.relatedCodeLocations.push_back(
                        fillLocationInfo(varDecl->getDefinition(), func->getNameAsString()));
                }
            }
        }
    }

    // 构建JSON输出
    try
    {
        // 构建JSON输出
        json arrayJson;
        arrayJson["filename"] = info.arrayLocation.filename;
        arrayJson["function"] = info.arrayLocation.functionName;
        arrayJson["forLoop"] = json::object();
        arrayJson["forLoop"]["startLine"] = info.forLoopLocation.startLine;
        arrayJson["forLoop"]["endLine"] = info.forLoopLocation.endLine;
        arrayJson["forLoop"]["sourceText"] = info.forLoopLocation.sourceText;

        // 添加数组信息
        json arrayInfo = json::object();
        arrayInfo["name"] = info.arrayName;
        arrayInfo["indexVar"] = info.indexVarName;
        arrayInfo["line"] = info.arrayLocation.startLine;
        arrayInfo["sourceText"] = info.arrayLocation.sourceText;

        // 检查是否已存在相同的forLoop信息
        bool found = false;
        for (auto &item : outputJson)
        {
            try
            {
                if (item.at("function") == arrayJson["function"] &&
                    item.at("forLoop").at("startLine") == arrayJson["forLoop"]["startLine"])
                {
                    if (!item.contains("arrays"))
                    {
                        item["arrays"] = json::array();
                    }
                    item["arrays"].push_back(arrayInfo);
                    found = true;
                    break;
                }
            }
            catch (const json::exception &e)
            {
                llvm::errs() << "JSON处理错误: " << e.what() << "\n";
                continue;
            }
        }

        if (!found)
        {
            arrayJson["arrays"] = json::array({arrayInfo});
            outputJson.push_back(arrayJson);
        }
    }
    catch (const json::exception &e)
    {
        llvm::errs() << "JSON处理错误: " << e.what() << "\n";
        return;
    }

    // 终端输出
    llvm::outs() << "发现数组访问:\n"
                 << "  文件: " << info.arrayLocation.filename << "\n"
                 << "  函数: " << info.arrayLocation.functionName << "\n"
                 << "  循环位置: 第" << info.forLoopLocation.startLine << "行\n"
                 << "  数组: " << info.arrayName << "\n"
                 << "  索引变量: " << info.indexVarName << "\n"
                 << "  源代码: " << info.arrayLocation.sourceText << "\n\n";
}

bool ArrayMatcher::isSimpleIncrement(const Stmt *Inc) const
{
    if (const auto *unary = dyn_cast<UnaryOperator>(Inc))
    {
        return unary->isIncrementOp() && unary->isPostfix();
    }
    return false;
}

bool isArrayIndexFromLoop(const ArraySubscriptExpr *Array,
                          const VarDecl *LoopVar) const
{
    const Expr *idx = Array->getIdx()->IgnoreParenImpCasts();

    // 检查直接使用循环变量的情况
    if (const auto *declRef = dyn_cast<DeclRefExpr>(idx))
    {
        return declRef->getDecl() == LoopVar;
    }

    // 检查循环变量的简单运算 (i+1, i-1 等)
    if (const auto *binOp = dyn_cast<BinaryOperator>(idx))
    {
        if (binOp->isAdditiveOp())
        {
            const Expr *lhs = binOp->getLHS()->IgnoreParenImpCasts();
            if (const auto *declRef = dyn_cast<DeclRefExpr>(lhs))
            {
                return declRef->getDecl() == LoopVar;
            }
        }
    }

    return false;
}

// ExtendedArrayMatcher的实现
bool ExtendedArrayMatcher::isSteppedIncrement(const Stmt *Inc, int &stepSize) const
{
    if (const auto *binary = dyn_cast<BinaryOperator>(Inc))
    {
        if (binary->getOpcode() == BO_AddAssign)
        {
            if (const auto *literal = dyn_cast<IntegerLiteral>(
                    binary->getRHS()->IgnoreParenImpCasts()))
            {
                stepSize = literal->getValue().getLimitedValue();
                return true;
            }
        }
    }
    return false;
}

bool ExtendedArrayMatcher::isMultiDimensionalAccess(
    const ArraySubscriptExpr *Array) const
{
    const Expr *base = Array->getBase()->IgnoreParenImpCasts();
    return isa<ArraySubscriptExpr>(base);
}

bool ExtendedArrayMatcher::hasBoundsChecking(
    const ArraySubscriptExpr *Array, const ForStmt *Loop) const
{
    // 检查循环条件
    if (const auto *cond = dyn_cast<BinaryOperator>(Loop->getCond()))
    {
        if (cond->getOpcode() == BO_LT || cond->getOpcode() == BO_LE)
        {
            // TODO: 进一步检查边界值是否与数组大小相关
            return true;
        }
    }
    return false;
}