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
        std::cout << "Running in locate mode.\n";

        auto matcher = forStmt(
            hasLoopInit(declStmt(hasSingleDecl(varDecl().bind("loopVar")))),
            forEachDescendant(arraySubscriptExpr().bind("arrayAccess")),
            hasAncestor(functionDecl().bind("containingFunction"))
        ).bind("forLoop");

        matchFinder.addMatcher(matcher, new ArrayMatcher(jsonManager.getOutputJson()));
        std::cout << "Array matcher added successfully.\n";
    } else if (options.getRestoreMode()) {
        std::cout << "Running in restore mode.\n";
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
        std::cout << "Running in modify mode.\n";
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

std::unique_ptr<clang::tooling::FrontendActionFactory> createCustomFrontendActionFactory()
{
    class CustomFrontendActionFactory : public clang::tooling::FrontendActionFactory
    {
    public:
        std::unique_ptr<clang::FrontendAction> create() override
        {
            return std::make_unique<MyFrontendAction>();
        }
    };
    return std::make_unique<CustomFrontendActionFactory>();
}
