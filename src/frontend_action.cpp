#include "preprocessor_config.h"
#include "frontend_action.h"
#include "array_matcher.h"
#include "code_modifier.h"
#include "util.h"
#include <iostream>

// 使用外部声明的命令行选项
extern llvm::cl::opt<bool> LocateMode;
extern llvm::cl::opt<bool> RestoreMode;
extern llvm::cl::opt<bool> ModifyMode;
extern json inputJson;
extern json outputJson;

std::unique_ptr<clang::ASTConsumer> MyFrontendAction::CreateASTConsumer(clang::CompilerInstance &CI,
                                                                        llvm::StringRef InFile)
{
    return std::make_unique<MyASTConsumer>(inputJson, outputJson);
}

bool MyFrontendAction::BeginSourceFileAction(clang::CompilerInstance &CI)
{
    // 根据模式选择是否配置预处理器
    if (!getModifyMode()) {
        // 只在非修改模式下应用预处理配置
        PreprocessorConfig::configure(CI);
        std::cout << "preprocess config done.\n";
    } else {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    }
    return true;
}

MyASTConsumer::MyASTConsumer(json &inputJson, json &outputJson)
{
    // 创建匹配查找器
    if (getLocateMode()) {
        // 定位模式的匹配器
        std::cout << "Locate mode.\n";
        // auto matcher = forStmt(hasLoopInit(declStmt(hasSingleDecl(varDecl().bind("loopVar")))),
        //                        hasBody(hasDescendant(arraySubscriptExpr().bind("arrayAccess"))),
        //                        hasAncestor(functionDecl().bind("containingFunction")))
        //         .bind("forLoop");
        auto matcher = forStmt(
            hasLoopInit(declStmt(hasSingleDecl(varDecl().bind("loopVar")))),
            forEachDescendant(arraySubscriptExpr().bind("arrayAccess")),
            hasAncestor(functionDecl().bind("containingFunction"))
        ).bind("forLoop");
        matchFinder.addMatcher(matcher, new ArrayMatcher(outputJson));
        // Matcher->addMatcher(matcher, new ForLoopHandler());
    } else if (getRestoreMode()) {
        std::cout << "Restore mode.\n";
        if (!inputJson.is_null()) {
            // 遍历JSON文件中的每个位置信息
            for (const auto &item: inputJson) {
                auto matcher =
                        arraySubscriptExpr(hasAncestor(functionDecl(hasName(item["function"].get<std::string>()))),
                                           hasAncestor(forStmt().bind("forLoop")))
                        .bind("arrayAccess");

                matchFinder.addMatcher(matcher, new ArrayMatcher(outputJson));
            }
        }
    } else if (getModifyMode()) {
        std::cout << "Modify mode.\n";
        // 修改模式的匹配器
        auto matcher = arraySubscriptExpr(hasAncestor(forStmt().bind("forLoop"))).bind("arrayAccess");

        matchFinder.addMatcher(matcher, new ExtendedCodeModifier(TheRewriter));
    }
}

void MyASTConsumer::HandleTranslationUnit(clang::ASTContext &Context)
{
    matchFinder.matchAST(Context);
}

std::unique_ptr<clang::tooling::FrontendActionFactory> createCustomFrontendActionFactory()
{
    class CustomFrontendActionFactory : public clang::tooling::FrontendActionFactory
    {
    public:
        std::unique_ptr<clang::FrontendAction> create() override { return std::make_unique<MyFrontendAction>(); }
    };

    return std::make_unique<CustomFrontendActionFactory>();
}
