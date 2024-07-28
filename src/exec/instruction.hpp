#pragma once

#include <cstdint>
#include <string_view>
#include <vector>

namespace rvm::exec {
    using Word = uint64_t;

    enum class OpCode : uint8_t {
        NOP,
        HALT,

        LOAD,
        STORE,
        LOADCONST,
        STORECONST,

        CONVERT,
        ADD,
        SUB,
        MUL,
        DIV,

        LAND,
        LOR,
        LNOT,

        GT,
        GEQ,
        LT,
        LEQ,
        EQ,
        NOTEQ,
        
        BAND,
        BOR,
        BXOR,
        BNOT,
        LSHIFT,
        RSHIFT,

        JMP,
        JMPIF,

        CREATELOCALS,
        CALL,
        RET,

        CALLINDIRECT,
        GETGLOBAL
    };

    enum class DataType : uint8_t {
        NONE,
        I8,
        I16,
        I32,
        I64,

        F32,
        F64,
        PTR
    };

    struct alignas(Word) InstructionHeader {
        OpCode code = OpCode::NOP;
        DataType optype[3] = {DataType::NONE};
        int32_t data = 0;
    };

    union alignas(Word) VMValue {
        int64_t i64;
        int32_t i32;
        int16_t i16;
        int8_t i8;
        float f32;
        double f64;
        void* ptr;
        char str[sizeof(Word)];

        VMValue() : i64(0) { }
        explicit VMValue(int64_t d) : VMValue() { i64 = d; }
        explicit VMValue(int32_t d) : VMValue() { i32 = d; }
        explicit VMValue(int16_t d) : VMValue() { i16 = d; }
        explicit VMValue(int8_t d) : VMValue() { i8 = d; }
        explicit VMValue(float d) : VMValue() { f32 = d; }
        explicit VMValue(double d) : VMValue() { f64 = d; }
        explicit VMValue(void* d) : VMValue() { ptr = d; }
    };

    union alignas(Word) InstructionUnit {
        InstructionHeader ins;
        VMValue data;

        InstructionUnit() : data(int64_t(0x00)) { }
        InstructionUnit(VMValue v) : data(v) { }
        InstructionUnit(InstructionHeader header) : ins(header) { }

        static std::vector<InstructionUnit> CreateInstructionDataStream(const std::string_view& str);
    };
}