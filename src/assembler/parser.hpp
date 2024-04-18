#pragma once

#include "token.hpp"
#include "../loading/loading.hpp"

namespace rvm::assembler {
    class Parser {
    private:
        std::vector<Token> tokens;
        int current = 0;
    public:
        Parser(const std::string& src);
        std::vector<loading::FunctionUnit> Parse();

    };
}