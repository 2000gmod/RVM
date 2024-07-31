#pragma once

#include <string>

namespace rvm::log {
    enum class LogCategory {
        ALL,
        INFO,
        WARNING,
        ERROR
    };

    extern LogCategory LogLevel;

    void LogError(const std::string& msg, int code = 1);
    void LogWarning(const std::string& msg);
    void LogInfo(const std::string& msg);
}