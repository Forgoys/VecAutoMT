// src/json_manager.cpp
#include "json_manager.h"

JsonManager& JsonManager::instance() {
    static JsonManager instance;
    return instance;
}

void JsonManager::readInputJson(const std::string& filename) {
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
    } catch (const json::exception& e) {
        throw std::runtime_error("JSON parsing error: " + std::string(e.what()));
    }
}

void JsonManager::writeOutputJson(const std::string& filename) {
    if (filename.empty()) {
        throw std::runtime_error("No output file specified");
    }

    std::ofstream out(filename);
    if (!out.is_open()) {
        throw std::runtime_error("Failed to open output file: " + filename);
    }

    out << outputJson.dump(4);
}

json& JsonManager::getInputJson() {
    return inputJson;
}

json& JsonManager::getOutputJson() {
    return outputJson;
}

void JsonManager::clearOutputJson() {
    outputJson = json::array();
}

void JsonManager::addArrayAccess(const ArrayAccessInfo& info) {
    // Create array info object
    json arrayInfo = json::object({
        {"name", info.arrayName},
        {"indexVar", info.indexVarName},
        {"line", info.arrayLocation.startLine},
        {"sourceText", info.arrayLocation.sourceText}
    });

    // Find or create file entry
    bool fileFound = false;
    for (auto& fileEntry : outputJson) {
        if (fileEntry["filename"] == info.arrayLocation.filename) {
            fileFound = true;

            // Find or create function entry
            bool functionFound = false;
            for (auto& funcEntry : fileEntry["functions"]) {
                if (funcEntry["name"] == info.arrayLocation.functionName) {
                    functionFound = true;

                    // Find or create loop entry
                    bool loopFound = false;
                    for (auto& loopEntry : funcEntry["forLoops"]) {
                        if (loopEntry["startLine"] == info.forLoopLocation.startLine) {
                            loopFound = true;

                            // Add array access if not exists
                            bool arrayExists = false;
                            for (const auto& arr : loopEntry["arrays"]) {
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
                    {"forLoops", json::array({
                        json::object({
                            {"startLine", info.forLoopLocation.startLine},
                            {"endLine", info.forLoopLocation.endLine},
                            {"sourceText", info.forLoopLocation.sourceText},
                            {"arrays", json::array({arrayInfo})}
                        })
                    })}
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
            {"functions", json::array({
                json::object({
                    {"name", info.arrayLocation.functionName},
                    {"forLoops", json::array({
                        json::object({
                            {"startLine", info.forLoopLocation.startLine},
                            {"endLine", info.forLoopLocation.endLine},
                            {"sourceText", info.forLoopLocation.sourceText},
                            {"arrays", json::array({arrayInfo})}
                        })
                    })}
                })
            })}
        });
        outputJson.push_back(fileEntry);
    }
}

void JsonManager::validateInputJson() const {
    if (!inputJson.is_array()) {
        throw std::runtime_error("Input JSON must be an array");
    }

    // Add more validation as needed
}