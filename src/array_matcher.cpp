#include "array_matcher.h"
#include "json_manager.h"

void ArrayMatcher::run(const MatchFinder::MatchResult &Result)
{
    const auto *arrayExpr = Result.Nodes.getNodeAs<ArraySubscriptExpr>("arrayAccess");
    const auto *forStmt = Result.Nodes.getNodeAs<ForStmt>("forLoop");
    const auto *loopVar = Result.Nodes.getNodeAs<VarDecl>("loopVar");
    const auto *func = Result.Nodes.getNodeAs<FunctionDecl>("containingFunction");

    if (!arrayExpr || !forStmt || !loopVar || !func)
        return;

    // 获取源码管理器
    SourceManager &SM = *Result.SourceManager;

    // 检查文件名是否匹配
    std::string filename = SM.getFilename(arrayExpr->getBeginLoc()).str();
    if (filename.empty()) {
        return;
    }

    // 收集位置信息
    ArrayAccessInfo info;

    // 获取数组名称
    const Expr *base = arrayExpr->getBase()->IgnoreParenImpCasts();
    if (const auto *declRef = dyn_cast<DeclRefExpr>(base)) {
        info.arrayName = declRef->getDecl()->getNameAsString();
    } else {
        info.arrayName = base->getType().getAsString();
    }

    info.indexVarName = loopVar->getNameAsString();

    // 填充位置信息
    auto fillLocationInfo = [&SM](const clang::Stmt *stmt, const std::string &funcName) -> LocationInfo {
        SourceLocation start = stmt->getBeginLoc();
        SourceLocation end = stmt->getEndLoc();

        LocationInfo loc;
        loc.filename = SM.getFilename(start).str();
        loc.functionName = funcName;
        loc.startLine = SM.getSpellingLineNumber(start);
        loc.endLine = SM.getSpellingLineNumber(end);
        loc.sourceText = Lexer::getSourceText(
            CharSourceRange::getCharRange(start, Lexer::getLocForEndOfToken(end, 0, SM, LangOptions())),
            SM,
            LangOptions()
        ).str();

        return loc;
    };

    // 获取数组位置信息
    info.arrayLocation = fillLocationInfo(arrayExpr, func->getNameAsString());

    // 获取循环位置信息
    info.forLoopLocation = fillLocationInfo(forStmt, func->getNameAsString());

    // 使用 JsonManager 添加数组访问信息
    JsonManager::instance().addArrayAccess(info);

    // 终端输出
    llvm::outs() << "Found array access:\n"
            << "  File: " << info.arrayLocation.filename << "\n"
            << "  Function: " << info.arrayLocation.functionName << "\n"
            << "  Loop location: Line " << info.forLoopLocation.startLine << "\n"
            << "  Array: " << info.arrayName << "\n"
            << "  Index variable: " << info.indexVarName << "\n"
            << "  Source code: " << info.arrayLocation.sourceText << "\n\n";
}

bool ArrayMatcher::isSimpleIncrement(const Stmt *Inc) const
{
    if (const auto *unary = dyn_cast<UnaryOperator>(Inc)) {
        return unary->isIncrementOp() && unary->isPostfix();
    }
    return false;
}

bool ArrayMatcher::isArrayIndexFromLoop(const ArraySubscriptExpr *Array, const VarDecl *LoopVar) const
{
    const Expr *idx = Array->getIdx()->IgnoreParenImpCasts();

    // 检查直接使用循环变量的情况
    if (const auto *declRef = dyn_cast<DeclRefExpr>(idx)) {
        return declRef->getDecl() == LoopVar;
    }

    // 检查循环变量的简单运算 (i+1, i-1 等)
    if (const auto *binOp = dyn_cast<BinaryOperator>(idx)) {
        if (binOp->isAdditiveOp()) {
            const Expr *lhs = binOp->getLHS()->IgnoreParenImpCasts();
            if (const auto *declRef = dyn_cast<DeclRefExpr>(lhs)) {
                return declRef->getDecl() == LoopVar;
            }
        }
    }

    return false;
}

// ExtendedArrayMatcher的实现
bool ExtendedArrayMatcher::isSteppedIncrement(const Stmt *Inc, int &stepSize) const
{
    if (const auto *binary = dyn_cast<BinaryOperator>(Inc)) {
        if (binary->getOpcode() == BO_AddAssign) {
            if (const auto *literal = dyn_cast<IntegerLiteral>(binary->getRHS()->IgnoreParenImpCasts())) {
                stepSize = literal->getValue().getLimitedValue();
                return true;
            }
        }
    }
    return false;
}

bool ExtendedArrayMatcher::isMultiDimensionalAccess(const ArraySubscriptExpr *Array) const
{
    const Expr *base = Array->getBase()->IgnoreParenImpCasts();
    return isa<ArraySubscriptExpr>(base);
}

bool ExtendedArrayMatcher::hasBoundsChecking(const ArraySubscriptExpr *Array, const ForStmt *Loop) const
{
    // 检查循环条件
    if (const auto *cond = dyn_cast<BinaryOperator>(Loop->getCond())) {
        if (cond->getOpcode() == BO_LT || cond->getOpcode() == BO_LE) {
            // TODO: 进一步检查边界值是否与数组大小相关
            return true;
        }
    }
    return false;
}
