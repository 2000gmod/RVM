#pragma once

#include <string>

namespace rvm::log {
    void LogError(const std::string& msg, int code = 1);
}