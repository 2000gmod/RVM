#include "loading.hpp"
#include "../exec/instruction.hpp"
#include <string>
#include "../log/log.hpp"

using rvm::loading::GlobalDataUnit;
namespace ldg = rvm::loading;

std::string ldg::Serialize(const GlobalDataUnit& unit) {
    std::string out;
    out += std::to_string(unit.name.size());
    out += (char) 0xFF;
    out += unit.name;

    out += std::to_string(unit.dataVector.size() * sizeof(exec::InstructionUnit));
    out += (char) 0xFF;

    for (auto& data : unit.dataVector) {
        for (int i = 0; i < 8; i++) out += data.data.str[i];
    }

    return out;
}

std::string ldg::Serialize(const std::vector<GlobalDataUnit>& units) {
    std::string out;
    for (auto& unit : units) {
        out += Serialize(unit);
    }
    return out;
}

std::vector<GlobalDataUnit> ldg::Deserialize(const std::string& code) {
    rvm::log::LogInfo("Deserializing file.");
    std::vector<GlobalDataUnit> out;

    unsigned int index = 0;
    while (index < code.size()) {
        std::string numberString, nameString, dataString;
        GlobalDataUnit gdu;

        char c = code.at(index++);
        do {
            numberString += c;
            c = code.at(index++);
        } while (c != (char) 0xFF);

        //index++;

        int length = std::stoi(numberString);
        for (int i = 0; i < length; i++) {
            nameString += code.at(index++);
        }
        //index++;

        numberString = "";
        c = code.at(index++);
        do {
            numberString += c;
            c = code.at(index++);
        } while (c != (char) 0xFF);
        //index++;

        length = std::stoi(numberString);
        for (int i = 0; i < length; i++) {
            dataString += code.at(index++);
        }

        gdu.name = nameString;

        auto dataStream = rvm::exec::InstructionUnit::CreateInstructionDataStream(dataString);
        for (auto &iu : dataStream) {
            gdu.dataVector.push_back(iu);
        }

        out.push_back(gdu);
    }
    rvm::log::LogInfo("Finished deserializing.");
    return out;
}