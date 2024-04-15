#include "vmachine.hpp"
#include "instruction.hpp"
#include <cstddef>
#include <cstdint>
#include <string_view>

using rvm::exec::VirtualMachine;

void VirtualMachine::LoadBytecode(const std::vector<InstructionUnit>& ins) {
    this->instructions = ins;
}

void VirtualMachine::LoadFunctionMap(const std::unordered_map<std::string_view, size_t>& map) {
    this->functionMap = map;
}

void VirtualMachine::Run() {
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
            hLoad();
            break;
        case Op::STORE:
            hStore();
            break;
        case Op::LOADCONST:
            hLoadConst();
            break;
        case Op::STORECONST:
            hStoreConst();
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

std::vector<rvm::exec::VMValue>& VirtualMachine::GetFrameLocals() {
    return locals.top();
}

rvm::exec::VMValue VirtualMachine::PopValue() {
    auto val = valuestack.top();
    valuestack.pop();
    return val;
}

decltype(VirtualMachine::valuestack) VirtualMachine::GetValueStackSnapshot() {
    return valuestack;
}