#include "log.hpp"

#include <iostream>

void rvm::log::LogError(const std::string& msg, int code) {
    std::cerr << msg << std::endl;
    std::exit(code);
}