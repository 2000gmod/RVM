#include <iostream>

#include "exec/instruction.hpp"
#include "exec/vmachine.hpp"
#include "loader/loading.hpp"

using namespace rvm;

int main() {
    exec::VirtualMachine vm;

    loading::FunctionUnit factorial {
        "fact", 
        {
            exec::InstructionHeader {exec::OpCode::LOAD, {}, 0},
            exec::InstructionHeader {exec::OpCode::LOADCONST},
            exec::VMValue(0),
            exec::InstructionHeader {exec::OpCode::EQ, {exec::DataType::I64}},
            exec::InstructionHeader {exec::OpCode::JMPIF, {}, 10},
            exec::InstructionHeader {exec::OpCode::LOAD, {}, 0},
            exec::InstructionHeader {exec::OpCode::LOAD, {}, 0},
            exec::InstructionHeader {exec::OpCode::LOADCONST},
            exec::VMValue(1),
            exec::InstructionHeader {exec::OpCode::SUB, {exec::DataType::I64}},
            exec::InstructionHeader {exec::OpCode::CALL, {}, 1},
            exec::VMValue(0x74636166),
            exec::InstructionHeader {exec::OpCode::MUL, {exec::DataType::I64}},
            exec::InstructionHeader {exec::OpCode::RET},
            exec::InstructionHeader {exec::OpCode::LOADCONST},
            exec::VMValue(1),
            exec::InstructionHeader {exec::OpCode::RET}
        }
    };

    loading::FunctionUnit main {
        "main",
        {
            exec::InstructionHeader {exec::OpCode::LOADCONST},
            exec::VMValue(37),
            exec::InstructionHeader {exec::OpCode::CALL, {}, 1},
            exec::VMValue(0x6f626966),
            exec::InstructionHeader {exec::OpCode::HALT},
        }
    };

    // 0x6f626966
    loading::FunctionUnit fibo {
        "fibo",
        {
            exec::InstructionHeader {exec::OpCode::LOAD, {}, 0},
            exec::InstructionHeader {exec::OpCode::LOADCONST},
            exec::VMValue(0),
            exec::InstructionHeader {exec::OpCode::EQ, {exec::DataType::I64}},
            exec::InstructionHeader {exec::OpCode::LOAD, {}, 0},
            exec::InstructionHeader {exec::OpCode::LOADCONST},
            exec::VMValue(1),
            exec::InstructionHeader {exec::OpCode::EQ, {exec::DataType::I64}},
            exec::InstructionHeader {exec::OpCode::LOR},
            exec::InstructionHeader {exec::OpCode::JMPIF, {}, 15},
            exec::InstructionHeader {exec::OpCode::LOAD, {}, 0},
            exec::InstructionHeader {exec::OpCode::LOADCONST},
            exec::VMValue(1),
            exec::InstructionHeader {exec::OpCode::SUB, {exec::DataType::I64}},
            exec::InstructionHeader {exec::OpCode::CALL, {}, 1},
            exec::VMValue(0x6f626966),
            exec::InstructionHeader {exec::OpCode::LOAD, {}, 0},
            exec::InstructionHeader {exec::OpCode::LOADCONST},
            exec::VMValue(2),
            exec::InstructionHeader {exec::OpCode::SUB, {exec::DataType::I64}},
            exec::InstructionHeader {exec::OpCode::CALL, {}, 1},
            exec::VMValue(0x6f626966),
            exec::InstructionHeader {exec::OpCode::ADD, {exec::DataType::I64}},
            exec::InstructionHeader {exec::OpCode::RET},
            exec::InstructionHeader {exec::OpCode::LOADCONST},
            exec::VMValue(1),
            exec::InstructionHeader {exec::OpCode::RET},
        }
    };

    vm.LoadBytecode({factorial, main, fibo});
    vm.Run();
    
    auto value = vm.GetValueStackSnapshot().top();
    std::cout << value.i64 << "\n";
}