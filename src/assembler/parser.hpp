#pragma once

#include "token.hpp"
#include "../loading/loading.hpp"
#include <vector>

namespace rvm::assembler {
    class Parser {
    private:
        std::vector<Token> tokens;
        int current = 0;

        std::vector<loading::GlobalDataUnit> globals;
    public:
        Parser(const std::string& src);
        std::vector<loading::GlobalDataUnit> Parse();

        static std::vector<loading::GlobalDataUnit> FromFile(const std::string& path);
    private:
        Token& Consume(TokenType type, const std::string& msg);
        bool IsAtEnd();

        Token& Peek();
        Token& Advance();
        Token& Previous();

        bool Match(TokenType type);
        bool Check(TokenType type);

        void ParseFunction();
        void ParseGlobal();

    };
}