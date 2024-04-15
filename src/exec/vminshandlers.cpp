#include "instruction.hpp"
#include "vmachine.hpp"
#include <cstdint>

using rvm::exec::VirtualMachine;

void VirtualMachine::hLoad() {
    auto index = FetchIns().data.i64;
    valuestack.push(GetFrameLocals().at(index));
}

void VirtualMachine::hStore() {
    auto index = FetchIns().data.i64;
    auto data = PopValue();
    GetFrameLocals().at(index) = data;
}

void VirtualMachine::hLoadConst() {
    auto data = FetchIns().data;
    valuestack.push(data);
}

void VirtualMachine::hStoreConst() {
    auto index = FetchIns().data.i64;
    auto data = FetchIns().data;
    GetFrameLocals().at(index) = data;
}

void VirtualMachine::hConvert(DataType from, DataType to) {
    if (from == to || to == DataType::PTR) return;

    auto data = PopValue();

    using enum DataType;
    auto fromI8 = [data, this] (DataType to) {
        switch (to) {
            case I16:
                valuestack.push(VMValue((int16_t) data.i8));
                return;
            case I32:
                valuestack.push(VMValue((int32_t) data.i8));
                return;
            case I64:
                valuestack.push(VMValue((int64_t) data.i8));
                return;
            case F32:
                valuestack.push(VMValue((float) data.i8));
                return;
            case F64:
                valuestack.push(VMValue((double) data.i8));
                return;
            default:
                return;
        }
    };

    auto fromI16 = [data, this] (DataType to) {
        switch (to) {
            case I8:
                valuestack.push(VMValue((int8_t) data.i16));
                return;
            case I32:
                valuestack.push(VMValue((int32_t) data.i16));
                return;
            case I64:
                valuestack.push(VMValue((int64_t) data.i16));
                return;
            case F32:
                valuestack.push(VMValue((float) data.i16));
                return;
            case F64:
                valuestack.push(VMValue((double) data.i16));
                return;
            default:
                return;
        }
    };

    auto fromI32 = [data, this] (DataType to) {
        switch (to) {
            case I8:
                valuestack.push(VMValue((int8_t) data.i32));
                return;
            case I16:
                valuestack.push(VMValue((int16_t) data.i32));
                return;
            case I64:
                valuestack.push(VMValue((int64_t) data.i32));
                return;
            case F32:
                valuestack.push(VMValue((float) data.i32));
                return;
            case F64:
                valuestack.push(VMValue((double) data.i32));
                return;
            default:
                return;
        }
    };

    auto fromI64 = [data, this] (DataType to) {
        switch (to) {
            case I8:
                valuestack.push(VMValue((int8_t) data.i64));
                return;
            case I16:
                valuestack.push(VMValue((int16_t) data.i64));
                return;
            case I32:
                valuestack.push(VMValue((int32_t) data.i64));
                return;
            case F32:
                valuestack.push(VMValue((float) data.i64));
                return;
            case F64:
                valuestack.push(VMValue((double) data.i64));
                return;
            default:
                return;
        }
    };

    auto fromF32 = [data, this] (DataType to) {
        switch (to) {
            case I8:
                valuestack.push(VMValue((int8_t) data.f32));
                return;
            case I16:
                valuestack.push(VMValue((int16_t) data.f32));
                return;
            case I32:
                valuestack.push(VMValue((int32_t) data.f32));
                return;
            case I64:
                valuestack.push(VMValue((int64_t) data.f32));
                return;
            case F64:
                valuestack.push(VMValue((double) data.f32));
                return;
            default:
                return;
        }
    };

    auto fromF64 = [data, this] (DataType to) {
        switch (to) {
            case I8:
                valuestack.push(VMValue((int8_t) data.f64));
                return;
            case I16:
                valuestack.push(VMValue((int16_t) data.f64));
                return;
            case I32:
                valuestack.push(VMValue((int32_t) data.f64));
                return;
            case I64:
                valuestack.push(VMValue((int64_t) data.f64));
                return;
            case F32:
                valuestack.push(VMValue((float) data.f64));
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
    valuestack.push(result);
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
    valuestack.push(result);
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
    valuestack.push(result);
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
    valuestack.push(result);
}

void VirtualMachine::hLand() {
    auto rhs = PopValue();
    auto lhs = PopValue();

    VMValue result;
    result.i8 = lhs.i8 && rhs.i8;
    valuestack.push(result);
}

void VirtualMachine::hLor() {
    auto rhs = PopValue();
    auto lhs = PopValue();

    VMValue result;
    result.i8 = lhs.i8 || rhs.i8;
    valuestack.push(result);
}

void VirtualMachine::hLnot() {
    auto data = PopValue();
    VMValue result;
    result.i8 = !data.i8;
    valuestack.push(result);
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
    valuestack.push(result);
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
    valuestack.push(result);
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
    valuestack.push(result);
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
    valuestack.push(result);
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
    valuestack.push(result);
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
    valuestack.push(result);
}

void VirtualMachine::hBand() {
    auto rhs = PopValue();
    auto lhs = PopValue();

    VMValue result;
    result.i64 = lhs.i64 & rhs.i64;
    valuestack.push(result);
}

void VirtualMachine::hBor() {
    auto rhs = PopValue();
    auto lhs = PopValue();

    VMValue result;
    result.i64 = lhs.i64 | rhs.i64;
    valuestack.push(result);
}

void VirtualMachine::hBxor() {
    auto rhs = PopValue();
    auto lhs = PopValue();

    VMValue result;
    result.i64 = lhs.i64 ^ rhs.i64;
    valuestack.push(result);
}

void VirtualMachine::hBnot() {
    auto data = PopValue();
    valuestack.emplace(int64_t(~data.i64));
}

void VirtualMachine::hLshift() {
    auto rhs = PopValue();
    auto lhs = PopValue();

    VMValue result;
    result.i64 = lhs.i64 << (rhs.i64 % 64);
    valuestack.push(result);
}

void VirtualMachine::hRshift() {
    auto rhs = PopValue();
    auto lhs = PopValue();

    VMValue result;
    result.i64 = lhs.i64 >> (rhs.i64 % 64);
    valuestack.push(result);
}

void VirtualMachine::hJmp(int32_t offset) {
    insIndex += offset;
}

void VirtualMachine::hJmpIf(int32_t offset) {
    auto flag = PopValue();
    if (flag.i8) insIndex += offset;
}

void VirtualMachine::hCreateLocals(int32_t number) {
    auto localframe = GetFrameLocals();
    for (int i = 0; i < number; i++) {
        localframe.emplace_back();
    }
}

void VirtualMachine::hCall(int32_t argnum) {
    auto name = ConsumeStringViewFromIns();
    callstack.push(insIndex);
    locals.emplace();
    auto argframe = GetFrameLocals();
    for (int i = 0; i < argnum; i++) {
        argframe.emplace_back(PopValue());
    }
    insIndex = functionMap[name];
}

void VirtualMachine::hRet() {
    auto retLoc = callstack.top();
    callstack.pop();
    locals.pop();
    insIndex = retLoc;
}