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
    DiagOpts->ShowColors = !LogToFile; // 只在终端输出时启用颜色
    DiagOpts->ShowCarets = true;
    DiagOpts->ShowFixits = true;
    DiagOpts->TabStop = 4;
    DiagOpts->MessageLength = 120;

    if (LogToFile) {
        std::error_code EC;
        llvm::raw_fd_ostream OS(LogFilename, EC, llvm::sys::fs::OF_None);
        if (EC) {
            llvm::errs() << "Error opening log file: " << EC.message() << "\n";
        } else {
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

const char *MyDiagnosticConsumer::GetSeverityColor(clang::DiagnosticsEngine::Level Level) const
{
    if (LogToFile) return ""; // 日志文件中不使用颜色

    switch (Level) {
        case clang::DiagnosticsEngine::Fatal:
        case clang::DiagnosticsEngine::Error:
            return Colors::RED;
        case clang::DiagnosticsEngine::Warning:
            return Colors::YELLOW;
        case clang::DiagnosticsEngine::Note:
            return Colors::CYAN;
        case clang::DiagnosticsEngine::Remark:
            return Colors::GREEN;
        default:
            return Colors::RESET;
    }
}

std::string MyDiagnosticConsumer::Color(const char *ColorCode) const
{
    return LogToFile ? "" : ColorCode; // 日志文件中不使用颜色
}

void MyDiagnosticConsumer::HandleDiagnostic(clang::DiagnosticsEngine::Level DiagLevel,
                                            const clang::Diagnostic &Info)
{
    DiagnosticInfo diagInfo;
    diagInfo.Level = DiagLevel;

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

    // 设置严重程度并更新计数器
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

    // 将诊断信息存入向量
    Diagnostics.push_back(diagInfo);
}

// diagnostic_tool.cpp
void MyDiagnosticConsumer::Finish() {
    std::error_code EC;
    std::unique_ptr<llvm::raw_fd_ostream> OS;
    if (LogToFile) {
        OS = std::make_unique<llvm::raw_fd_ostream>(
            LogFilename, EC, llvm::sys::fs::OpenFlags::OF_Append);
        if (EC) {
            llvm::errs() << "Error opening log file: " << EC.message() << "\n";
            return;
        }
    }

    PrintStatistics();

    for (const auto &diagInfo : Diagnostics) {
        std::string Output = FormatDiagnostic(diagInfo);
        if (LogToFile && OS) {
            *OS << Output;
        } else {
            llvm::errs() << Output;
        }
    }

    Clear();
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
                    std::string Spaces(LinePrefix.length() - 2, ' ');
                    CaretLine = Spaces + "| " + std::string(ColNo - 1, ' ') + "^";

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

// void MyDiagnosticConsumer::OutputDiagnostic(const DiagnosticInfo &Info)
// {
//     std::string Output = FormatDiagnostic(Info);
//
//     if (LogToFile) {
//         std::error_code EC;
//         llvm::raw_fd_ostream OS(LogFilename, EC,
//                                 llvm::sys::fs::OpenFlags::OF_Append);
//         if (!EC) {
//             OS << Output;
//         }
//     } else {
//         llvm::errs() << Output;
//     }
// }

std::string MyDiagnosticConsumer::FormatDiagnostic(const DiagnosticInfo &Info) const
{
    std::string Output;
    llvm::raw_string_ostream OS(Output);

    // 使用条件颜色输出
    const std::string separator(80, '-');
    OS << "\n" << (LogToFile ? separator : Color(Colors::DIM) + separator + Color(Colors::RESET)) << "\n";

    // 严重程度
    if (LogToFile) {
        OS << Info.Severity;
    } else {
        OS << Color(GetSeverityColor(Info.Level)) << Color(Colors::BOLD)
                << Info.Severity << Color(Colors::RESET);
    }

    // 类别
    if (!Info.Category.empty()) {
        if (LogToFile) {
            OS << " [" << Info.Category << "]";
        } else {
            OS << Color(Colors::CYAN) << " [" << Info.Category << "]" << Color(Colors::RESET);
        }
    }
    OS << "\n\n";

    // 文件位置
    if (!Info.Filename.empty()) {
        if (LogToFile) {
            OS << "In file: " << Info.Filename << ":" << Info.Line << ":" << Info.Column << "\n\n";
        } else {
            OS << Color(Colors::BLUE) << "In file: " << Info.Filename << ":"
                    << Info.Line << ":" << Info.Column << Color(Colors::RESET) << "\n\n";
        }
    }

    // 错误消息
    if (LogToFile) {
        OS << "Message: " << Info.Message << "\n\n";
    } else {
        OS << "Message: " << Color(GetSeverityColor(Info.Level))
                << Info.Message << Color(Colors::RESET) << "\n\n";
    }

    // 代码片段
    if (!Info.CodeSnippet.empty()) {
        OS << (LogToFile ? "" : Color(Colors::BOLD))
                << "Relevant Code:\n"
                << (LogToFile ? "" : Color(Colors::RESET));

        std::istringstream iss(Info.CodeSnippet);
        std::string line;
        while (std::getline(iss, line)) {
            size_t separatorPos = line.find('|');
            if (separatorPos != std::string::npos && !LogToFile) {
                // 在终端输出时，行号使用暗色
                OS << Color(Colors::DIM) << line.substr(0, separatorPos + 1)
                        << Color(Colors::RESET) << line.substr(separatorPos + 1) << "\n";
            } else {
                OS << line << "\n";
            }
        }

        if (!Info.CaretLine.empty()) {
            if (LogToFile) {
                OS << Info.CaretLine;
            } else {
                OS << Color(GetSeverityColor(Info.Level)) << Info.CaretLine
                        << Color(Colors::RESET);
            }
        }
        OS << "\n";
    }

    // 注释信息
    if (!Info.Notes.empty()) {
        OS << (LogToFile ? "" : Color(Colors::BOLD)) << "Additional Notes:\n"
                << (LogToFile ? "" : Color(Colors::RESET));
        for (const auto &Note: Info.Notes) {
            if (LogToFile) {
                OS << "  • " << Note << "\n";
            } else {
                OS << Color(Colors::CYAN) << "  • " << Note << Color(Colors::RESET) << "\n";
            }
        }
        OS << "\n";
    }

    OS << (LogToFile ? separator : Color(Colors::DIM) + separator + Color(Colors::RESET)) << "\n";
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

void MyDiagnosticConsumer::PrintStatistics()
{
    std::string stats;
    if (LogToFile) {
        stats = "\nDiagnostic Statistics:\n" +
                std::string(20, '=') + "\n" +
                "Total Errors  : " + std::to_string(ErrorCount) + "\n" +
                "Total Warnings: " + std::to_string(WarningCount) + "\n" +
                std::string(20, '=') + "\n";
    } else {
        stats = Color(Colors::CYAN) + "\nDiagnostic Statistics:\n" +
                std::string(20, '=') + Color(Colors::RESET) + "\n" +
                Color(Colors::RED) + "Total Errors  : " + std::to_string(ErrorCount) +
                Color(Colors::RESET) + "\n" +
                Color(Colors::YELLOW) + "Total Warnings: " + std::to_string(WarningCount) +
                Color(Colors::RESET) + "\n" +
                Color(Colors::CYAN) + std::string(20, '=') + Color(Colors::RESET) + "\n";
    }

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

// void MyDiagnosticConsumer::OutputAllDiagnostics() const
// {
//     for (const auto &diagInfo: Diagnostics) {
//         std::string Output = FormatDiagnostic(diagInfo);
//
//         if (LogToFile) {
//             std::error_code EC;
//             llvm::raw_fd_ostream OS(LogFilename, EC,
//                                     llvm::sys::fs::OpenFlags::OF_Append);
//             if (!EC) {
//                 OS << Output;
//             }
//         } else {
//             llvm::errs() << Output;
//         }
//     }
// }
