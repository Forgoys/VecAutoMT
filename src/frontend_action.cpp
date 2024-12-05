#include "frontend_action.h"
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
    auto& options = CommandLineOptions::instance();

    if (!options.getModifyMode()) {
        // 只在非修改模式下应用预处理配置
        PreprocessorConfig::configure(CI);
        std::cout << "Preprocessor configuration completed.\n";
    } else {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    }
    return true;
}

MyASTConsumer::MyASTConsumer()
{
    auto& options = CommandLineOptions::instance();
    auto& jsonManager = JsonManager::instance();

    if (options.getLocateMode()) {
        std::cout << "Running in locate mode.\n";
        auto matcher = forStmt(
            hasLoopInit(declStmt(hasSingleDecl(varDecl().bind("loopVar")))),
            forEachDescendant(arraySubscriptExpr().bind("arrayAccess")),
            hasAncestor(functionDecl().bind("containingFunction"))
        ).bind("forLoop");
        matchFinder.addMatcher(matcher, new ArrayMatcher(jsonManager.getOutputJson()));

    } else if (options.getRestoreMode()) {
        std::cout << "Running in restore mode.\n";
        auto& inputJson = jsonManager.getInputJson();
        if (!inputJson.is_null()) {
            for (const auto &item: inputJson) {
                auto matcher = arraySubscriptExpr(
                    hasAncestor(functionDecl(hasName(item["function"].get<std::string>()))),
                    hasAncestor(forStmt().bind("forLoop"))
                ).bind("arrayAccess");
                matchFinder.addMatcher(matcher, new ArrayMatcher(jsonManager.getOutputJson()));
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
}

std::unique_ptr<clang::tooling::FrontendActionFactory> createCustomFrontendActionFactory()
{
    class CustomFrontendActionFactory : public clang::tooling::FrontendActionFactory {
    public:
        std::unique_ptr<clang::FrontendAction> create() override {
            return std::make_unique<MyFrontendAction>();
        }
    };
    return std::make_unique<CustomFrontendActionFactory>();
}