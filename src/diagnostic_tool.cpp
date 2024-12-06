#include "diagnostic_tool.h"

#include <iomanip>
#include <sstream>
#include <clang/Basic/SourceManager.h>
#include <clang/Basic/DiagnosticCategories.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Format.h>

MyDiagnosticConsumer::MyDiagnosticConsumer(bool logToFile,
                                           const std::string &logFilename)
    : LogToFile(logToFile),
      LogFilename(logFilename),
      ErrorCount(0),
      WarningCount(0)
{
    DiagOpts = std::make_shared<clang::DiagnosticOptions>();
    DiagOpts->ShowColors = true;
    DiagOpts->ShowCarets = true;
    DiagOpts->ShowFixits = true;
    DiagOpts->TabStop = 4; // 设置制表符宽度
    DiagOpts->MessageLength = 120; // 设置消息最大长度

    // 清空日志文件并写入头部信息
    if (LogToFile) {
        std::error_code EC;
        llvm::raw_fd_ostream OS(LogFilename, EC, llvm::sys::fs::OF_None);
        if (EC) {
            llvm::errs() << "Error opening log file: " << EC.message() << "\n";
        } else {
            // 获取当前时间并格式化
            auto now = std::chrono::system_clock::now();
            auto in_time_t = std::chrono::system_clock::to_time_t(now);
            std::stringstream ss;
            ss << "=== Diagnostic Log ===\n"
                    << "Time: " << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d, %H:%M:%S") << "\n"
                    << "==================\n\n";

            OS << ss.str();
        }
    }
}

void MyDiagnosticConsumer::HandleDiagnostic(clang::DiagnosticsEngine::Level DiagLevel,
                                            const clang::Diagnostic &Info)
{
    DiagnosticInfo diagInfo;

    // 获取位置信息
    if (Info.getLocation().isValid()) {
        clang::FullSourceLoc FullLoc(Info.getLocation(), Info.getSourceManager());
        diagInfo.Filename = FullLoc.getManager().getFilename(FullLoc).str();
        diagInfo.Line = FullLoc.getSpellingLineNumber();
        diagInfo.Column = FullLoc.getSpellingColumnNumber();

        // 获取代码片段和错误位置标记
        std::tie(diagInfo.CodeSnippet, diagInfo.CaretLine) =
                GetSourceSnippetAndCaret(FullLoc, Info, 2);
    }

    // 获取错误信息
    llvm::SmallVector<char, 100> MessageStr;
    Info.FormatDiagnostic(MessageStr);
    diagInfo.Message = std::string(MessageStr.begin(), MessageStr.end());

    // 设置严重程度
    switch (DiagLevel) {
        case clang::DiagnosticsEngine::Fatal:
            diagInfo.Severity = "Fatal Error";
            ErrorCount++;
            break;
        case clang::DiagnosticsEngine::Error:
            diagInfo.Severity = "Error";
            ErrorCount++;
            break;
        case clang::DiagnosticsEngine::Warning:
            diagInfo.Severity = "Warning";
            WarningCount++;
            break;
        case clang::DiagnosticsEngine::Note:
            diagInfo.Severity = "Note";
            break;
        case clang::DiagnosticsEngine::Remark:
            diagInfo.Severity = "Remark";
            break;
        default:
            diagInfo.Severity = "Other";
    }

    // 获取诊断类别
    unsigned DiagID = Info.getID();
    llvm::StringRef Category = clang::DiagnosticIDs::getCategoryNameFromID(DiagID);
    if (!Category.empty()) {
        diagInfo.Category = Category.str();
    }

    // 存储诊断信息
    Diagnostics.push_back(diagInfo);

    // 输出到日志文件或标准错误流
    OutputDiagnostic(diagInfo);
}

std::string MyDiagnosticConsumer::GetLineContent(const clang::SourceManager &SM,
                                                 clang::SourceLocation Loc) const
{
    if (!Loc.isValid())
        return "";

    unsigned FileOffset = SM.getFileOffset(Loc);
    const char *BufferStart = SM.getCharacterData(Loc);
    if (!BufferStart)
        return "";

    const char *LineStart = BufferStart;
    llvm::Expected<llvm::StringRef> Content = SM.getBufferData(SM.getFileID(Loc));
    if (!Content) {
        return "";
    }
    llvm::StringRef BufferStr = *Content;
    while (LineStart > BufferStr.begin() && LineStart[-1] != '\n')
        --LineStart;

    const char *LineEnd = BufferStart;
    while (*LineEnd && *LineEnd != '\n')
        ++LineEnd;

    return std::string(LineStart, LineEnd);
}

