#pragma once

#include <string>
#include "../exec/instruction.hpp"

namespace rvm::assembler {
    enum class TokenType {
        FUNCTION,
        GLOBAL,
        LABEL,
        NAME,
        INSTRUCTION,
        EMBED_TYPE,
        EMBED_DATA,
        DATA_LITERAL,
        STRING_LITERAL,
        LEFT_BRACE,
        RIGHT_BRACE
    };

    struct Token {
        TokenType type;

        struct {
            std::string nameString = "";
            exec::OpCode instruction = exec::OpCode::NOP;
            exec::DataType embedType = exec::DataType::NONE;
            int32_t embedData = 0;
            
            exec::VMValue dataLiteral = exec::VMValue(0);
            std::string stringLiteral = "";
        } data;
    };
}