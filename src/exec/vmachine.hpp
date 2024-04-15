#pragma once

#include <cstddef>
#include <cstdint>
#include <stack>
#include <string_view>
#include <vector>
#include <unordered_map>

#include "instruction.hpp"


namespace rvm::exec {
    class VirtualMachine {
    private:
        std::vector<InstructionUnit> instructions;
        std::stack<VMValue, std::vector<VMValue>> valuestack;
        std::stack<size_t> callstack;
        std::unordered_map<std::string_view, size_t> functionMap;
        std::stack<std::vector<VMValue>> locals;

        //size_t stackFrameBase = 0;
        size_t insIndex = 0;
    public:
        VirtualMachine() = default;
        
        void LoadBytecode(const std::vector<InstructionUnit>& ins);
        void LoadFunctionMap(const std::unordered_map<std::string_view, size_t>& map);
        void Run();

    private:
        InstructionUnit FetchIns();
        void ExecutionLoop();
        bool ExecuteInstruction(InstructionUnit ins);
        std::string_view ConsumeStringViewFromIns();
        std::vector<VMValue>& GetFrameLocals();

        VMValue PopValue();


        // Instruction handlers

        void hLoad();
        void hStore();
        void hLoadConst();
        void hStoreConst();

        void hConvert(DataType t1, DataType t2);
        void hAdd(DataType t);
        void hSub(DataType t);
        void hMul(DataType t);
        void hDiv(DataType t);

        void hLand();
        void hLor();
        void hLnot();

        void hGt(DataType t);
        void hGeq(DataType t);
        void hLt(DataType t);
        void hLeq(DataType t);
        void hEq(DataType t);
        void hNotEq(DataType t);

        void hBand();
        void hBor();
        void hBxor();
        void hBnot();
        void hLshift();
        void hRshift();

        void hJmp(int32_t offset);
        void hJmpIf(int32_t offset);

        void hCreateLocals(int32_t number);
        void hCall(int32_t argnumber);
        void hRet();

    public:
        decltype(valuestack) GetValueStackSnapshot();
    };
}