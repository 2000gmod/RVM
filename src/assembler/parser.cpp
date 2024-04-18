#include "parser.hpp"
#include "scanner.hpp"

using namespace rvm::assembler;
using namespace rvm::loading;

Parser::Parser(const std::string& src) {
    Scanner scanner(src);
    tokens = scanner.Tokenize();
}

std::vector<FunctionUnit> Parser::Parse() {
    std::vector<FunctionUnit> out;

    

    return out;
}