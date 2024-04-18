#include "instruction.hpp"
#include "vmachine.hpp"
#include <cstdint>

using rvm::exec::VirtualMachine;

void VirtualMachine::hLoad(int32_t index) {
    PushValue(GetLocalAtIndex(index));
}

void VirtualMachine::hStore(int32_t index) {
    auto data = PopValue();
    GetLocalAtIndex(index) = data;
}

void VirtualMachine::hLoadConst() {
    auto data = FetchIns().data;
    PushValue(data);
}

void VirtualMachine::hStoreConst(int32_t index) {
    auto data = FetchIns().data;
    GetLocalAtIndex(index) = data;
}

void VirtualMachine::hConvert(DataType from, DataType to) {
    if (from == to || to == DataType::PTR) return;

    auto data = PopValue();

    using enum DataType;
    auto fromI8 = [data, this] (DataType to) {
        switch (to) {
            case I16:
                PushValue(VMValue((int16_t) data.i8));
                return;
            case I32:
                PushValue(VMValue((int32_t) data.i8));
                return;
            case I64:
                PushValue(VMValue((int64_t) data.i8));
                return;
            case F32:
                PushValue(VMValue((float) data.i8));
                return;
            case F64:
                PushValue(VMValue((double) data.i8));
                return;
            default:
                return;
        }
    };

    auto fromI16 = [data, this] (DataType to) {
        switch (to) {
            case I8:
                PushValue(VMValue((int8_t) data.i16));
                return;
            case I32:
                PushValue(VMValue((int32_t) data.i16));
                return;
            case I64:
                PushValue(VMValue((int64_t) data.i16));
                return;
            case F32:
                PushValue(VMValue((float) data.i16));
                return;
            case F64:
                PushValue(VMValue((double) data.i16));
                return;
            default:
                return;
        }
    };

    auto fromI32 = [data, this] (DataType to) {
        switch (to) {
            case I8:
                PushValue(VMValue((int8_t) data.i32));
                return;
            case I16:
                PushValue(VMValue((int16_t) data.i32));
                return;
            case I64:
                PushValue(VMValue((int64_t) data.i32));
                return;
            case F32:
                PushValue(VMValue((float) data.i32));
                return;
            case F64:
                PushValue(VMValue((double) data.i32));
                return;
            default:
                return;
        }
    };

    auto fromI64 = [data, this] (DataType to) {
        switch (to) {
            case I8:
                PushValue(VMValue((int8_t) data.i64));
                return;
            case I16:
                PushValue(VMValue((int16_t) data.i64));
                return;
            case I32:
                PushValue(VMValue((int32_t) data.i64));
                return;
            case F32:
                PushValue(VMValue((float) data.i64));
                return;
            case F64:
                PushValue(VMValue((double) data.i64));
                return;
            default:
                return;
        }
    };

    auto fromF32 = [data, this] (DataType to) {
        switch (to) {
            case I8:
                PushValue(VMValue((int8_t) data.f32));
                return;
            case I16:
                PushValue(VMValue((int16_t) data.f32));
                return;
            case I32:
                PushValue(VMValue((int32_t) data.f32));
                return;
            case I64:
                PushValue(VMValue((int64_t) data.f32));
                return;
            case F64:
                PushValue(VMValue((double) data.f32));
                return;
            default:
                return;
        }
    };

    auto fromF64 = [data, this] (DataType to) {
        switch (to) {
            case I8:
                PushValue(VMValue((int8_t) data.f64));
                return;
            case I16:
                PushValue(VMValue((int16_t) data.f64));
                return;
            case I32:
                PushValue(VMValue((int32_t) data.f64));
                return;
            case I64:
                PushValue(VMValue((int64_t) data.f64));
                return;
            case F32:
                PushValue(VMValue((float) data.f64));
                return;
            default:
                return;
        }
    };

    switch (from) {
        case I8:
            fromI8(to);
            return;
        case I16:
            fromI16(to);
            return;
        case I32:
            fromI32(to);
            return;
        case I64:
            fromI64(to);
            return;
        case F32:
            fromF32(to);
            return;
        case F64:
            fromF64(to);
            return;
        default:
            return;
    }
}

