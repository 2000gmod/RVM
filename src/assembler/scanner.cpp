#include "scanner.hpp"
#include "token.hpp"

#include "../log/log.hpp"
#include <cstdint>
#include <string>

using rvm::assembler::Scanner;
using namespace std::literals;

Scanner::Scanner(const std::string& src) : src(src) {
    keywords = {
        {"function", TokenType::FUNCTION},
        {"label", TokenType::LABEL}
    };

    using enum exec::OpCode;
    names = {
        {"nop", NOP},
        {"halt", HALT},
        {"load", LOAD},
        {"loadconst", LOADCONST},
        {"storeconst", STORECONST},
        {"convert", CONVERT},
        {"add", ADD},
        {"sub", SUB},
        {"mul", MUL},
        {"div", DIV},
        {"land", LAND},
        {"lor", LOR},
        {"lnot", LNOT},
        {"gt", GT},
        {"geq", GEQ},
        {"lt", LT},
        {"leq", LEQ},
        {"eq", EQ},
        {"noteq", NOTEQ},
        {"band", BAND},
        {"bor", BOR},
        {"bxor", BXOR},
        {"bnot", BNOT},
        {"lshift", LSHIFT},
        {"rshift", RSHIFT},
        {"jmp", JMP},
        {"jmpif", JMPIF},
        {"createlocals", CREATELOCALS},
        {"call", CALL},
        {"ret", RET}
    };

    using enum exec::DataType;
    dataTypes = {
        {"i8", I8},
        {"i16", I16},
        {"i32", I32},
        {"i64", I64},
        {"f32", F32},
        {"f64", F64},
        {"ptr", PTR},
    };
}

std::vector<rvm::assembler::Token> Scanner::Tokenize() {
    while (!AtEnd()) {
        start = current;
        ScanToken();
    }
    return tokens;
}

void Scanner::ScanToken() {
    char c = Advance();

    switch (c) {
        case '{':
            AddToken(TokenType::LEFT_BRACE);
            break;
        case '}':
            AddToken(TokenType::RIGHT_BRACE);
            break;
        case '@':
            HandleTypeSpecifier();
            break;
        case '!':
            HandleDataLiteral();
            break;
        case '[':
            HandleEmbededData();
            break;
        case '$':
            HandleStringLiteral();
            break;
        case ' ':
        case '\n':
        case '\t':
            break;
        default:
            if (IsAlpha(c)) ScanName();
            else throw c;
    }
}

void Scanner::AddToken(const TokenType& tok) {
    tokens.push_back({.type = tok, .data = {}});
}

void Scanner::AddToken(const Token& tok) {
    tokens.push_back(tok);
}

char Scanner::Advance() {
    return src[current++];
}

bool Scanner::AtEnd() {
    return current >= (int) src.length();
}

char Scanner::Peek() {
    if (AtEnd()) return '\0';
    return src[current];
}

bool Scanner::IsAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Scanner::IsDigit(char c) {
    return c >= '0' && c <= '9';
}

bool Scanner::IsAlphaNumeric(char c) {
    return IsAlpha(c) || IsDigit(c);
}

void Scanner::HandleTypeSpecifier() {
    while (IsAlphaNumeric(Peek())) Advance();
    std::string specifier = src.substr(start, current - start);
    specifier = specifier.substr(1);

    if (!dataTypes.contains(specifier)) {
        rvm::log::LogError("Unknown type specifier: "s + specifier + ".");
    }

    AddToken(Token {
        .type = TokenType::EMBED_TYPE,
        .data = {
            .embedType = dataTypes.at(specifier)
        }
    });
}

void Scanner::HandleDataLiteral() {
    while (IsAlphaNumeric(Peek())) Advance();
    std::string specifier = src.substr(start, current - start);
    specifier = specifier.substr(1);
    

    if (!dataTypes.contains(specifier)) {
        rvm::log::LogError("Unknown literal data type: "s + specifier + ".");
    }

    auto type = dataTypes.at(specifier);

    while (!IsAlphaNumeric(Peek())) Advance();
    start = current;

    while (IsAlphaNumeric(Peek())) Advance();
    std::string num = src.substr(start, current - start);

    Token out;
    out.type = TokenType::DATA_LITERAL;

    using rvm::exec::DataType;
    using rvm::exec::VMValue;

    auto outOfRange = [] {
        rvm::log::LogError("Literal out of range.");
    };

    try {
        if (type == DataType::I8) {
            auto data = std::stoll(num, nullptr, 0);
            if (data > INT8_MAX || data < INT8_MIN) outOfRange();
            out.data.dataLiteral = VMValue((int8_t) data);
        }
        else if (type == DataType::I16) {
            auto data = std::stoll(num, nullptr, 0);
            if (data > INT16_MAX || data < INT16_MIN) outOfRange();
            out.data.dataLiteral = VMValue((int16_t) data);
        }
        else if (type == DataType::I32) {
            auto data = std::stoll(num, nullptr, 0);
            if (data > INT32_MAX || data < INT32_MIN) outOfRange();
            out.data.dataLiteral = VMValue((int32_t) data);
        }
        else if (type == DataType::I64 || type == DataType::PTR) {
            auto data = std::stoll(num, nullptr, 0);
            if (data > INT64_MAX || data < INT64_MIN) outOfRange();
            out.data.dataLiteral = VMValue((int64_t) data);
        }
        else if (type == DataType::F32) {
            auto data = std::stof(num, nullptr);
            out.data.dataLiteral = VMValue(data);
        }
        else if (type == DataType::F64) {
            auto data = std::stod(num, nullptr);
            out.data.dataLiteral = VMValue(data);
        }
    }
    catch (...) {
        rvm::log::LogError("Error while parsing number literal.");
    }

    AddToken(out);
}

void Scanner::HandleEmbededData() {
    std::string num;
    while (Peek() != ']') num += Advance();
    Advance();

    Token out;
    out.type = TokenType::EMBED_DATA;

    auto data = std::stoi(num, nullptr, 0);
    out.data.embedData = data;
    AddToken(out);
}

void Scanner::HandleStringLiteral() {
    std::string str;

    if (Advance() != '"') rvm::log::LogError("Expected string literal.");
    while (Peek() != '"') str += Advance();
    Advance();

    AddToken(Token {
        .type = TokenType::STRING_LITERAL,
        .data = {
            .stringLiteral = str
        }
    });
}

void Scanner::ScanName() {
    while (IsAlphaNumeric(Peek())) Advance();
    std::string text = src.substr(start, current - start);

    Token out;

    if (keywords.contains(text)) {
        out.type = keywords.at(text);
    }
    else if (names.contains(text)) {
        out.type = TokenType::INSTRUCTION;
        out.data.instruction = names.at(text);
    }
    else {
        out.type = TokenType::NAME;
        out.data.nameString = text;
    }
    AddToken(out);
}