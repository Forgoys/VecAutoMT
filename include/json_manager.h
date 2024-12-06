#ifndef JSON_MANAGER_H
#define JSON_MANAGER_H

#include "array_matcher.h"
#include "command_line_options.h"
#include "util.h"
#include <string>
#include <fstream>
#include <stdexcept>

class JsonManager {
public:
    static JsonManager& instance();

    void readInputJson(const std::string& filename);
    void writeOutputJson(const std::string& filename);

    json& getInputJson();
    json& getOutputJson();

    void clearOutputJson();
    void addArrayAccess(const ArrayAccessInfo& info);

private:
    JsonManager() = default;

    json inputJson = json::array();
    json outputJson = json::array();

    void validateInputJson() const;
    // 处理locate模式
    void handleLocateMode(const ArrayAccessInfo& info);

    // 处理restore模式
    void handleRestoreMode(const ArrayAccessInfo& info);

    // 处理modify模式
    void handleModifyMode(const ArrayAccessInfo& info) {};
};

#endif // JSON_MANAGER_H