void VirtualMachine::hAdd(DataType t) {
    auto rhs = PopValue();
    auto lhs = PopValue();

    VMValue result;

    switch (t) {
        case DataType::I8:
            result.i8 = lhs.i8 + rhs.i8;
            break;
        case DataType::I16:
            result.i16 = lhs.i16 + rhs.i16;
            break;
        case DataType::I32:
            result.i32 = lhs.i32 + rhs.i32;
            break;
        case DataType::I64:
            result.i64 = lhs.i64 + rhs.i64;
            break;
        case DataType::F32:
            result.f32 = lhs.f32 + rhs.f32;
            break;
        case DataType::F64:
            result.f64 = lhs.f64 + rhs.f64;
            break;
        default:
            result.i64 = lhs.i64 + rhs.i64;
            break;
    }
    PushValue(result);
}

void VirtualMachine::hSub(DataType t) {
    auto rhs = PopValue();
    auto lhs = PopValue();

    VMValue result;

    switch (t) {
        case DataType::I8:
            result.i8 = lhs.i8 - rhs.i8;
            break;
        case DataType::I16:
            result.i16 = lhs.i16 - rhs.i16;
            break;
        case DataType::I32:
            result.i32 = lhs.i32 - rhs.i32;
            break;
        case DataType::I64:
            result.i64 = lhs.i64 - rhs.i64;
            break;
        case DataType::F32:
            result.f32 = lhs.f32 - rhs.f32;
            break;
        case DataType::F64:
            result.f64 = lhs.f64 - rhs.f64;
            break;
        default:
            result.i64 = lhs.i64 - rhs.i64;
            break;
    }
    PushValue(result);
}

void VirtualMachine::hMul(DataType t) {
    auto rhs = PopValue();
    auto lhs = PopValue();

    VMValue result;

    switch (t) {
        case DataType::I8:
            result.i8 = lhs.i8 * rhs.i8;
            break;
        case DataType::I16:
            result.i16 = lhs.i16 * rhs.i16;
            break;
        case DataType::I32:
            result.i32 = lhs.i32 * rhs.i32;
            break;
        case DataType::I64:
            result.i64 = lhs.i64 * rhs.i64;
            break;
        case DataType::F32:
            result.f32 = lhs.f32 * rhs.f32;
            break;
        case DataType::F64:
            result.f64 = lhs.f64 * rhs.f64;
            break;
        default:
            result.i64 = lhs.i64 * rhs.i64;
            break;
    }
    PushValue(result);
}

void VirtualMachine::hDiv(DataType t) {
    auto rhs = PopValue();
    auto lhs = PopValue();

    VMValue result;

    switch (t) {
        case DataType::I8:
            result.i8 = lhs.i8 / rhs.i8;
            break;
        case DataType::I16:
            result.i16 = lhs.i16 / rhs.i16;
            break;
        case DataType::I32:
            result.i32 = lhs.i32 / rhs.i32;
            break;
        case DataType::I64:
            result.i64 = lhs.i64 / rhs.i64;
            break;
        case DataType::F32:
            result.f32 = lhs.f32 / rhs.f32;
            break;
        case DataType::F64:
            result.f64 = lhs.f64 / rhs.f64;
            break;
        default:
            result.i64 = lhs.i64 / rhs.i64;
            break;
    }
    PushValue(result);
}

void VirtualMachine::hLand() {
    auto rhs = PopValue();
    auto lhs = PopValue();

    VMValue result;
    result.i8 = lhs.i8 && rhs.i8;
    PushValue(result);
}

