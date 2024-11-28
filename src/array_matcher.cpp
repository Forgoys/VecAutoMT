#include "array_matcher.h"
#include <fstream>

void ArrayMatcher::run(const MatchFinder::MatchResult &Result) {
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
    auto fillLocationInfo = [&SM](const Stmt* stmt, const std::string& funcName) -> LocationInfo {
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
            LangOptions()
        ).str();
        
        return loc;
    };
    
    // 获取数组位置信息
    info.arrayLocation = fillLocationInfo(arrayExpr, func->getNameAsString());
    
    // 获取循环位置信息
    info.forLoopLocation = fillLocationInfo(forStmt, func->getNameAsString());
    
    // 收集相关代码位置
    // 包括数组声明、循环变量声明等
    if (const auto *arrayDecl = arrayExpr->getBase()->getType()->getAsArrayTypeUnsafe()) {
        if (const auto *declStmt = dyn_cast<DeclStmt>(arrayDecl->getElement()->getAsStmt())) {
            info.relatedCodeLocations.push_back(
                fillLocationInfo(declStmt, func->getNameAsString())
            );
        }
    }
    
    // 构建JSON输出
    json arrayJson;
    arrayJson["filename"] = info.arrayLocation.filename;
    arrayJson["function"] = info.arrayLocation.functionName;
    arrayJson["forLoop"] = {
        {"startLine", info.forLoopLocation.startLine},
        {"endLine", info.forLoopLocation.endLine},
        {"sourceText", info.forLoopLocation.sourceText}
    };
    
    // 添加数组信息
    json arrayInfo = {
        {"name", info.arrayName},
        {"indexVar", info.indexVarName},
        {"line", info.arrayLocation.startLine},
        {"sourceText", info.arrayLocation.sourceText}
    };
    
    // 检查是否已存在相同的forLoop信息
    bool found = false;
    for (auto& item : outputJson) {
        if (item["function"] == arrayJson["function"] &&
            item["forLoop"]["startLine"] == arrayJson["forLoop"]["startLine"]) {
            // 已存在相同的循环，添加数组信息
            item["arrays"].push_back(arrayInfo);
            found = true;
            break;
        }
    }
    
    if (!found) {
        // 新建forLoop条目
        arrayJson["arrays"] = json::array();
        arrayJson["arrays"].push_back(arrayInfo);
        outputJson.push_back(arrayJson);
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

bool ArrayMatcher::isSimpleIncrement(const Stmt* Inc) const {
    if (const auto *unary = dyn_cast<UnaryOperator>(Inc)) {
        return unary->isIncrementOp() && unary->isPostfix();
    }
    return false;
}

bool ArrayMatcher::isArrayIndexFromLoop(const ArraySubscriptExpr* Array,
                                      const VarDecl* LoopVar) const {
    if (const auto *idx = dyn_cast<DeclRefExpr>(Array->getIdx()->IgnoreParenImpCasts())) {
        return idx->getDecl() == LoopVar;
    }
    return false;
}

// ExtendedArrayMatcher的实现
bool ExtendedArrayMatcher::isSteppedIncrement(const Stmt* Inc, int& stepSize) const {
    if (const auto *binary = dyn_cast<BinaryOperator>(Inc)) {
        if (binary->getOpcode() == BO_AddAssign) {
            if (const auto *literal = dyn_cast<IntegerLiteral>(
                    binary->getRHS()->IgnoreParenImpCasts())) {
                stepSize = literal->getValue().getLimitedValue();
                return true;
            }
        }
    }
    return false;
}

bool ExtendedArrayMatcher::isMultiDimensionalAccess(
    const ArraySubscriptExpr* Array) const {
    const Expr* base = Array->getBase()->IgnoreParenImpCasts();
    return isa<ArraySubscriptExpr>(base);
}

bool ExtendedArrayMatcher::hasBoundsChecking(
    const ArraySubscriptExpr* Array, const ForStmt* Loop) const {
    // 检查循环条件
    if (const auto *cond = dyn_cast<BinaryOperator>(Loop->getCond())) {
        if (cond->getOpcode() == BO_LT || cond->getOpcode() == BO_LE) {
            // TODO: 进一步检查边界值是否与数组大小相关
            return true;
        }
    }
    return false;
}