// src/command_line_options.cpp
#include "command_line_options.h"

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