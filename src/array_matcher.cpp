#include "array_matcher.h"

#include <command_line_options.h>

#include "json_manager.h"
#include "clang/AST/RecursiveASTVisitor.h"

void ArrayMatcher::run(const MatchFinder::MatchResult &Result)
{
    const auto &options = CommandLineOptions::instance();
    if (options.getLocateMode()) {
        handleLocateMode(Result);
    } else if (options.getRestoreMode()) {
        handleRestoreMode(Result);
    } else {
        handleModifyMode(Result);
    }
}

void ArrayMatcher::handleLocateMode(const MatchFinder::MatchResult &Result)
{
    const auto *arrayExpr = Result.Nodes.getNodeAs<ArraySubscriptExpr>("arrayAccess");
    const auto *forStmt = Result.Nodes.getNodeAs<ForStmt>("forLoop");
    const auto *func = Result.Nodes.getNodeAs<FunctionDecl>("containingFunction");

    if (!arrayExpr || !forStmt || !func || !isValidLoop(forStmt))
        return;

    // 创建基础信息
    ArrayAccessInfo info;

    // 获取源码管理器
    SourceManager &SM = *Result.SourceManager;
    const LangOptions &langOpts = Result.Context->getLangOpts();

    // 获取数组名称
    const Expr *base = arrayExpr->getBase()->IgnoreParenImpCasts();
    if (const auto *declRef = dyn_cast<DeclRefExpr>(base)) {
        info.arrayName = declRef->getDecl()->getNameAsString();
    }

    // 获取循环变量名
    if (const auto *init = dyn_cast<DeclStmt>(forStmt->getInit())) {
        if (const auto *varDecl = dyn_cast<VarDecl>(init->getSingleDecl())) {
            info.indexVarName = varDecl->getNameAsString();
        }
    }

    // 填充位置信息的辅助函数
    auto fillLocationInfo = [&SM, &langOpts](const Stmt *stmt, const std::string &funcName) -> LocationInfo {
        SourceLocation start = stmt->getBeginLoc();
        SourceLocation end = stmt->getEndLoc();

        LocationInfo loc;
        loc.filename = SM.getFilename(start).str();
        loc.functionName = funcName;
        loc.startLine = SM.getSpellingLineNumber(start);
        loc.endLine = SM.getSpellingLineNumber(end);
        loc.sourceText = Lexer::getSourceText(
            CharSourceRange::getCharRange(start, Lexer::getLocForEndOfToken(end, 0, SM, langOpts)),
            SM,
            langOpts
        ).str();

        return loc;
    };

    // 获取位置信息
    info.arrayLocation = fillLocationInfo(arrayExpr, func->getNameAsString());
    info.forLoopLocation = fillLocationInfo(forStmt, func->getNameAsString());

    // 获取循环信息
    // if (!getLoopInfo(forStmt, info)) {
    //     return;
    // }

    // 收集所有相关计算
    // collectArrayComputeInfo(arrayExpr, forStmt, info, Result);
    // llvm::outs() << info.toString();
    // 添加到输出JSON
    JsonManager::instance().addArrayAccess(info);
}

