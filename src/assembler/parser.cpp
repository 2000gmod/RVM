#include "parser.hpp"
#include "scanner.hpp"
#include "token.hpp"
#include "../log/log.hpp"
#include <cstddef>
#include <fstream>
#include <sstream>
#include <unordered_map>

using namespace std::literals;

using namespace rvm::assembler;
using namespace rvm::loading;

Parser::Parser(const std::string& src) {
    Scanner scanner(src);
    tokens = scanner.Tokenize();
}

std::vector<GlobalDataUnit> Parser::Parse() {
    while (!IsAtEnd()) {
        if (Match(TokenType::FUNCTION)) ParseFunction();
        else if (Match(TokenType::GLOBAL)) ParseGlobal();
        else log::LogError("Unexpected token.");
    }

    return globals;
}

Token& Parser::Consume(TokenType type, const std::string& msg) {
    auto& out = Advance();
    if (out.type != type) log::LogError(msg);
    return out;
}

bool Parser::IsAtEnd() {
    return current == (int) tokens.size();
}

Token& Parser::Peek() {
    return tokens[current];
}

Token& Parser::Advance() {
    return tokens[current++];
}

Token& Parser::Previous() {
    return tokens[current - 1];
}

bool Parser::Match(TokenType type) {
    if (Check(type)) {
        Advance();
        return true;
    }
    return false;
}

bool Parser::Check(TokenType type) {
    if (IsAtEnd()) return false;
    return Peek().type == type;
}

void Parser::ParseFunction() {
    auto funcname = Consume(TokenType::NAME, "Expected function name.").data.nameString;
    Consume(TokenType::LEFT_BRACE, "Expected function block.");

    loading::GlobalDataUnit unit;
    unit.name = funcname;

    size_t streamOffset = 0;
    std::unordered_map<std::string, size_t> labelOffsets;
    std::unordered_map<size_t, std::string> jmpStreamLocations;


    while (true) {
        if (Match(TokenType::INSTRUCTION)) {
            auto& instruction = Previous();
            exec::InstructionHeader toInsert {
                instruction.data.instruction
            };

            using enum exec::OpCode;
            switch (toInsert.code) {
                default:
                    unit.dataVector.push_back(toInsert);
                    streamOffset++;
                    break;
                
                case LOAD:
                case STORE:
                case CREATELOCALS:
                case CALLINDIRECT:
                case RET: {
                    auto embedData = Consume(TokenType::EMBED_DATA, "Expected embeded data specifier.");
                    toInsert.data = embedData.data.embedData;
                    unit.dataVector.push_back(toInsert);
                    streamOffset++;
                    break;
                }
                
                case LOADCONST: {
                    unit.dataVector.push_back(toInsert);
                    auto data = Consume(TokenType::DATA_LITERAL, "Expected data literal.");
                    unit.dataVector.push_back(exec::VMValue(data.data.dataLiteral));
                    streamOffset += 2;
                    break;
                }

                case STORECONST: {
                    auto embedData = Consume(TokenType::EMBED_DATA, "Expected embeded data specifier.");
                    toInsert.data = embedData.data.embedData;
                    unit.dataVector.push_back(toInsert);

                    auto streamData = Consume(TokenType::DATA_LITERAL, "Expected data literal.");
                    unit.dataVector.push_back(exec::VMValue(streamData.data.dataLiteral));
                    streamOffset += 2;
                    break;
                }

                case ADD:
                case SUB:
                case MUL:
                case DIV:
                case GT:
                case GEQ:
                case LT:
                case LEQ:
                case EQ:
                case NOTEQ: {
                    auto typespec = Consume(TokenType::EMBED_TYPE, "Expected embeded type argument.");
                    toInsert.optype[0] = typespec.data.embedType;
                    unit.dataVector.push_back(toInsert);
                    streamOffset++;
                    break;
                }
                case CONVERT: {
                    auto from = Consume(TokenType::EMBED_TYPE, "Expected embeded type argument.");
                    auto to = Consume(TokenType::EMBED_TYPE, "Expected embeded type argument.");

                    toInsert.optype[0] = from.data.embedType;
                    toInsert.optype[1] = from.data.embedType;

                    unit.dataVector.push_back(toInsert);
                    streamOffset++;
                    break;
                }

                case CALL: {
                    auto argnum = Consume(TokenType::EMBED_DATA, "Expected embeded data specifier.");
                    toInsert.data = argnum.data.embedData;
                    
                    auto callname = Consume(TokenType::STRING_LITERAL, "Expected function name in call instruction.");
                    auto name = exec::InstructionUnit::CreateInstructionDataStream(callname.data.stringLiteral);
                    unit.dataVector.push_back(toInsert);
                    for (auto& c : name) {
                        unit.dataVector.emplace_back(c);
                    }
                    streamOffset += 1 + name.size();
                    break;
                }

                case JMP:
                case JMPIF: {
                    auto name = Consume(TokenType::NAME, "Expected label name.");
                    unit.dataVector.push_back(toInsert);
                    jmpStreamLocations.insert_or_assign(streamOffset, name.data.nameString);
                    streamOffset++;
                    break;
                }

                case GETGLOBAL: {
                    auto globname = Consume(TokenType::STRING_LITERAL, "Expected global name");
                    auto name = exec::InstructionUnit::CreateInstructionDataStream(globname.data.stringLiteral);
                    unit.dataVector.push_back(toInsert);
                    for (auto& c : name) {
                        unit.dataVector.emplace_back(c);
                    }
                    streamOffset += 1 + name.size();
                    break;
                }
            }

        }
        else if (Match(TokenType::LABEL)) {
            auto labelName = Consume(TokenType::NAME, "Expected label name.");
            if (labelOffsets.contains(labelName.data.nameString)) {
                log::LogError("Label already exists.");
            }
            labelOffsets.insert_or_assign(labelName.data.nameString, streamOffset);
        }
        else if (Match(TokenType::RIGHT_BRACE)) break;
        else {
            log::LogError("Unexpected token.");
        }
    }
    //Consume(TokenType::RIGHT_BRACE, "Expected closing brace after function body.");

    for (auto& [offset, toLabel] : jmpStreamLocations) {
        if (!labelOffsets.contains(toLabel)) log::LogError("Unkown label: \"" + toLabel + "\".");

        auto& jmpInstruction = unit.dataVector.at(offset);
        size_t labelLocation = labelOffsets.at(toLabel);

        jmpInstruction.ins.data = labelLocation - offset;
    }

    globals.push_back(unit);
}

void Parser::ParseGlobal() {
    auto globalName = Consume(TokenType::NAME, "Expected global name.");
    Consume(TokenType::LEFT_BRACE, "Expected global data block.");

    loading::GlobalDataUnit unit;
    unit.name = globalName.data.nameString;

    while (true) {
        if (Match(TokenType::RIGHT_BRACE)) break;
        else if (Match(TokenType::STRING_LITERAL)) {
            auto data = exec::InstructionUnit::CreateInstructionDataStream(Previous().data.stringLiteral);
            for (auto& c : data) unit.dataVector.push_back(c);
        }
        else if (Match(TokenType::DATA_LITERAL)) {
            unit.dataVector.push_back(Previous().data.dataLiteral);
        }
        else {
            log::LogError("Expected literal data inside global data block.");
        }
    }
    globals.push_back(unit);
}

std::vector<GlobalDataUnit> Parser::FromFile(const std::string& path) {
    std::ifstream input(path.data());
    if (!input.is_open()) {
        log::LogError("Error opening file: " + path + ".");
    }

    std::stringstream buf;
    buf << input.rdbuf();

    Parser parser(buf.str());
    return parser.Parse();
}