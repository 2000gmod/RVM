#include <iostream>

#include "exec/instruction.hpp"
#include "exec/vmachine.hpp"

using namespace rvm;

int main() {
    exec::VirtualMachine vm;

    std::vector<exec::InstructionUnit> bytecode = {
        {exec::InstructionHeader {exec::OpCode::LOADCONST}},
        {exec::VMValue {2.0}},
        {exec::InstructionHeader {exec::OpCode::LOADCONST}},
        {exec::VMValue {4.0}},
        {exec::InstructionHeader {exec::OpCode::DIV, {exec::DataType::F64}}}
    };
    vm.LoadBytecode(bytecode);
    vm.Run();
    
    auto value = vm.GetValueStackSnapshot().top();
    std::cout << value.f64 << "\n";
}