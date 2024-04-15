#pragma once

#include <string>
#include "../exec/instruction.hpp"

namespace rvm::loading {
    struct FunctionUnit {
        std::string name;
        std::vector<exec::InstructionUnit> code;
    };
}