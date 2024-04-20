#include "assembler/parser.hpp"
#include "exec/vmachine.hpp"
#include "log/log.hpp"


using namespace rvm;

int main(int argc, char** argv) {
    if (argc == 1) log::LogError("Expected input file.");
    auto code = assembler::Parser::FromFile(argv[1]);
    
    exec::VirtualMachine vm;
    vm.LoadBytecode(code);
    vm.Run();
}