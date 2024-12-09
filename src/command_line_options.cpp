#include "command_line_options.h"
#include <filesystem>

// Static member initialization
llvm::cl::OptionCategory CommandLineOptions::ToolCategory("Array Analysis Tool Options");

llvm::cl::opt<bool> CommandLineOptions::LocateMode("locate",
    llvm::cl::desc("Enable locate mode"),
    llvm::cl::cat(ToolCategory));

llvm::cl::opt<bool> CommandLineOptions::RestoreMode("restore",
    llvm::cl::desc("Enable restore mode"),
    llvm::cl::cat(ToolCategory));

llvm::cl::opt<bool> CommandLineOptions::ModifyMode("modify",
    llvm::cl::desc("Enable modify mode"),
    llvm::cl::cat(ToolCategory));

// 新增：诊断日志选项
llvm::cl::opt<bool> CommandLineOptions::DiagnosticLogToFile("log-to-file",
    llvm::cl::desc("Write diagnostic messages to file instead of terminal"),
    llvm::cl::cat(ToolCategory));

llvm::cl::opt<std::string> CommandLineOptions::DiagnosticLogFile("log-file",
    llvm::cl::desc("Specify diagnostic log file path (default: test/tool_diagnostics.log)"),
    llvm::cl::init("test/tool_diagnostics.log"),
    llvm::cl::value_desc("filename"),
    llvm::cl::cat(ToolCategory));

llvm::cl::opt<std::string> CommandLineOptions::InputFile("input",
    llvm::cl::desc("Specify input JSON file"),
    llvm::cl::value_desc("filename"),
    llvm::cl::cat(ToolCategory));

llvm::cl::opt<std::string> CommandLineOptions::OutputFile("output",
    llvm::cl::desc("Specify output file"),
    llvm::cl::value_desc("filename"),
    llvm::cl::cat(ToolCategory));

CommandLineOptions& CommandLineOptions::instance() {
    static CommandLineOptions instance;
    return instance;
}

void CommandLineOptions::initialize(int argc, const char** argv) {
    auto ExpectedParser = clang::tooling::CommonOptionsParser::create(argc, argv, ToolCategory);
    if (!ExpectedParser) {
        llvm::errs() << ExpectedParser.takeError();
        throw std::runtime_error("Failed to parse command line options");
    }
    OptionsParser = std::make_unique<clang::tooling::CommonOptionsParser>(std::move(ExpectedParser.get()));

    // 如果指定了输出到文件，确保日志文件目录存在
    if (DiagnosticLogToFile) {
        std::filesystem::path logPath(DiagnosticLogFile.getValue());  // 修复：使用 getValue() 获取字符串值
        std::filesystem::create_directories(logPath.parent_path());
    }
}

bool CommandLineOptions::getLocateMode() const {
    return LocateMode;
}

bool CommandLineOptions::getRestoreMode() const {
    return RestoreMode;
}

bool CommandLineOptions::getModifyMode() const {
    return ModifyMode;
}

bool CommandLineOptions::getDiagnosticLogToFile() const {
    return DiagnosticLogToFile;
}

std::string CommandLineOptions::getDiagnosticLogFile() const {
    return DiagnosticLogFile;  // llvm::cl::opt<std::string> 会自动转换为 std::string
}

std::string CommandLineOptions::getInputFile() const {
    return InputFile;
}

std::string CommandLineOptions::getOutputFile() const {
    return OutputFile;
}

clang::tooling::CommonOptionsParser& CommandLineOptions::getOptionsParser() {
    if (!OptionsParser) {
        throw std::runtime_error("OptionsParser not initialized");
    }
    return *OptionsParser;
}

const clang::tooling::CommonOptionsParser& CommandLineOptions::getOptionsParser() const {
    if (!OptionsParser) {
        throw std::runtime_error("OptionsParser not initialized");
    }
    return *OptionsParser;
}

[[nodiscard]] std::string CommandLineOptions::toString() const {
    std::string runMode;
    if (getLocateMode()) runMode += "Locate Mode";
    if (getRestoreMode()) runMode += (runMode.empty() ? "" : ", ") + std::string("Restore Mode");
    if (getModifyMode()) runMode += (runMode.empty() ? "" : ", ") + std::string("Modify Mode");
    if (runMode.empty()) runMode = "No Mode Selected";

    return std::string("\nCommand Line Options:\n") +
           std::string(20, '=') + "\n" +
           "Run Mode: " + runMode + "\n" +
           "InputFile: '" + getInputFile() + "'\n" +
           "OutputFile: '" + getOutputFile() + "'\n" +
           "Diagnostic Output: " + (getDiagnosticLogToFile() ?
               "File ('" + getDiagnosticLogFile() + "')" : "Terminal") + "\n" +
           std::string(20, '=') + "\n\n";
}