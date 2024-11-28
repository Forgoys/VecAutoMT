#include "util.h"
#include "array_matcher.h"
#include "code_modifier.h"
#include <fstream>
#include <iostream>

using namespace clang::tooling;
using namespace llvm;

// 命令行选项
static cl::OptionCategory ToolCategory("Array Analysis Tool Options");

static cl::opt<std::string> OutputFile(
    "output",
    cl::desc("指定输出文件"),
    cl::value_desc("filename"),
    cl::cat(ToolCategory));

static cl::opt<std::string> InputFile(
    "input",
    cl::desc("指定输入JSON文件"),
    cl::value_desc("filename"),
    cl::cat(ToolCategory));

static cl::opt<bool> LocateMode(
    "locate",
    cl::desc("启用定位模式"),
    cl::cat(ToolCategory));

static cl::opt<bool> RestoreMode(
    "restore",
    cl::desc("启用恢复模式"),
    cl::cat(ToolCategory));

static cl::opt<bool> ModifyMode(
    "modify",
    cl::desc("启用修改模式"),
    cl::cat(ToolCategory));

int main(int argc, const char **argv)
{
    // 解析命令行选项
    auto ExpectedParser = CommonOptionsParser::create(argc, argv, ToolCategory);
    if (!ExpectedParser)
    {
        llvm::errs() << ExpectedParser.takeError();
        return 1;
    }

    CommonOptionsParser &OptionsParser = ExpectedParser.get();
    ClangTool Tool(OptionsParser.getCompilations(),
                   OptionsParser.getSourcePathList());

    json outputJson = json::array();
    MatchFinder Finder;

    if (LocateMode)
    {
        // 定位模式：查找符合条件的数组访问
        auto matcher = forStmt(
                           hasLoopInit(declStmt(hasSingleDecl(varDecl().bind("loopVar")))),
                           hasBody(hasDescendant(arraySubscriptExpr().bind("arrayAccess"))),
                           hasAncestor(functionDecl().bind("containingFunction")))
                           .bind("forLoop");

        ExtendedArrayMatcher Callback(outputJson);
        Finder.addMatcher(matcher, &Callback);

        if (Tool.run(newFrontendActionFactory(&Finder).get()) != 0)
            return 1;

        // 输出JSON文件
        if (!OutputFile.empty())
        {
            std::ofstream out(OutputFile);
            out << outputJson.dump(4);
        }
    }
    else if (RestoreMode)
    {
        // 恢复模式：从JSON文件读取位置信息并重新定位
        if (InputFile.empty())
        {
            llvm::errs() << "错误：未指定输入JSON文件\n";
            return 1;
        }

        // 读取JSON文件
        std::ifstream input(InputFile);
        json inputJson;
        input >> inputJson;

        // 遍历JSON文件中的每个位置信息
        for (const auto &item : inputJson)
        {
            // 构建匹配器，根据函数名和位置信息定位代码
            auto matcher = arraySubscriptExpr(
                               // 匹配函数名
                               hasAncestor(
                                   functionDecl(hasName(item["function"].get<std::string>()))),
                               // 匹配for循环
                               hasAncestor(
                                   forStmt(
                                       // 匹配循环的起始行号
                                       hasSourceLocation(
                                           lineRange(
                                               item["forLoop"]["startLine"].get<unsigned>(),
                                               item["forLoop"]["endLine"].get<unsigned>())))
                                       .bind("forLoop")),
                               // 确保数组在正确的源文件中
                               hasSourceLocation(
                                   anyOf(
                                       filePathMatches(item["filename"].get<std::string>()),
                                       filePathEndsWith(item["filename"].get<std::string>()))))
                               .bind("arrayAccess");

            // 创建回调并添加匹配器
            ArrayMatcher Callback(outputJson);
            Finder.addMatcher(matcher, &Callback);
        }

        if (Tool.run(newFrontendActionFactory(&Finder).get()) != 0)
            return 1;

        // 输出恢复的位置信息
        if (!OutputFile.empty())
        {
            std::ofstream out(OutputFile);
            out << outputJson.dump(4);
        }
    }
    else if (ModifyMode)
    {
        // 修改模式：根据位置信息修改代码
        // 创建重写器
        Rewriter TheRewriter;

        // 构建匹配器
        auto matcher = arraySubscriptExpr(
                           hasAncestor(
                               forStmt().bind("forLoop")))
                           .bind("arrayAccess");

        // 创建扩展的代码修改器
        ExtendedCodeModifier ModCallback(TheRewriter);
        Finder.addMatcher(matcher, &ModCallback);

        std::unique_ptr<FrontendActionFactory> Factory =
            newFrontendActionFactory(&Finder);

        if (Tool.run(Factory.get()) != 0)
            return 1;

        // 输出修改后的代码
        if (!OutputFile.empty())
        {
            std::error_code EC;
            llvm::raw_fd_ostream OS(OutputFile, EC);
            if (EC)
            {
                llvm::errs() << "Error opening output file: " << EC.message() << "\n";
                return 1;
            }

            const RewriteBuffer *RewriteBuf =
                TheRewriter.getRewriteBufferFor(TheRewriter.getSourceMgr().getMainFileID());
            if (!RewriteBuf)
            {
                llvm::errs() << "No edits were made to the source file.\n";
                return 1;
            }

            OS << std::string(RewriteBuf->begin(), RewriteBuf->end());
        }
    }
    else
    {
        llvm::errs() << "错误：请指定运行模式（-locate、-restore 或 -modify）\n";
        return 1;
    }

    return 0;
}