std::pair<std::string, std::string> MyDiagnosticConsumer::GetSourceSnippetAndCaret(
    const clang::FullSourceLoc &Loc,
    const clang::Diagnostic &Info,
    unsigned Context)
{
    if (!Loc.isValid())
        return {"", ""};

    const clang::SourceManager &SM = Loc.getManager();
    unsigned LineNo = Loc.getSpellingLineNumber();
    unsigned ColNo = Loc.getSpellingColumnNumber();
    clang::FileID FID = SM.getFileID(Loc);

    std::string Snippet;
    std::string CaretLine;

    if (FID.isValid()) {
        for (unsigned i = (LineNo > Context ? LineNo - Context : 1);
             i <= LineNo + Context; ++i) {
            clang::SourceLocation StartOfLine = SM.translateLineCol(FID, i, 1);
            if (StartOfLine.isValid()) {
                bool IsErrorLine = (i == LineNo);
                std::string LineContent = GetLineContent(SM, StartOfLine);

                // 添加行号和内容
                std::string LinePrefix = std::to_string(i) + " | ";
                Snippet += LinePrefix + LineContent + "\n";

                // 为错误行生成插入符号行
                if (IsErrorLine) {
                    // 计算前缀空格
                    std::string Spaces(LinePrefix.length() - 2, ' ');
                    CaretLine = Spaces + "| " + std::string(ColNo - 1, ' ') + "^";

                    // 如果有错误跨度，添加波浪线
                    clang::SourceLocation EndLoc = Info.getLocation();
                    if (EndLoc.isValid()) {
                        unsigned EndCol = SM.getSpellingColumnNumber(EndLoc);
                        if (EndCol > ColNo) {
                            CaretLine += std::string(EndCol - ColNo - 1, '~');
                        }
                    }
                    CaretLine += "\n";
                }
            }
        }
    }
    return {Snippet, CaretLine};
}

void MyDiagnosticConsumer::OutputDiagnostic(const DiagnosticInfo &Info)
{
    std::string Output = FormatDiagnostic(Info);

    if (LogToFile) {
        std::error_code EC;
        llvm::raw_fd_ostream OS(LogFilename, EC,
                                llvm::sys::fs::OpenFlags::OF_Append);
        if (!EC) {
            OS << Output;
        }
    } else {
        llvm::errs() << Output;
    }
}

std::string MyDiagnosticConsumer::FormatDiagnostic(const DiagnosticInfo &Info)
{
    std::string Output;
    llvm::raw_string_ostream OS(Output);

    // 使用更清晰的分隔符
    OS << "\n" << std::string(80, '-') << "\n";

    // 添加严重程度和类别
    OS << Info.Severity;
    if (!Info.Category.empty()) {
        OS << " [" << Info.Category << "]";
    }
    OS << "\n\n";

    // 添加位置信息
    if (!Info.Filename.empty()) {
        OS << "In file: " << Info.Filename << ":" << Info.Line << ":"
                << Info.Column << "\n\n";
    }

    // 添加错误消息
    OS << "Message: " << Info.Message << "\n\n";

    // 添加代码片段和错误位置标记
    if (!Info.CodeSnippet.empty()) {
        OS << "Relevant Code:\n" << Info.CodeSnippet;
        if (!Info.CaretLine.empty()) {
            OS << Info.CaretLine;
        }
        OS << "\n";
    }

    // 添加注释信息
    if (!Info.Notes.empty()) {
        OS << "Additional Notes:\n";
        for (const auto &Note: Info.Notes) {
            OS << "  • " << Note << "\n";
        }
        OS << "\n";
    }

    OS << std::string(80, '-') << "\n";
    return OS.str();
}

void MyDiagnosticConsumer::Clear()
{
    Diagnostics.clear();
    ErrorCount = 0;
    WarningCount = 0;
}

const std::vector<MyDiagnosticConsumer::DiagnosticInfo> &
MyDiagnosticConsumer::GetDiagnostics() const
{
    return Diagnostics;
}

void MyDiagnosticConsumer::PrintStatistics() const
{
    std::string stats = "\nDiagnostic Statistics:\n" +
                        std::string(20, '=') + "\n" +
                        "Total Errors  : " + std::to_string(ErrorCount) + "\n" +
                        "Total Warnings: " + std::to_string(WarningCount) + "\n" +
                        std::string(20, '=') + "\n";

    if (LogToFile) {
        std::error_code EC;
        llvm::raw_fd_ostream OS(LogFilename, EC,
                                llvm::sys::fs::OpenFlags::OF_Append);
        if (!EC) {
            OS << stats;
        }
    }
    llvm::errs() << stats;
}
