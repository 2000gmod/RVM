#pragma once

#include "token.hpp"
#include <unordered_map>
#include <vector>

namespace rvm::assembler {
    class Scanner {
    private:
        std::vector<Token> tokens;
        std::unordered_map<std::string, TokenType> keywords;
        std::unordered_map<std::string, exec::OpCode> names;
        std::unordered_map<std::string, exec::DataType> dataTypes;

        int start = 0, current = 0, line = 1;
        std::string src;
    public:
        explicit Scanner(const std::string& src);
        std::vector<Token> Tokenize();
    private:
        void ScanToken();
        bool Match(char expected);
        char Peek();
        char PeekNext();
        static bool IsAlpha(char c);
        static bool IsDigit(char c);
        static bool IsAlphaNumeric(char c);
        char Advance();
        bool AtEnd();
        void AddToken(const Token& tok);
        void AddToken(const TokenType& tok);
        void ScanName();

        void HandleTypeSpecifier();
        void HandleDataLiteral();
        void HandleEmbededData();
        void HandleStringLiteral();

    };
}