void ArrayMatcher::handleRestoreMode(const MatchFinder::MatchResult &Result)
{
    const auto *arrayExpr = Result.Nodes.getNodeAs<ArraySubscriptExpr>("arrayAccess");
    const auto *forStmt = Result.Nodes.getNodeAs<ForStmt>("forLoop");

    if (!arrayExpr || !forStmt || !isValidLoop(forStmt)) {
        return;
    }

    // 创建数组访问信息结构
    ArrayAccessInfo info;
    SourceManager &SM = *Result.SourceManager;
    const LangOptions &langOpts = Result.Context->getLangOpts();

    // 获取数组名称
    const Expr *base = arrayExpr->getBase()->IgnoreParenImpCasts();
    if (const auto *declRef = dyn_cast<DeclRefExpr>(base)) {
        info.arrayName = declRef->getDecl()->getNameAsString();
    }

    // 获取循环变量名称
    if (const auto *init = dyn_cast<DeclStmt>(forStmt->getInit())) {
        if (const auto *varDecl = dyn_cast<VarDecl>(init->getSingleDecl())) {
            info.indexVarName = varDecl->getNameAsString();
        }
    }

    // 收集位置信息
    info.arrayLocation = {
        SM.getFilename(arrayExpr->getBeginLoc()).str(),
        getCurrentFunctionName(Result),
        SM.getSpellingLineNumber(arrayExpr->getBeginLoc()),
        SM.getSpellingLineNumber(arrayExpr->getEndLoc()),
        getSourceText(arrayExpr, SM, langOpts)
    };

    info.forLoopLocation = {
        SM.getFilename(forStmt->getBeginLoc()).str(),
        getCurrentFunctionName(Result),
        SM.getSpellingLineNumber(forStmt->getBeginLoc()),
        SM.getSpellingLineNumber(forStmt->getEndLoc()),
        getSourceText(forStmt, SM, langOpts)
    };

    // 分析循环信息
    analyzeLoopInit(forStmt, info);
    analyzeLoopCondition(forStmt, info);
    analyzeLoopIncrement(forStmt, info);

    // 收集所有相关的数组计算
    collectArrayComputations(forStmt->getBody(),
                             info.arrayName,
                             info.computeInfos,
                             SM,
                             langOpts,
                             info.arrayLocation.functionName);

    // 输出调试信息
    // llvm::outs() << info.toString();

    // 将信息添加到输出JSON
    JsonManager::instance().addArrayAccess(info);
}

bool ArrayMatcher::analyzeLoopInit(const ForStmt *Loop, ArrayAccessInfo &Info)
{
    if (const auto *init = dyn_cast<DeclStmt>(Loop->getInit())) {
        if (const auto *varDecl = dyn_cast<VarDecl>(init->getSingleDecl())) {
            if (const auto *initExpr = varDecl->getInit()) {
                if (const auto *literal = dyn_cast<IntegerLiteral>(initExpr)) {
                    Info.loopInfo.start = literal->getValue().getSExtValue();
                    return true;
                }
            }
        }
    }
    Info.loopInfo.start = 0; // 默认值
    return false;
}

bool ArrayMatcher::analyzeLoopCondition(const ForStmt *Loop, ArrayAccessInfo &Info)
{
    if (const auto *cond = dyn_cast<BinaryOperator>(Loop->getCond())) {
        if (const auto *rhs = dyn_cast<IntegerLiteral>(cond->getRHS()->IgnoreParenImpCasts())) {
            Info.loopInfo.end = rhs->getValue().getSExtValue();
            Info.loopInfo.isConstBound = true;
            return true;
        } else if (const auto *declRef = dyn_cast<DeclRefExpr>(cond->getRHS()->IgnoreParenImpCasts())) {
            // 处理变量边界
            Info.loopInfo.isConstBound = false;
            return true;
        }
    }
    return false;
}

bool ArrayMatcher::analyzeLoopIncrement(const ForStmt *Loop, ArrayAccessInfo &Info)
{
    if (const auto *inc = dyn_cast<UnaryOperator>(Loop->getInc())) {
        Info.loopInfo.step = 1;
        return true;
    } else if (const auto *inc = dyn_cast<BinaryOperator>(Loop->getInc())) {
        if (const auto *literal = dyn_cast<IntegerLiteral>(inc->getRHS()->IgnoreParenImpCasts())) {
            Info.loopInfo.step = literal->getValue().getSExtValue();
            return true;
        }
    }
    Info.loopInfo.step = 1; // 默认步长
    return false;
}

