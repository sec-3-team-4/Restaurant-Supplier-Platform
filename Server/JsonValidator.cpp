#include "JsonValidator.h"

bool JsonValidator::isValid(std::string msg) {
    if (msg.empty()) return false;

    if (msg.find("{") == std::string::npos) return false;
    if (msg.find("}") == std::string::npos) return false;
    if (msg.find("\"type\"") == std::string::npos) return false;

    return true;
}