void VirtualMachine::hLor() {
    auto rhs = PopValue();
    auto lhs = PopValue();

    VMValue result;
    result.i8 = lhs.i8 || rhs.i8;
    PushValue(result);
}

void VirtualMachine::hLnot() {
    auto data = PopValue();
    VMValue result;
    result.i8 = !data.i8;
    PushValue(result);
}

void VirtualMachine::hGt(DataType t) {
    auto rhs = PopValue();
    auto lhs = PopValue();

    VMValue result;

    switch (t) {
        case DataType::I8:
            result.i8 = lhs.i8 > rhs.i8;
            break;
        case DataType::I16:
            result.i8 = lhs.i16 > rhs.i16;
            break;
        case DataType::I32:
            result.i8 = lhs.i32 > rhs.i32;
            break;
        case DataType::I64:
            result.i8 = lhs.i64 > rhs.i64;
            break;
        case DataType::F32:
            result.i8 = lhs.f32 > rhs.f32;
            break;
        case DataType::F64:
            result.i8 = lhs.f64 > rhs.f64;
            break;
        default:
            result.i8 = lhs.ptr > rhs.ptr;
            break;
    }
    PushValue(result);
}

void VirtualMachine::hGeq(DataType t) {
    auto rhs = PopValue();
    auto lhs = PopValue();

    VMValue result;

    switch (t) {
        case DataType::I8:
            result.i8 = lhs.i8 >= rhs.i8;
            break;
        case DataType::I16:
            result.i8 = lhs.i16 >= rhs.i16;
            break;
        case DataType::I32:
            result.i8 = lhs.i32 >= rhs.i32;
            break;
        case DataType::I64:
            result.i8 = lhs.i64 >= rhs.i64;
            break;
        case DataType::F32:
            result.i8 = lhs.f32 >= rhs.f32;
            break;
        case DataType::F64:
            result.i8 = lhs.f64 >= rhs.f64;
            break;
        default:
            result.i8 = lhs.ptr >= rhs.ptr;
            break;
    }
    PushValue(result);
}

void VirtualMachine::hLt(DataType t) {
    auto rhs = PopValue();
    auto lhs = PopValue();

    VMValue result;

    switch (t) {
        case DataType::I8:
            result.i8 = lhs.i8 < rhs.i8;
            break;
        case DataType::I16:
            result.i8 = lhs.i16 < rhs.i16;
            break;
        case DataType::I32:
            result.i8 = lhs.i32 < rhs.i32;
            break;
        case DataType::I64:
            result.i8 = lhs.i64 < rhs.i64;
            break;
        case DataType::F32:
            result.i8 = lhs.f32 < rhs.f32;
            break;
        case DataType::F64:
            result.i8 = lhs.f64 < rhs.f64;
            break;
        default:
            result.i8 = lhs.ptr < rhs.ptr;
            break;
    }
    PushValue(result);
}

void VirtualMachine::hLeq(DataType t) {
    auto rhs = PopValue();
    auto lhs = PopValue();

    VMValue result;

    switch (t) {
        case DataType::I8:
            result.i8 = lhs.i8 <= rhs.i8;
            break;
        case DataType::I16:
            result.i8 = lhs.i16 <= rhs.i16;
            break;
        case DataType::I32:
            result.i8 = lhs.i32 <= rhs.i32;
            break;
        case DataType::I64:
            result.i8 = lhs.i64 <= rhs.i64;
            break;
        case DataType::F32:
            result.i8 = lhs.f32 <= rhs.f32;
            break;
        case DataType::F64:
            result.i8 = lhs.f64 <= rhs.f64;
            break;
        default:
            result.i8 = lhs.ptr <= rhs.ptr;
            break;
    }
    PushValue(result);
}