void ArrayMatcher::collectArrayComputations(const Stmt *stmt,
                                            const std::string &arrayName,
                                            std::vector<ArrayComputeInfo> &computeInfos,
                                            SourceManager &SM,
                                            const LangOptions &langOpts,
                                            const std::string &funcName)
{
    // 使用递归遍历语法树来收集所有计算
    if (!stmt) return;

    // 检查是否是二元操作
    if (const auto *bo = dyn_cast<BinaryOperator>(stmt)) {
        bool containsArray = false;
        std::string expr = getSourceText(bo, SM, langOpts);

        // 检查左右操作数是否包含目标数组
        auto checkOperand = [&](const Expr *operand) {
            if (const auto *arrayRef = dyn_cast<ArraySubscriptExpr>(operand->IgnoreParenImpCasts())) {
                if (const auto *base = dyn_cast<DeclRefExpr>(arrayRef->getBase()->IgnoreParenImpCasts())) {
                    if (base->getNameInfo().getAsString() == arrayName) {
                        containsArray = true;
                    }
                }
            }
        };

        checkOperand(bo->getLHS());
        checkOperand(bo->getRHS());

        if (containsArray) {
            ArrayComputeInfo compInfo;
            compInfo.expression = expr;
            compInfo.location = {
                SM.getFilename(bo->getBeginLoc()).str(),
                funcName,
                SM.getSpellingLineNumber(bo->getBeginLoc()),
                SM.getSpellingLineNumber(bo->getEndLoc()),
                expr
            };

            // 确定操作类型
            if (bo->isAssignmentOp()) {
                compInfo.opType = containsArray ? ArrayOpType::WRITE : ArrayOpType::READ;
            } else {
                compInfo.opType = ArrayOpType::READ;
            }

            // 收集依赖变量
            class DependencyVisitor : public RecursiveASTVisitor<DependencyVisitor>
            {
            public:
                std::vector<std::string> dependencies;

                bool VisitDeclRefExpr(DeclRefExpr *expr)
                {
                    dependencies.push_back(expr->getNameInfo().getAsString());
                    return true;
                }
            };

            DependencyVisitor visitor;
            visitor.TraverseStmt(const_cast<Stmt *>(stmt));
            compInfo.dependentVars = std::move(visitor.dependencies);

            computeInfos.push_back(std::move(compInfo));
        }
    }

    // 递归遍历所有子节点
    for (const Stmt *child: stmt->children()) {
        collectArrayComputations(child, arrayName, computeInfos, SM, langOpts, funcName);
    }
}

bool ArrayMatcher::isValidLoop(const ForStmt *Loop) const
{
    // 验证是否是规范的for循环结构
    if (!Loop->getInit() || !Loop->getCond() || !Loop->getInc())
        return false;

    // 验证循环变量声明
    if (const auto *init = dyn_cast<DeclStmt>(Loop->getInit())) {
        if (!init->isSingleDecl() || !isa<VarDecl>(init->getSingleDecl()))
            return false;
    }

    return true;
}

// 获取源代码文本的辅助函数
std::string ArrayMatcher::getSourceText(const Stmt *stmt,
                                        SourceManager &SM,
                                        const LangOptions &langOpts)
{
    SourceLocation start = stmt->getBeginLoc();
    SourceLocation end = stmt->getEndLoc();
    return Lexer::getSourceText(
        CharSourceRange::getCharRange(start,
                                      Lexer::getLocForEndOfToken(end, 0, SM, langOpts)),
        SM, langOpts).str();
}

// 获取当前函数名的辅助函数
std::string ArrayMatcher::getCurrentFunctionName(const MatchFinder::MatchResult &Result)
{
    if (const auto *function = Result.Nodes.getNodeAs<FunctionDecl>("functionDecl")) {
        return function->getNameAsString();
    }
    return "";
}

// 收集数组计算信息
void ArrayMatcher::collectArrayComputeInfo(const ArraySubscriptExpr *Array,
                                           const ForStmt *Loop,
                                           ArrayAccessInfo &Info,
                                           const MatchFinder::MatchResult &Result)
{
    ComputeVisitor visitor(Info.arrayName,
                           *Result.SourceManager,
                           Result.Context->getLangOpts(),
                           Info.arrayLocation.functionName);
    visitor.TraverseStmt(const_cast<Stmt *>(Loop->getBody()));
    Info.computeInfos = std::move(visitor.computeInfos);
}

