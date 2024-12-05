#include "array_matcher.h"
#include <fstream>

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

    // 获取源码管理器
    SourceManager &SM = *Result.SourceManager;

    // 检查文件名是否匹配
    std::string filename = SM.getFilename(arrayExpr->getBeginLoc()).str();
    if (filename.empty()) {
        return;
    }

    // 检查行号是否在预期范围内
    // unsigned int lineNum = SM.getSpellingLineNumber(forStmt->getBeginLoc());
    //
    // // 验证循环条件
    // if (!isSimpleIncrement(forStmt->getInc()))
    //     return;
    //
    // // 验证数组访问
    // if (!isArrayIndexFromLoop(arrayExpr, loopVar))
    //     return;

    // 收集位置信息
    ArrayAccessInfo info;
    info.arrayName = arrayExpr->getBase()->getType().getAsString();
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

        // 获取源代码文本
        loc.sourceText = Lexer::getSourceText(CharSourceRange::getCharRange(
                                                  start, Lexer::getLocForEndOfToken(end, 0, SM, LangOptions())),
                                              SM, LangOptions())
                .str();

        return loc;
    };

    auto fillDeclLocationInfo = [&SM](const clang::Decl *decl, const std::string &funcName) -> LocationInfo {
        SourceLocation start = decl->getBeginLoc();
        SourceLocation end = decl->getEndLoc();

        LocationInfo loc;
        loc.filename = SM.getFilename(start).str();
        loc.functionName = funcName;
        loc.startLine = SM.getSpellingLineNumber(start);
        loc.endLine = SM.getSpellingLineNumber(end);

        // 获取源代码文本
        loc.sourceText = Lexer::getSourceText(CharSourceRange::getCharRange(
                                                  start, Lexer::getLocForEndOfToken(end, 0, SM, LangOptions())),
                                              SM, LangOptions())
                .str();

        return loc;
    };

    // 获取数组位置信息
    info.arrayLocation = fillLocationInfo(arrayExpr, func->getNameAsString());

    // 获取循环位置信息
    info.forLoopLocation = fillLocationInfo(forStmt, func->getNameAsString());

    // 收集相关代码位置
    if (const auto *baseExpr = arrayExpr->getBase()->IgnoreParenImpCasts()) {
        if (const auto *declRef = dyn_cast<DeclRefExpr>(baseExpr)) {
            if (const auto *varDecl = dyn_cast<VarDecl>(declRef->getDecl())) {
                info.relatedCodeLocations.push_back(fillDeclLocationInfo(varDecl, func->getNameAsString()));
            }
        }
    }

    // 在 array_matcher.cpp 的 run 方法中修改 JSON 处理部分
    try {
        llvm::outs() << "正在处理新的数组访问信息:\n";

        // 更新数组名称
        const Expr *base = arrayExpr->getBase()->IgnoreParenImpCasts();
        if (const auto *declRef = dyn_cast<DeclRefExpr>(base)) {
            info.arrayName = declRef->getDecl()->getNameAsString();
        } else {
            info.arrayName = base->getType().getAsString();
        }

        if (!outputJson.is_array()) {
            llvm::outs() << "初始化 outputJson 为空数组\n";
            outputJson = json::array();
        }

        // 创建数组信息对象
        json arrayInfo = json::object({
            {"name", info.arrayName},
            {"indexVar", info.indexVarName},
            {"line", info.arrayLocation.startLine},
            {"sourceText", info.arrayLocation.sourceText}
        });

        // 查找匹配的文件
        bool fileFound = false;
        for (auto &fileEntry: outputJson) {
            if (fileEntry["filename"] == info.arrayLocation.filename) {
                fileFound = true;

                // 查找匹配的函数
                bool functionFound = false;
                for (auto &funcEntry: fileEntry["functions"]) {
                    if (funcEntry["name"] == info.arrayLocation.functionName) {
                        functionFound = true;

                        // 查找匹配的循环
                        bool loopFound = false;
                        for (auto &loopEntry: funcEntry["forLoops"]) {
                            if (loopEntry["startLine"] == info.forLoopLocation.startLine) {
                                loopFound = true;

                                // 检查是否已存在相同的数组访问
                                bool arrayExists = false;
                                for (const auto &arr: loopEntry["arrays"]) {
                                    if (arr["name"] == info.arrayName &&
                                        arr["line"] == info.arrayLocation.startLine) {
                                        arrayExists = true;
                                        break;
                                    }
                                }

                                if (!arrayExists) {
                                    llvm::outs() << "添加新的数组访问: " << info.arrayName << "\n";
                                    loopEntry["arrays"].push_back(arrayInfo);
                                }
                                break;
                            }
                        }

                        if (!loopFound) {
                            // 创建新的循环条目
                            json loopEntry = json::object({
                                {"startLine", info.forLoopLocation.startLine},
                                {"endLine", info.forLoopLocation.endLine},
                                {"sourceText", info.forLoopLocation.sourceText},
                                {"arrays", json::array({arrayInfo})}
                            });
                            funcEntry["forLoops"].push_back(loopEntry);
                        }
                        break;
                    }
                }

                if (!functionFound) {
                    // 创建新的函数条目
                    json funcEntry = json::object({
                        {"name", info.arrayLocation.functionName},
                        {
                            "forLoops", json::array({
                                json::object({
                                    {"startLine", info.forLoopLocation.startLine},
                                    {"endLine", info.forLoopLocation.endLine},
                                    {"sourceText", info.forLoopLocation.sourceText},
                                    {"arrays", json::array({arrayInfo})}
                                })
                            })
                        }
                    });
                    fileEntry["functions"].push_back(funcEntry);
                }
                break;
            }
        }

        if (!fileFound) {
            // 创建新的文件条目
            json fileEntry = json::object({
                {"filename", info.arrayLocation.filename},
                {
                    "functions", json::array({
                        json::object({
                            {"name", info.arrayLocation.functionName},
                            {
                                "forLoops", json::array({
                                    json::object({
                                        {"startLine", info.forLoopLocation.startLine},
                                        {"endLine", info.forLoopLocation.endLine},
                                        {"sourceText", info.forLoopLocation.sourceText},
                                        {"arrays", json::array({arrayInfo})}
                                    })
                                })
                            }
                        })
                    })
                }
            });
            outputJson.push_back(fileEntry);
        }

        llvm::outs() << "当前 outputJson 内容:\n" << outputJson.dump(2) << "\n\n";
    } catch (const json::exception &e) {
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
