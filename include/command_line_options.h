#ifndef COMMAND_LINE_OPTIONS_H
#define COMMAND_LINE_OPTIONS_H

#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include <string>

class CommandLineOptions {
public:
    static CommandLineOptions& instance();

    bool getLocateMode() const;
    bool getRestoreMode() const;
    bool getModifyMode() const;
    std::string getInputFile() const;
    std::string getOutputFile() const;

    void initialize(int argc, const char** argv);
    clang::tooling::CommonOptionsParser& getOptionsParser();
    const clang::tooling::CommonOptionsParser& getOptionsParser() const;

    [[nodiscard]] std::string toString() const {
        return "CommandLineOptions {\n"
               "  LocateMode: " + std::string(getLocateMode() ? "true" : "false") + ",\n"
               "  RestoreMode: " + std::string(getRestoreMode() ? "true" : "false") + ",\n"
               "  ModifyMode: " + std::string(getModifyMode() ? "true" : "false") + ",\n"
               "  InputFile: '" + getInputFile() + "',\n"
               "  OutputFile: '" + getOutputFile() + "'\n"
               "}";
    }
    
private:
    CommandLineOptions() = default;
    static void registerOptions();

    static llvm::cl::OptionCategory ToolCategory;
    static llvm::cl::opt<bool> LocateMode;
    static llvm::cl::opt<bool> RestoreMode;
    static llvm::cl::opt<bool> ModifyMode;
    static llvm::cl::opt<std::string> InputFile;
    static llvm::cl::opt<std::string> OutputFile;

    std::unique_ptr<clang::tooling::CommonOptionsParser> OptionsParser;

};

#endif // COMMAND_LINE_OPTIONS_H