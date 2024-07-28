#pragma once

#include <string>
#include "../exec/instruction.hpp"

namespace rvm::loading {
    struct GlobalDataUnit {
        std::string name;
        std::vector<exec::InstructionUnit> dataVector;
    };

    std::string Serialize(const GlobalDataUnit& unit);
    std::string Serialize(const std::vector<GlobalDataUnit>& units);
    std::vector<GlobalDataUnit> Deserialize(const std::string& code);
}