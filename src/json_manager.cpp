#include "json_manager.h"

JsonManager &JsonManager::instance()
{
    static JsonManager instance;
    return instance;
}

void JsonManager::readInputJson(const std::string &filename)
{
    if (filename.empty()) {
        throw std::runtime_error("No input JSON file specified");
    }

    std::ifstream input(filename);
    if (!input.is_open()) {
        throw std::runtime_error("Failed to open input JSON file: " + filename);
    }

    try {
        input >> inputJson;
        validateInputJson();
    } catch (const json::exception &e) {
        throw std::runtime_error("JSON parsing error: " + std::string(e.what()));
    }
}

// src/json_manager.cpp
void JsonManager::writeOutputJson(const std::string &filename)
{
    if (filename.empty()) {
        throw std::runtime_error("No output file specified");
    }

    // 确保输出目录存在
    std::size_t found = filename.find_last_of("/\\");
    if (found != std::string::npos) {
        std::string dir = filename.substr(0, found);
        std::system(("mkdir -p " + dir).c_str());
    }

    std::ofstream out(filename);
    if (!out.is_open()) {
        throw std::runtime_error("Failed to open output file: " + filename);
    }

    // 如果 JSON 为空，至少输出一个空数组
    if (outputJson.empty()) {
        outputJson = json::array();
    }

    // 使用4空格缩进美化输出
    out << outputJson.dump(4) << std::endl;

    // 确保文件被正确写入
    out.flush();
    if (!out.good()) {
        throw std::runtime_error("Error occurred while writing to file: " + filename);
    }

    out.close();
    llvm::outs() << "Successfully wrote JSON output to: " << filename << "\n";
}

json &JsonManager::getInputJson()
{
    return inputJson;
}

json &JsonManager::getOutputJson()
{
    return outputJson;
}

void JsonManager::clearOutputJson()
{
    outputJson = json::array();
}

void JsonManager::addArrayAccess(const ArrayAccessInfo &info)
{
    const auto &options = CommandLineOptions::instance();
    if (options.getLocateMode()) {
        handleLocateMode(info);
    } else if (options.getRestoreMode()) {
        handleRestoreMode(info);
    } else {
        handleModifyMode(info);
    }
}

void JsonManager::handleLocateMode(const ArrayAccessInfo &info)
{
    // Create array info object
    json arrayInfo = json::object({
        {"name", info.arrayName},
        {"indexVar", info.indexVarName},
        {"line", info.arrayLocation.startLine},
        {"sourceText", info.arrayLocation.sourceText}
    });

    // Find or create file entry
    bool fileFound = false;
    for (auto &fileEntry: outputJson) {
        if (fileEntry["filename"] == info.arrayLocation.filename) {
            fileFound = true;

            // Find or create function entry
            bool functionFound = false;
            for (auto &funcEntry: fileEntry["functions"]) {
                if (funcEntry["name"] == info.arrayLocation.functionName) {
                    functionFound = true;

                    // Find or create loop entry
                    bool loopFound = false;
                    for (auto &loopEntry: funcEntry["forLoops"]) {
                        if (loopEntry["startLine"] == info.forLoopLocation.startLine) {
                            loopFound = true;

                            // Add array access if not exists
                            bool arrayExists = false;
                            for (const auto &arr: loopEntry["arrays"]) {
                                if (arr["name"] == info.arrayName &&
                                    arr["line"] == info.arrayLocation.startLine) {
                                    arrayExists = true;
                                    break;
                                }
                            }

                            if (!arrayExists) {
                                loopEntry["arrays"].push_back(arrayInfo);
                            }
                            break;
                        }
                    }

                    if (!loopFound) {
                        // Create new loop entry
                        json loopEntry = json::object({
                            {"startLine", info.forLoopLocation.startLine},
                            {"endLine", info.forLoopLocation.endLine},
                            {"sourceText", info.forLoopLocation.sourceText},
                            {"arrays", json::array({arrayInfo})}
                        });
                        funcEntry["forLoops"].push_back(loopEntry);
                    }
                    break;
                }
            }

            if (!functionFound) {
                // Create new function entry
                json funcEntry = json::object({
                    {"name", info.arrayLocation.functionName},
                    {
                        "forLoops", json::array({
                            json::object({
                                {"startLine", info.forLoopLocation.startLine},
                                {"endLine", info.forLoopLocation.endLine},
                                {"sourceText", info.forLoopLocation.sourceText},
                                {"arrays", json::array({arrayInfo})}
                            })
                        })
                    }
                });
                fileEntry["functions"].push_back(funcEntry);
            }
            break;
        }
    }

    if (!fileFound) {
        // Create new file entry
        json fileEntry = json::object({
            {"filename", info.arrayLocation.filename},
            {
                "functions", json::array({
                    json::object({
                        {"name", info.arrayLocation.functionName},
                        {
                            "forLoops", json::array({
                                json::object({
                                    {"startLine", info.forLoopLocation.startLine},
                                    {"endLine", info.forLoopLocation.endLine},
                                    {"sourceText", info.forLoopLocation.sourceText},
                                    {"arrays", json::array({arrayInfo})}
                                })
                            })
                        }
                    })
                })
            }
        });
        outputJson.push_back(fileEntry);
    }
}

