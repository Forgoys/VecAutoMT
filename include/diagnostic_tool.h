#ifndef DIAGNOSTIC_TOOL_H
#define DIAGNOSTIC_TOOL_H

#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/DiagnosticOptions.h>
#include <clang/Basic/DiagnosticIDs.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/raw_ostream.h>
#include <string>
#include <vector>
#include <memory>
#include <utility>

class MyDiagnosticConsumer : public clang::DiagnosticConsumer {
public:
    // 错误信息的结构体
    struct DiagnosticInfo {
        std::string Message;        // 错误信息
        std::string Filename;       // 文件名
        unsigned Line;              // 行号
        unsigned Column;            // 列号
        std::string Severity;       // 严重程度
        std::string Category;       // 错误类别
        std::string CodeSnippet;    // 相关代码片段
        std::string CaretLine;      // 错误位置标记行
        std::vector<std::string> Notes; // 相关的注释信息
    };

    MyDiagnosticConsumer(bool logToFile = false,
                        const std::string& logFilename = "clang_diagnostics.log");

    void HandleDiagnostic(clang::DiagnosticsEngine::Level DiagLevel,
                         const clang::Diagnostic& Info) override;

    // 获取统计信息
    void PrintStatistics() const;

    // 清除所有诊断信息
    void Clear();

    // 获取所有诊断信息
    const std::vector<DiagnosticInfo>& GetDiagnostics() const;

    // 输出诊断信息
    void OutputDiagnostic(const DiagnosticInfo& Info);

private:
    bool LogToFile;
    std::string LogFilename;
    std::shared_ptr<clang::DiagnosticOptions> DiagOpts;
    std::vector<DiagnosticInfo> Diagnostics;
    unsigned ErrorCount;
    unsigned WarningCount;

    // 获取源代码片段和错误标记
    std::pair<std::string, std::string> GetSourceSnippetAndCaret(
        const clang::FullSourceLoc& Loc,
        const clang::Diagnostic& Info,
        unsigned Context);

    // 格式化诊断信息
    std::string FormatDiagnostic(const DiagnosticInfo& Info);

    // 辅助函数：获取行内容
    std::string GetLineContent(const clang::SourceManager& SM,
                             clang::SourceLocation Loc) const;
};

#endif // DIAGNOSTIC_TOOL_H