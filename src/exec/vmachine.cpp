#include "vmachine.hpp"
#include "instruction.hpp"
#include <cstdint>
#include <string_view>
#include <iostream>
#include "../log/log.hpp"

using rvm::exec::VirtualMachine;
using namespace std::literals;

VirtualMachine::VirtualMachine() : VirtualMachine(8192, 8192) { }

VirtualMachine::VirtualMachine(int64_t stack, int64_t localSize) : stackSize(stack) {
    locals.reserve(localSize);
    valueStack = std::make_unique<VMValue[]>(stackSize);
    SetupBuiltInFuncs();
}

void VirtualMachine::LoadBytecode(const std::vector<loading::GlobalDataUnit>& datums) {
    size_t reserveSize = 0;
    for (auto &d : datums) {
        reserveSize += d.dataVector.size();
    }
    instructions.reserve(reserveSize);

    for (auto& data : datums) {
        auto fIndex = instructions.size();
        for (auto& ins : data.dataVector) {
            instructions.emplace_back(ins);
        }
        globalDataMap.insert_or_assign(data.name, &instructions[fIndex]);
    }
}

void VirtualMachine::Run(const std::string& entry) {
    if (!globalDataMap.contains(entry)) {
        log::LogError("Unable to find entry function: "s + entry + ".");
    }
    insIndex = globalDataMap.at(entry) - &instructions[0];
    valueIndexStack.push(valuesFrameBaseIndex);
    try {
        ExecutionLoop();
    }
    catch (std::exception& e) {
        log::LogError("Fatal error reported: "s + e.what());
    }
}

const rvm::exec::InstructionUnit& VirtualMachine::FetchIns() {
    return instructions[insIndex++];
}

void VirtualMachine::ExecutionLoop() {
    while (insIndex < instructions.size() && running) {
        auto ins = FetchIns();
        if (!ExecuteInstruction(ins)) break;
    }

}

bool VirtualMachine::ExecuteInstruction(const InstructionUnit& ins) {
    using Op = OpCode;
    switch (ins.ins.code) {
        default:
            throw VirtualMachineException("Unknown instruction.");
            break;
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
            hRet(ins.ins.data);
            break;
        case Op::CALLINDIRECT:
            hCallIndirect(ins.ins.data);
            break;
        case Op::GETGLOBAL:
            hGetGlobal();
            break;
    }
    return true;
}

const char* VirtualMachine::ConsumeStringViewFromIns() {
    auto* insPtr = (char*) &instructions[insIndex];
    std::string_view out {insPtr};

    uint64_t len = out.length();
    insIndex += (len / 8) + 1;

    return out.data();
}

rvm::exec::VMValue VirtualMachine::PopValue() {
    if (stackIndex < std::bit_cast<int64_t>(valuesFrameBaseIndex)) {
        throw VirtualMachineException("Value stack operation fell outside of function frame.");
    }
    auto val = valueStack[stackIndex--];
    return val;
}

void VirtualMachine::PushValue(rvm::exec::VMValue value) { 
    if (stackIndex >= stackSize - 1) {
        throw VirtualMachineException("Stack overflow error.");
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

void VirtualMachine::SetupBuiltInFuncs() {

    builtInFunctions.insert_or_assign("__printchar", [this] (int) {
        auto val = PopValue();
        std::cout << val.i8;
    });

    builtInFunctions.insert_or_assign("__printi8", [this] (int) {
        auto val = PopValue();
        std::cout << int(val.i8);
    });

    builtInFunctions.insert_or_assign("__printi16", [this] (int) {
        auto val = PopValue();
        std::cout << val.i16;
    });

    builtInFunctions.insert_or_assign("__printi32", [this] (int) {
        auto val = PopValue();
        std::cout << val.i32;
    });

    builtInFunctions.insert_or_assign("__printi64", [this] (int) {
        auto val = PopValue();
        std::cout << val.i64;
    });

    builtInFunctions.insert_or_assign("__printf32", [this] (int) {
        auto val = PopValue();
        std::cout << val.f32;
    });

    builtInFunctions.insert_or_assign("__printf64", [this] (int) {
        auto val = PopValue();
        std::cout << val.i64;
    });

    builtInFunctions.insert_or_assign("__printstr", [this] (int) {
        auto val = PopValue();
        std::cout << (char*) val.ptr;
    });

    builtInFunctions.insert_or_assign("__printnl", [] (int) {
        std::cout << "\n";
    });
}