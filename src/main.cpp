#include "diagnostic_tool.h"
#include "code_modifier.h"
#include "frontend_action.h"
#include "command_line_options.h"
#include "json_manager.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include <iostream>
#include <memory>

using namespace clang::tooling;

// Global rewriter for modify mode
clang::Rewriter TheRewriter;


int main(int argc, const char **argv)
{
    try {
        // Initialize command line options
        CommandLineOptions::instance().initialize(argc, argv);
        auto &options = CommandLineOptions::instance();
        llvm::outs() << options.toString();
        // Get non-const reference to CommonOptionsParser
        auto &optionsParser = options.getOptionsParser();

        // Create compilation database and source paths
        auto &compilations = optionsParser.getCompilations();
        auto sourcePaths = optionsParser.getSourcePathList();

        // Create ClangTool
        ClangTool Tool(compilations, sourcePaths);

        auto &jsonManager = JsonManager::instance();

        // 创建详细诊断消费者实例
        auto DiagConsumer = std::make_unique<MyDiagnosticConsumer>(
            true,  // 启用文件日志
            "test/tool_diagnostics.log"  // 日志文件名
        );

        // 获取诊断消费者的原始指针（ClangTool 将接管所有权）
        auto* diagConsumerPtr = DiagConsumer.release();

        // 设置诊断选项
        Tool.setDiagnosticConsumer(diagConsumerPtr);
        if (options.getLocateMode()) {
            // Run the tool with custom frontend action
            if (Tool.run(createCustomFrontendActionFactory().get()) != 0) {
                llvm::errs() << "Error: Some errors happened in the locate mode.\n";
                return 1;
            }

            // Write output if specified
            if (!options.getOutputFile().empty()) {
                jsonManager.writeOutputJson(options.getOutputFile());
            }
        } else if (options.getRestoreMode()) {
            if (options.getInputFile().empty()) {
                llvm::errs() << "Error: No input JSON file specified\n";
                return 1;
            }

            // Read input JSON
            jsonManager.readInputJson(options.getInputFile());

            // Run the tool
            Tool.run(createCustomFrontendActionFactory().get());
            diagConsumerPtr->PrintStatistics();
            const auto& diagnostics = diagConsumerPtr->GetDiagnostics();
            // for (const auto& diag : diagnostics) {
            //     diagConsumerPtr->OutputDiagnostic(diag);
            // }

            // Write output if specified
            if (!options.getOutputFile().empty()) {
                jsonManager.writeOutputJson(options.getOutputFile());
            }
        } else if (options.getModifyMode()) {
            if (options.getInputFile().empty()) {
                llvm::errs() << "Error: Modify mode requires input JSON file\n";
                return 1;
            }

            // Read input JSON
            jsonManager.readInputJson(options.getInputFile());

            // Run the tool
            if (Tool.run(createCustomFrontendActionFactory().get()) != 0) {
                llvm::errs() << "Error: Some errors happened in the modify mode.\n";
                return 1;
            }

            // Write modified code
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

                // Preserve headers and special attributes
                OS << "#include <compiler/m3000.h>\n";
                OS << "#include \"hthread_device.h\"\n\n";
                OS << std::string(RewriteBuf->begin(), RewriteBuf->end());
            }
        } else {
            llvm::errs() << "Error: Please specify run mode (-locate, -restore, or -modify)\n";
            return 1;
        }

        return 0;
    } catch (const std::exception &e) {
        llvm::errs() << "Error: " << e.what() << "\n";
        return 1;
    }
}
