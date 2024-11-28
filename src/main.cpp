#include "array_matcher.h"
#include "code_modifier.h"
#include "preprocessor_config.h"
#include "util.h"
#include <fstream>
#include <iostream>

using namespace clang::tooling;
using namespace llvm;

// 命令行选项
static cl::OptionCategory ToolCategory("Array Analysis Tool Options");

static cl::opt<std::string> OutputFile("output", cl::desc("指定输出文件"), cl::value_desc("filename"),
                                       cl::cat(ToolCategory));

static cl::opt<std::string> InputFile("input", cl::desc("指定输入JSON文件"), cl::value_desc("filename"),
                                      cl::cat(ToolCategory));

// 定义并导出命令行选项
llvm::cl::opt<bool> LocateMode("locate", llvm::cl::desc("启用定位模式"), llvm::cl::cat(ToolCategory));

llvm::cl::opt<bool> RestoreMode("restore", llvm::cl::desc("启用恢复模式"), llvm::cl::cat(ToolCategory));

llvm::cl::opt<bool> ModifyMode("modify", llvm::cl::desc("启用修改模式"), llvm::cl::cat(ToolCategory));

// 定义getter函数
llvm::cl::opt<bool> &getLocateMode() { return LocateMode; }
llvm::cl::opt<bool> &getRestoreMode() { return RestoreMode; }
llvm::cl::opt<bool> &getModifyMode() { return ModifyMode; }

json inputJson = json::array();
json outputJson = json::array();
clang::Rewriter TheRewriter; // 在ModifyMode中初始化

int main(int argc, const char **argv)
{
    // 解析命令行选项
    auto ExpectedParser = CommonOptionsParser::create(argc, argv, ToolCategory);
    if (!ExpectedParser) {
        llvm::errs() << ExpectedParser.takeError();
        return 1;
    }

    CommonOptionsParser &OptionsParser = ExpectedParser.get();
    ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());

    MatchFinder Finder;

    if (getLocateMode()) {
        // 使用自定义的 FrontendAction
        if (Tool.run(createCustomFrontendActionFactory().get()) != 0)
            return 1;

        // 输出JSON文件
        if (!OutputFile.empty()) {
            std::ofstream out(OutputFile);
            out << outputJson.dump(4);
        }
    } else if (getRestoreMode()) {
        // 恢复模式：从JSON文件读取位置信息并重新定位
        if (InputFile.empty()) {
            llvm::errs() << "错误：未指定输入JSON文件\n";
            return 1;
        }

        // 读取JSON文件
        std::ifstream input(InputFile);
        input >> inputJson;

        // 遍历JSON文件中的每个位置信息
        for (const auto &item : inputJson) {
            // 构建匹配器，根据函数名和位置信息定位代码
            auto matcher = arraySubscriptExpr(
                               // 匹配函数名
                               hasAncestor(functionDecl(hasName(item["function"].get<std::string>()))),
                               // 匹配for循环
                               hasAncestor(forStmt().bind("forLoop")))
                               .bind("arrayAccess");

            // 创建回调并添加匹配器
            ArrayMatcher Callback(outputJson);
            Finder.addMatcher(matcher, &Callback);
        }

        if (Tool.run(createCustomFrontendActionFactory().get()) != 0)
            return 1;

        // 输出恢复的位置信息
        if (!OutputFile.empty()) {
            std::ofstream out(OutputFile);
            out << outputJson.dump(4);
        }
    } else if (getModifyMode()) {
        // 检查输入文件
        if (InputFile.empty()) {
            llvm::errs() << "错误：修改模式需要指定输入JSON文件\n";
            return 1;
        }

        // 读取JSON文件
        std::ifstream input(InputFile);
        try {
            input >> inputJson;
        } catch (const json::exception &e) {
            llvm::errs() << "JSON文件解析错误: " << e.what() << "\n";
            return 1;
        }

        // 运行工具
        if (Tool.run(createCustomFrontendActionFactory().get()) != 0)
            return 1;

        // 输出修改后的代码
        if (!OutputFile.empty()) {
            std::error_code EC;
            llvm::raw_fd_ostream OS(OutputFile, EC);
            if (EC) {
                llvm::errs() << "打开输出文件错误: " << EC.message() << "\n";
                return 1;
            }

            const RewriteBuffer *RewriteBuf =
                TheRewriter.getRewriteBufferFor(TheRewriter.getSourceMgr().getMainFileID());
            if (!RewriteBuf) {
                llvm::errs() << "没有对源文件进行修改。\n";
                return 1;
            }

            // 保留原有的头文件和特殊属性
            OS << "#include <compiler/m3000.h>\n";
            OS << "#include \"hthread_device.h\"\n\n";
            OS << std::string(RewriteBuf->begin(), RewriteBuf->end());
        }
    } else {
        llvm::errs() << "错误：请指定运行模式（-locate、-restore 或 -modify）\n";
        return 1;
    }

    return 0;
}