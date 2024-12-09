# include "frontend_action.h"
#include "preprocessor_config.h"
#include "array_matcher.h"
#include "code_modifier.h"
#include <iostream>

std::unique_ptr<clang::ASTConsumer> MyFrontendAction::CreateASTConsumer(
    clang::CompilerInstance &CI,
    llvm::StringRef InFile)
{
    return std::make_unique<MyASTConsumer>();
}

bool MyFrontendAction::BeginSourceFileAction(clang::CompilerInstance &CI)
{
    auto &options = CommandLineOptions::instance();

    // 设置诊断选项
    if (CI.hasDiagnostics()) {
        auto &diagEngine = CI.getDiagnostics();
        diagEngine.setShowColors(!options.getDiagnosticLogToFile());
        // 注意：其他诊断选项已在DiagnosticConsumer中设置
    }

    if (!options.getModifyMode()) {
        PreprocessorConfig::configure(CI);
        std::cout << "Preprocessor configuration completed.\n";
    } else {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    }
    return true;
}

MyASTConsumer::MyASTConsumer()
{
    auto &options = CommandLineOptions::instance();
    auto &jsonManager = JsonManager::instance();

    if (options.getLocateMode()) {
        jsonManager.clearOutputJson();

        auto matcher = forStmt(
            hasLoopInit(declStmt(hasSingleDecl(varDecl().bind("loopVar")))),
            forEachDescendant(arraySubscriptExpr().bind("arrayAccess")),
            hasAncestor(functionDecl().bind("containingFunction"))
        ).bind("forLoop");

        matchFinder.addMatcher(matcher, new ArrayMatcher(jsonManager.getOutputJson()));
        std::cout << "Array matcher added successfully.\n";
    } else if (options.getRestoreMode()) {
        auto &inputJson = jsonManager.getInputJson();

        for (const auto &item: inputJson) {
            try {
                if (!item.contains("functions")) {
                    std::cerr << "Warning: Input JSON item missing 'functions' field\n";
                    continue;
                }

                for (const auto &func: item["functions"]) {
                    if (!func.contains("name") || !func.contains("forLoops")) {
                        std::cerr << "Warning: Function object missing required fields\n";
                        continue;
                    }

                    std::string funcName = func["name"].get<std::string>();

                    for (const auto &forloop: func["forLoops"]) {
                        if (!forloop.contains("arrays") || !forloop.contains("startLine")) {
                            std::cerr << "Warning: ForLoop missing required fields\n";
                            continue;
                        }

                        int startLine = forloop["startLine"].get<int>();

                        for (const auto &array: forloop["arrays"]) {
                            if (!array.contains("name") || !array.contains("indexVar") || !array.contains("line")) {
                                std::cerr << "Warning: Array object missing required fields\n";
                                continue;
                            }

                            std::string arrayName = array["name"].get<std::string>();
                            std::string indexVar = array["indexVar"].get<std::string>();
                            int arrayLine = array["line"].get<int>();

                            // 构建更精确的matcher
                            auto matcher = arraySubscriptExpr(
                                // 确保在指定函数内
                                hasAncestor(functionDecl(hasName(funcName)).bind("functionDecl")),
                                // 确保基础表达式是正确的数组
                                hasLHS(implicitCastExpr(has(declRefExpr(to(varDecl(hasName(arrayName))))))),
                                // 确保索引变量匹配
                                hasRHS(implicitCastExpr(has(declRefExpr(to(varDecl(hasName(indexVar))))))),
                                // 确保在for循环内
                                hasAncestor(forStmt().bind("forLoop"))
                            ).bind("arrayAccess");

                            // 将array信息传递给matcher处理器
                            matchFinder.addMatcher(matcher, new ArrayMatcher(jsonManager.getOutputJson()));
                        }
                    }
                }
            } catch (const json::exception &e) {
                std::cerr << "Warning: Error processing JSON item: " << e.what() << "\n";
                continue;
            }
        }
    } else if (options.getModifyMode()) {
        auto matcher = arraySubscriptExpr(
            hasAncestor(forStmt().bind("forLoop"))
        ).bind("arrayAccess");
        matchFinder.addMatcher(matcher, new ExtendedCodeModifier(TheRewriter));
    }
}

void MyASTConsumer::HandleTranslationUnit(clang::ASTContext &Context)
{
    matchFinder.matchAST(Context);

    auto &options = CommandLineOptions::instance();
    if (options.getLocateMode() && !options.getOutputFile().empty()) {
        try {
            JsonManager::instance().writeOutputJson(options.getOutputFile());
            std::cout << "JSON output written to: " << options.getOutputFile() << "\n";
        } catch (const std::exception &e) {
            std::cerr << "Error writing JSON output: " << e.what() << "\n";
        }
    }
}

// 自定义 FrontendAction 类
class CustomFrontendAction : public MyFrontendAction
{
public:
    explicit CustomFrontendAction(MyDiagnosticConsumer *consumer)
        : DiagConsumer(consumer)
    {
    }

    bool BeginSourceFileAction(clang::CompilerInstance &CI) override
    {
        if (CI.hasDiagnostics() && DiagConsumer) {
            CI.getDiagnostics().setClient(DiagConsumer, false); // false表示不接管所有权
        }
        return MyFrontendAction::BeginSourceFileAction(CI);
    }

private:
    MyDiagnosticConsumer *DiagConsumer; // 改用原始指针
};

class CustomFrontendActionFactory : public clang::tooling::FrontendActionFactory
{
public:
    explicit CustomFrontendActionFactory(MyDiagnosticConsumer *consumer)
        : DiagConsumer(consumer)
    {
    }

    std::unique_ptr<clang::FrontendAction> create() override
    {
        return std::make_unique<CustomFrontendAction>(DiagConsumer);
    }

private:
    MyDiagnosticConsumer *DiagConsumer; // 改用原始指针
};

// 工厂函数的实现
std::unique_ptr<clang::tooling::FrontendActionFactory> createCustomFrontendActionFactory(
    MyDiagnosticConsumer *DiagConsumer)
{
    return std::make_unique<CustomFrontendActionFactory>(DiagConsumer);
}
