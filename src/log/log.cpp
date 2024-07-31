#include "log.hpp"

#include <chrono>
#include <iostream>

rvm::log::LogCategory rvm::log::LogLevel = rvm::log::LogCategory::ERROR;

auto ProgramStart = std::chrono::high_resolution_clock::now();
using namespace std::chrono;

namespace {
    void Log(const std::string& msg, rvm::log::LogCategory category) {
        if (category < rvm::log::LogLevel) return;

        auto currentTime = std::chrono::high_resolution_clock::now();
        auto programDuration = std::chrono::duration_cast<microseconds>(currentTime - ProgramStart).count();

        std::cout << "[T+" << programDuration << "us]\t"; 

        using enum rvm::log::LogCategory;
        switch (category) {
            case INFO:
                std::cout << "[INFO] ";
                break;
            case WARNING:
                std::cout << "[WARNING] ";
                break;
            case ERROR:
                std::cout << "[ERROR] ";
                break;
            case ALL:
                break;
        }
        std::cout << msg << "\n";
    }
}

void rvm::log::LogError(const std::string& msg, int code) {
    Log(msg, LogCategory::ERROR);
    std::exit(code);
}

void rvm::log::LogWarning(const std::string &msg) {
    Log(msg, LogCategory::WARNING);
}

void rvm::log::LogInfo(const std::string &msg) {
    Log(msg, LogCategory::INFO);
}