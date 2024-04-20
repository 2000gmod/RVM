#pragma once

#include <cstddef>
#include <cstdint>
#include <stack>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>

#include "instruction.hpp"
#include "../loading/loading.hpp"

namespace rvm::exec {
    class VirtualMachine {
    private:
        std::vector<InstructionUnit> instructions;
        std::unique_ptr<VMValue[]> valueStack;
        std::stack<size_t, std::vector<size_t>> returnStack, frameIndexStack;
        std::unordered_map<std::string, size_t> functionMap;
        std::unordered_map<std::string, std::function<void(int)>> builtInFunctions;

        std::vector<VMValue> locals;

        size_t insIndex = 0;
        size_t localFrameBaseIndex = 0;

        int64_t stackSize = 8192;
        int64_t stackIndex = -1;

        bool running = true;
    public:
        VirtualMachine();
        VirtualMachine(int64_t stack, int64_t localSize);
        ~VirtualMachine() = default;
        
        void LoadBytecode(const std::vector<loading::FunctionUnit>& functions);
        void Run(const std::string& entry = "main");

    private:
        const InstructionUnit& FetchIns();
        void ExecutionLoop();
        bool ExecuteInstruction(const InstructionUnit& ins);
        const char* ConsumeStringViewFromIns();

        VMValue PopValue();
        void PushValue(VMValue value);
        VMValue& GetLocalAtIndex(int32_t index);

        void SetupBuiltInFuncs();


        // Instruction handlers

        void hLoad(int32_t index);
        void hStore(int32_t index);
        void hLoadConst();
        void hStoreConst(int32_t index);

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
        std::vector<VMValue> GetValueStackSnapshot();
    };
}