void VirtualMachine::hEq(DataType t) {
    auto rhs = PopValue();
    auto lhs = PopValue();

    VMValue result;

    switch (t) {
        case DataType::I8:
            result.i8 = lhs.i8 == rhs.i8;
            break;
        case DataType::I16:
            result.i8 = lhs.i16 == rhs.i16;
            break;
        case DataType::I32:
            result.i8 = lhs.i32 == rhs.i32;
            break;
        case DataType::I64:
            result.i8 = lhs.i64 == rhs.i64;
            break;
        case DataType::F32:
            result.i8 = lhs.f32 == rhs.f32;
            break;
        case DataType::F64:
            result.i8 = lhs.f64 == rhs.f64;
            break;
        default:
            result.i8 = lhs.ptr == rhs.ptr;
            break;
    }
    PushValue(result);
}

void VirtualMachine::hNotEq(DataType t) {
    auto rhs = PopValue();
    auto lhs = PopValue();

    VMValue result;

    switch (t) {
        case DataType::I8:
            result.i8 = lhs.i8 != rhs.i8;
            break;
        case DataType::I16:
            result.i8 = lhs.i16 != rhs.i16;
            break;
        case DataType::I32:
            result.i8 = lhs.i32 != rhs.i32;
            break;
        case DataType::I64:
            result.i8 = lhs.i64 != rhs.i64;
            break;
        case DataType::F32:
            result.i8 = lhs.f32 != rhs.f32;
            break;
        case DataType::F64:
            result.i8 = lhs.f64 != rhs.f64;
            break;
        default:
            result.i8 = lhs.ptr != rhs.ptr;
            break;
    }
    PushValue(result);
}

void VirtualMachine::hBand() {
    auto rhs = PopValue();
    auto lhs = PopValue();

    VMValue result;
    result.i64 = lhs.i64 & rhs.i64;
    PushValue(result);
}

void VirtualMachine::hBor() {
    auto rhs = PopValue();
    auto lhs = PopValue();

    VMValue result;
    result.i64 = lhs.i64 | rhs.i64;
    PushValue(result);
}

void VirtualMachine::hBxor() {
    auto rhs = PopValue();
    auto lhs = PopValue();

    VMValue result;
    result.i64 = lhs.i64 ^ rhs.i64;
    PushValue(result);
}

void VirtualMachine::hBnot() {
    auto data = PopValue();
    PushValue(VMValue(~data.i64));
}

void VirtualMachine::hLshift() {
    auto rhs = PopValue();
    auto lhs = PopValue();

    VMValue result;
    result.i64 = lhs.i64 << (rhs.i64 % 64);
    PushValue(result);
}

void VirtualMachine::hRshift() {
    auto rhs = PopValue();
    auto lhs = PopValue();

    VMValue result;
    result.i64 = lhs.i64 >> (rhs.i64 % 64);
    PushValue(result);
}

void VirtualMachine::hJmp(int32_t offset) {
    insIndex += offset - 1;
}

void VirtualMachine::hJmpIf(int32_t offset) {
    auto flag = PopValue();
    if (flag.i8) insIndex += offset - 1;
}

void VirtualMachine::hCreateLocals(int32_t number) {
    for (int i = 0; i < number; i++) {
        locals.emplace_back();
    }
}

void VirtualMachine::hCall(int32_t argnum) {
    auto name = std::string(ConsumeStringViewFromIns());

    returnStack.push(insIndex);
    frameIndexStack.push(localFrameBaseIndex);
    localFrameBaseIndex = locals.size();

    for (int i = 0; i < argnum; i++) {
        locals.push_back(PopValue());
    }
    insIndex = functionMap.at(name);
}

void VirtualMachine::hRet() {
    if (returnStack.empty()) {
        running = false;
        return;
    }
    auto retLoc = returnStack.top();
    returnStack.pop();
    
    auto previousBase = frameIndexStack.top();
    frameIndexStack.pop();
    locals.erase(locals.begin() + previousBase + 1, locals.end());
    localFrameBaseIndex = previousBase;

    insIndex = retLoc;
}