bool ArrayMatcher::getLoopInfo(const ForStmt *Loop, ArrayAccessInfo &Info) const
{
    // 分析循环初始值
    if (const auto *init = dyn_cast<DeclStmt>(Loop->getInit())) {
        if (const auto *varDecl = dyn_cast<VarDecl>(init->getSingleDecl())) {
            if (const auto *initExpr = varDecl->getInit()) {
                if (const auto *literal = dyn_cast<IntegerLiteral>(initExpr)) {
                    Info.loopInfo.start = literal->getValue().getSExtValue();
                }
            }
        }
    }

    // 分析循环条件
    if (const auto *cond = dyn_cast<BinaryOperator>(Loop->getCond())) {
        if (const auto *rhs = dyn_cast<IntegerLiteral>(cond->getRHS()->IgnoreParenImpCasts())) {
            Info.loopInfo.end = rhs->getValue().getSExtValue();
            Info.loopInfo.isConstBound = true;
        } else {
            Info.loopInfo.isConstBound = false;
        }
    }

    // 分析步长
    if (const auto *inc = dyn_cast<UnaryOperator>(Loop->getInc())) {
        Info.loopInfo.step = 1;
    } else if (const auto *inc = dyn_cast<BinaryOperator>(Loop->getInc())) {
        if (const auto *literal = dyn_cast<IntegerLiteral>(inc->getRHS()->IgnoreParenImpCasts())) {
            Info.loopInfo.step = literal->getValue().getSExtValue();
        }
    }

    return true;
}

ArrayMatcher::ComputeVisitor::ComputeVisitor(const std::string &arrayName,
                                             SourceManager &SM,
                                             const LangOptions &langOpts,
                                             const std::string &funcName)
    : arrayName(arrayName), SM(SM), langOpts(langOpts), funcName(funcName)
{
}

bool ArrayMatcher::ComputeVisitor::VisitBinaryOperator(BinaryOperator *op)
{
    // 检查是否包含目标数组
    if (containsArray(op, arrayName)) {
        ArrayComputeInfo compute;
        compute.expression = getSourceText(op, SM, langOpts);
        compute.opType = determineOpType(op);
        compute.location = getLocation(op);
        compute.dependentVars = collectDependencies(op);
        computeInfos.push_back(compute);
    }
    return true;
}

bool ArrayMatcher::ComputeVisitor::containsArray(const Expr *expr, const std::string &arrayName) const
{
    if (const auto *arrayExpr = dyn_cast<ArraySubscriptExpr>(expr)) {
        if (const auto *base = dyn_cast<DeclRefExpr>(
            arrayExpr->getBase()->IgnoreParenImpCasts())) {
            return base->getNameInfo().getAsString() == arrayName;
        }
    }
    return false;
}

ArrayOpType ArrayMatcher::ComputeVisitor::determineOpType(BinaryOperator *op) const
{
    if (op->isAssignmentOp()) {
        return containsArray(op->getLHS(), arrayName) ? ArrayOpType::WRITE : ArrayOpType::READ;
    }
    return ArrayOpType::READ;
}

LocationInfo ArrayMatcher::ComputeVisitor::getLocation(BinaryOperator *op) const
{
    return {
        SM.getFilename(op->getBeginLoc()).str(),
        funcName,
        SM.getSpellingLineNumber(op->getBeginLoc()),
        SM.getSpellingLineNumber(op->getEndLoc()),
        getSourceText(op, SM, langOpts)
    };
}

std::vector<std::string> ArrayMatcher::ComputeVisitor::collectDependencies(const Expr *expr) const
{
    std::vector<std::string> deps;
    collectDependenciesImpl(expr, deps);
    return deps;
}

void ArrayMatcher::ComputeVisitor::collectDependenciesImpl(const Expr *expr, std::vector<std::string> &deps) const
{
    if (!expr) return;

    if (const auto *declRef = dyn_cast<DeclRefExpr>(expr)) {
        deps.push_back(declRef->getNameInfo().getAsString());
    }

    for (const Stmt *child: expr->children()) {
        if (const Expr *childExpr = dyn_cast_or_null<Expr>(child)) {
            collectDependenciesImpl(childExpr, deps);
        }
    }
}
