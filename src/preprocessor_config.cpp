#include "preprocessor_config.h"
#include "array_matcher.h"
#include "code_modifier.h"
#include "util.h"

void PreprocessorConfig::configure(clang::CompilerInstance &CI)
{
    clang::Preprocessor &PP = CI.getPreprocessor();

    // 仅在分析阶段处理特殊属性
    if (getLocateMode() || getRestoreMode()) {
        std::string predefs = PP.getPredefines();
        predefs += "\n"
                   "#ifdef global\n"
                   "#undef global\n"
                   "#endif\n"
                   "#define global\n"
                   "#ifdef share\n"
                   "#undef share\n"
                   "#endif\n"
                   "#define share\n"
                   "#ifdef gsm\n"
                   "#undef gsm\n"
                   "#endif\n"
                   "#define gsm\n"
                   "#ifdef EXTRACT_VECTOR\n"
                   "#undef EXTRACT_VECTOR\n"
                   "#endif\n"
                   "#define EXTRACT_VECTOR(vr, i, r) ((void)0)\n"
                   "#ifdef __attribute__\n"
                   "#undef __attribute__\n"
                   "#endif\n"
                   "#define __attribute__(x)\n"
                   "#ifdef section\n"
                   "#undef section\n"
                   "#endif\n"
                   "#define section(x)\n";
        PP.setPredefines(predefs);
    }
}

// 使用外部声明的命令行选项
extern llvm::cl::opt<bool> LocateMode;
extern llvm::cl::opt<bool> RestoreMode;
extern llvm::cl::opt<bool> ModifyMode;
extern json outputJson;

std::unique_ptr<clang::ASTConsumer> CustomFrontendAction::CreateASTConsumer(clang::CompilerInstance &CI,
                                                                            llvm::StringRef InFile)
{
    // 根据模式选择是否配置预处理器
    if (!getModifyMode()) { // 只在非修改模式下应用预处理配置
        PreprocessorConfig::configure(CI);
    }

    // 创建匹配查找器
    auto Finder = std::make_unique<clang::ast_matchers::MatchFinder>();

    if (getLocateMode()) {
        // 定位模式的匹配器
        auto matcher = forStmt(hasLoopInit(declStmt(hasSingleDecl(varDecl().bind("loopVar")))),
                               hasBody(hasDescendant(arraySubscriptExpr().bind("arrayAccess"))),
                               hasAncestor(functionDecl().bind("containingFunction")))
                           .bind("forLoop");

        Finder->addMatcher(matcher, new ExtendedArrayMatcher(outputJson));
    } else if (getRestoreMode()) {
        if (!inputJson.is_null()) {
            // 遍历JSON文件中的每个位置信息
            for (const auto &item : inputJson) {
                auto matcher =
                    arraySubscriptExpr(hasAncestor(functionDecl(hasName(item["function"].get<std::string>()))),
                                       hasAncestor(forStmt().bind("forLoop")))
                        .bind("arrayAccess");

                Finder->addMatcher(matcher, new ArrayMatcher(outputJson));
            }
        }
    } else if (getModifyMode()) {
        // 修改模式的匹配器
        auto matcher = arraySubscriptExpr(hasAncestor(forStmt().bind("forLoop"))).bind("arrayAccess");

        Finder->addMatcher(matcher, new ExtendedCodeModifier(TheRewriter));
    }

    // 创建并返回AST消费者
    return Finder->newASTConsumer();
}

bool CustomFrontendAction::BeginSourceFileAction(clang::CompilerInstance &CI)
{
    if (getModifyMode()) {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    }
    return true;
}

std::unique_ptr<clang::tooling::FrontendActionFactory> createCustomFrontendActionFactory()
{
    class CustomFrontendActionFactory : public clang::tooling::FrontendActionFactory
    {
    public:
        std::unique_ptr<clang::FrontendAction> create() override { return std::make_unique<CustomFrontendAction>(); }
    };

    return std::make_unique<CustomFrontendActionFactory>();
}