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

// ANSI Color codes
struct Colors
{
    static constexpr const char *RESET = "\033[0m";
    static constexpr const char *RED = "\033[31m";
    static constexpr const char *GREEN = "\033[32m";
    static constexpr const char *YELLOW = "\033[33m";
    static constexpr const char *BLUE = "\033[34m";
    static constexpr const char *MAGENTA = "\033[35m";
    static constexpr const char *CYAN = "\033[36m";
    static constexpr const char *BOLD = "\033[1m";
    static constexpr const char *DIM = "\033[2m";
};

class MyDiagnosticConsumer : public clang::DiagnosticConsumer
{
public:
    // 错误信息的结构体
    struct DiagnosticInfo
    {
        std::string Message; // 错误信息
        std::string Filename; // 文件名
        unsigned Line; // 行号
        unsigned Column; // 列号
        std::string Severity; // 严重程度
        std::string Category; // 错误类别
        std::string CodeSnippet; // 相关代码片段
        std::string CaretLine; // 错误位置标记行
        std::vector<std::string> Notes; // 相关的注释信息
        clang::DiagnosticsEngine::Level Level; // 保存原始级别用于着色
    };

    MyDiagnosticConsumer(bool logToFile = false,
                         const std::string &logFilename = "clang_diagnostics.log");

    void HandleDiagnostic(clang::DiagnosticsEngine::Level DiagLevel,
                          const clang::Diagnostic &Info) override;

    // 完成所有诊断后的最终输出
    void Finish();

    // 获取统计信息
    void PrintStatistics();

    // 清除所有诊断信息
    void Clear();

    // 获取所有诊断信息
    const std::vector<DiagnosticInfo> &GetDiagnostics() const;

private:
    bool LogToFile;
    std::string LogFilename;
    std::shared_ptr<clang::DiagnosticOptions> DiagOpts;
    std::vector<DiagnosticInfo> Diagnostics;
    unsigned ErrorCount;
    unsigned WarningCount;

    // 获取源代码片段和错误标记
    std::pair<std::string, std::string> GetSourceSnippetAndCaret(
        const clang::FullSourceLoc &Loc,
        const clang::Diagnostic &Info,
        unsigned Context);

    // 格式化诊断信息
    std::string FormatDiagnostic(const DiagnosticInfo &Info) const;

    // 输出诊断信息
    // void OutputDiagnostic(const DiagnosticInfo &Info);

    // 辅助函数：获取行内容
    std::string GetLineContent(const clang::SourceManager &SM,
                               clang::SourceLocation Loc) const;

    // 获取诊断级别对应的颜色
    const char *GetSeverityColor(clang::DiagnosticsEngine::Level Level) const;

    // 根据是否输出到文件返回相应的颜色字符串
    std::string Color(const char *ColorCode) const;

    // 输出所有缓存的诊断信息
    // void OutputAllDiagnostics() const;
};

#endif // DIAGNOSTIC_TOOL_H
