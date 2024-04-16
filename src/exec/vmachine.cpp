#include "vmachine.hpp"
#include "instruction.hpp"
#include <cstdint>
#include <string_view>
#include <iostream>

using rvm::exec::VirtualMachine;

VirtualMachine::VirtualMachine() : VirtualMachine(8192, 8192) { }

VirtualMachine::VirtualMachine(int64_t stack, int64_t localSize) : stackSize(stack) {
    locals.reserve(localSize);
    valueStack = std::make_unique<VMValue[]>(stackSize);
}

void VirtualMachine::LoadBytecode(const std::vector<loading::FunctionUnit>& functions) {
    for (auto& func : functions) {
        auto fIndex = instructions.size();
        for (auto& ins : func.code) {
            instructions.emplace_back(ins);
        }
        functionMap.insert_or_assign(func.name, fIndex);
    }
}

void VirtualMachine::Run(const std::string& entry) {
    if (!functionMap.contains(entry)) {
        std::cerr << "Fatal error: Unable to find entry point function: \"" << entry << "\".\n";
        std::exit(1);
    }
    insIndex = functionMap.at(entry);
    ExecutionLoop();
}

rvm::exec::InstructionUnit VirtualMachine::FetchIns() {
    auto out = instructions[insIndex];
    insIndex++;
    return out;
}

void VirtualMachine::ExecutionLoop() {
    while (insIndex < instructions.size()) {
        auto ins = FetchIns();
        if (!ExecuteInstruction(ins)) break;
    }

}

bool VirtualMachine::ExecuteInstruction(InstructionUnit ins) {
    using Op = OpCode;
    switch (ins.ins.code) {
        case Op::NOP:
            break;
        case Op::HALT:
            return false;
        case Op::LOAD: 
            hLoad(ins.ins.data);
            break;
        case Op::STORE:
            hStore(ins.ins.data);
            break;
        case Op::LOADCONST:
            hLoadConst();
            break;
        case Op::STORECONST:
            hStoreConst(ins.ins.data);
            break;
        case Op::CONVERT:
            hConvert(ins.ins.optype[0], ins.ins.optype[1]);
            break;
        case Op::ADD:
            hAdd(ins.ins.optype[0]);
            break;
        case Op::SUB:
            hSub(ins.ins.optype[0]);
            break;
        case Op::MUL:
            hMul(ins.ins.optype[0]);
            break;
        case Op::DIV:
            hDiv(ins.ins.optype[0]);
            break;
        case Op::LAND:
            hLand();
            break;
        case Op::LOR:
            hLor();
            break;
        case Op::LNOT:
            hLnot();
            break;
        case Op::GT:
            hGt(ins.ins.optype[0]);
            break;
        case Op::GEQ:
            hGeq(ins.ins.optype[0]);
            break;
        case Op::LT:
            hLt(ins.ins.optype[0]);
            break;
        case Op::LEQ:
            hLeq(ins.ins.optype[0]);
            break;
        case Op::EQ:
            hEq(ins.ins.optype[0]);
            break;
        case Op::NOTEQ:
            hNotEq(ins.ins.optype[0]);
            break;
        case Op::BAND:
            hBand();
            break;
        case Op::BOR:
            hBor();
            break;
        case Op::BXOR:
            hBxor();
            break;
        case Op::BNOT:
            hBnot();
            break;
        case Op::LSHIFT:
            hLshift();
            break;
        case Op::RSHIFT:
            hRshift();
            break;
        case Op::JMP:
            hJmp(ins.ins.data);
            break;
        case Op::JMPIF:
            hJmpIf(ins.ins.data);
            break;
        case Op::CREATELOCALS:
            hCreateLocals(ins.ins.data);
            break;
        case Op::CALL:
            hCall(ins.ins.data);
            break;
        case Op::RET:
            hRet();
            break;
    }
    return true;
}

std::string_view VirtualMachine::ConsumeStringViewFromIns() {
    auto* insPtr = (char*) &instructions[insIndex];
    std::string_view out {insPtr};

    uint64_t len = out.length();
    insIndex += (len / 8) + 1;

    return out;
}

rvm::exec::VMValue VirtualMachine::PopValue() {
    auto val = valueStack[stackIndex--];
    return val;
}

void VirtualMachine::PushValue(rvm::exec::VMValue value) { 
    if (stackIndex >= stackSize - 1) {
        std::cerr << "Stack overflow error.\n";
        std::exit(1);
    }
    valueStack[++stackIndex] = value;
}

rvm::exec::VMValue& VirtualMachine::GetLocalAtIndex(int32_t index) {
    return locals[index + localFrameBaseIndex];
}
std::vector<rvm::exec::VMValue> VirtualMachine::GetValueStackSnapshot() {
    std::vector<VMValue> out;
    for (unsigned i = 0; i < stackSize; i++) {
        out.push_back(valueStack[i]);
    }
    return out;
}