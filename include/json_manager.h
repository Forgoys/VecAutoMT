#ifndef JSON_MANAGER_H
#define JSON_MANAGER_H

#include "array_matcher.h"
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
};

#endif // JSON_MANAGER_H