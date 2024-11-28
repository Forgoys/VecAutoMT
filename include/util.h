#ifndef UTIL_H
#define UTIL_H

#include "json.hpp"
#include "clang/AST/AST.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <system_error>
#include <vector>

using json = nlohmann::json;

// 工具运行模式
enum class ToolMode
{
    LOCATE,  // 定位模式
    RESTORE, // 恢复模式
    MODIFY   // 修改模式
};

// 位置信息结构
struct LocationInfo
{
    std::string filename;
    std::string functionName;
    unsigned startLine;
    unsigned endLine;
    std::string sourceText;

    // 转换为JSON
    json toJson() const;
    // 从JSON构造
    static LocationInfo fromJson(const json &j);
};

// 工具配置
struct ToolConfig
{
    ToolMode mode;
    std::string inputFile;
    std::string outputFile;
    bool verbose;

    static ToolConfig parseCommandLine(int argc, const char **argv);
};

#endif // UTIL_H