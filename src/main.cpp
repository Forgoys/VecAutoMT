#include "diagnostic_tool.h"
#include "code_modifier.h"
#include "frontend_action.h"
#include "command_line_options.h"
#include "json_manager.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include <iostream>
#include <memory>
#include <clang/Frontend/CompilerInstance.h>

using namespace clang::tooling;

// Global rewriter for modify mode
clang::Rewriter TheRewriter;


int main(int argc, const char **argv)
{
    try {
        // 初始化命令行选项
        CommandLineOptions::instance().initialize(argc, argv);
        auto &options = CommandLineOptions::instance();
        llvm::outs() << options.toString();
        // 获取对CommonOptionsParser的非常量引用
        auto &optionsParser = options.getOptionsParser();

        // 创建编译数据库和源文件路径
        auto &compilations = optionsParser.getCompilations();
        auto sourcePaths = optionsParser.getSourcePathList();

        // 创建ClangTool
        ClangTool Tool(compilations, sourcePaths);

        auto &jsonManager = JsonManager::instance();

        // 创建诊断消费者并确保其生命周期
        auto DiagConsumer = std::make_unique<MyDiagnosticConsumer>(
            CommandLineOptions::instance().getDiagnosticLogToFile(),
            CommandLineOptions::instance().getDiagnosticLogFile()
        );

        // 保存原始指针
        MyDiagnosticConsumer* DiagConsumerPtr = DiagConsumer.get();


        if (options.getLocateMode()) {
            // 使用自定义前端操作运行工具
            if (Tool.run(createCustomFrontendActionFactory(DiagConsumerPtr).get()) != 0) {
                llvm::errs() << "Error: Some errors happened in the locate mode.\n";
                return 1;
            }

            // 如果指定，则写入输出
            if (!options.getOutputFile().empty()) {
                jsonManager.writeOutputJson(options.getOutputFile());
            }
        } else if (options.getRestoreMode()) {
            if (options.getInputFile().empty()) {
                llvm::errs() << "Error: No input JSON file specified\n";
                return 1;
            }

            // 读取输入JSON
            jsonManager.readInputJson(options.getInputFile());

            // 运行工具
            Tool.run(createCustomFrontendActionFactory(DiagConsumerPtr).get());

            // 如果指定，则写入输出
            if (!options.getOutputFile().empty()) {
                jsonManager.writeOutputJson(options.getOutputFile());
            }
        } else if (options.getModifyMode()) {
            if (options.getInputFile().empty()) {
                llvm::errs() << "Error: Modify mode requires input JSON file\n";
                return 1;
            }

            // 读取输入JSON
            jsonManager.readInputJson(options.getInputFile());

            // 运行工具
            if (Tool.run(createCustomFrontendActionFactory(DiagConsumerPtr).get()) != 0) {
                llvm::errs() << "Error: Some errors happened in the modify mode.\n";
                return 1;
            }

            // 写入修改后的代码
            if (!options.getOutputFile().empty()) {
                std::error_code EC;
                llvm::raw_fd_ostream OS(options.getOutputFile(), EC);
                if (EC) {
                    llvm::errs() << "Error opening output file: " << EC.message() << "\n";
                    return 1;
                }

                const RewriteBuffer *RewriteBuf =
                        TheRewriter.getRewriteBufferFor(TheRewriter.getSourceMgr().getMainFileID());
                if (!RewriteBuf) {
                    llvm::errs() << "No modifications made to source file.\n";
                    return 1;
                }

                // 保留头文件和特殊属性
                OS << "#include <compiler/m3000.h>\n";
                OS << "#include \"hthread_device.h\"\n\n";
                OS << std::string(RewriteBuf->begin(), RewriteBuf->end());
            }
        } else {
            llvm::errs() << "Error: Please specify run mode (-locate, -restore, or -modify)\n";
            return 1;
        }
        // 在所有处理完成后，输出诊断信息
        DiagConsumer->Finish();

        return 0;
    } catch (const std::exception &e) {
        llvm::errs() << "Error: " << e.what() << "\n";
        return 1;
    }
}
