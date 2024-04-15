#include "instruction.hpp"

using namespace rvm::exec;

std::vector<InstructionUnit> InstructionUnit::CreateInstructionDataStream(const std::string_view& str) {
    std::vector<InstructionUnit> out;
    out.reserve((str.size() / 8) + 1);
    int relpos = 0;

    VMValue data;

    for (auto c : str) {
        if (relpos > 7) {
            out.emplace_back(data);
            relpos = 0;
            data.i64 = 0;
        }
        data.str[relpos] = c;
        relpos++;
    }

    if (relpos != 0) out.emplace_back(data);

    return out;
}