void JsonManager::handleRestoreMode(const ArrayAccessInfo &info)
{
    json computeEntry = {
        {"arrayName", info.arrayName},
        {"functionName", info.arrayLocation.functionName},
        {"indexVariable", info.indexVarName},
        {
            "location", {
                {"filename", info.arrayLocation.filename},
                {"startLine", info.arrayLocation.startLine},
                {"endLine", info.arrayLocation.endLine},
                {"sourceText", info.arrayLocation.sourceText}
            }
        },
        {
            "loop", {
                {"startLine", info.forLoopLocation.startLine},
                {"endLine", info.forLoopLocation.endLine},
                {"sourceText", info.forLoopLocation.sourceText},
                {
                    "bounds", {
                        {"start", info.loopInfo.start},
                        {"end", info.loopInfo.end},
                        {"step", info.loopInfo.step},
                        {"isConstBound", info.loopInfo.isConstBound}
                    }
                }
            }
        },
        {"computations", json::array()}
    };

    // 添加所有数组计算信息
    for (const auto &compute: info.computeInfos) {
        json compInfo = {
            {
                "type", [&compute]() {
                    switch (compute.opType) {
                        case ArrayOpType::READ: return "read";
                        case ArrayOpType::WRITE: return "write";
                        case ArrayOpType::READ_WRITE: return "read_write";
                        default: return "unknown";
                    }
                }()
            },
            {"expression", compute.expression},
            {
                "location", {
                    {"startLine", compute.location.startLine},
                    {"endLine", compute.location.endLine},
                    {"sourceText", compute.location.sourceText}
                }
            },
            {"dependencies", compute.dependentVars}
        };
        computeEntry["computations"].push_back(compInfo);
    }

    // 更新输出JSON结构
    bool fileExists = false;
    for (auto &file: outputJson) {
        if (file["filename"] == info.arrayLocation.filename) {
            fileExists = true;
            if (!file.contains("arrayAccesses")) {
                file["arrayAccesses"] = json::array();
            }
            file["arrayAccesses"].push_back(computeEntry);
            break;
        }
    }

    if (!fileExists) {
        outputJson.push_back({
            {"filename", info.arrayLocation.filename},
            {"arrayAccesses", json::array({computeEntry})}
        });
    }
}

void JsonManager::validateInputJson() const
{
    if (!inputJson.is_array()) {
        throw std::runtime_error("Input JSON must be an array");
    }

    for (const auto &item: inputJson) {
        if (!item.contains("functions")) {
            throw std::runtime_error("Input JSON item missing required 'functions' field");
        }

        if (!item["functions"].is_array()) {
            throw std::runtime_error("'functions' field must be an array");
        }

        for (const auto &func: item["functions"]) {
            if (!func.contains("name")) {
                throw std::runtime_error("Function object missing required 'name' field");
            }

            if (!func["name"].is_string()) {
                throw std::runtime_error("Function 'name' must be a string");
            }
        }
    }
}
