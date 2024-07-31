#include <args.hxx>

#include "exec/vmachine.hpp"
#include "loading/loading.hpp"
#include "log/log.hpp"
#include "assembler/parser.hpp"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>

void MainError(const char* msg, int code = 1) {
    std::cerr << msg << "\n";
    std::exit(code);
}

#define USE_DEBUG_ARGS 0

int main(int argc, char** argv) {
    args::ArgumentParser parser("");
    args::HelpFlag help(parser, "help", "Help page", {'h', "help"});
    
    args::Group executeFlags(parser, "Execution options", args::Group::Validators::DontCare);
    args::ValueFlag<unsigned long> stackSize(executeFlags, "size", "Stack size (in MB).", {"xmS"}, 1);
    args::ValueFlag<unsigned long> localSize(executeFlags, "N", "Number of locals pre-allocated (in thousands).", {"xmL"}, 8);
    args::ValueFlag<std::string> entryPoint(executeFlags, "func", "Name of entry function (defaults to \"main\").", {'e', "entry"}, "main");

    args::Group assemblyGroup(parser, "Assembler options", args::Group::Validators::AllOrNone);
    args::Flag assemble(assemblyGroup, "", "Enable assembler (max. 1 input file).", {'a', "asm"});
    args::ValueFlag<std::string> outputFile(assemblyGroup, "out", "Specify output file (if using assembler).", {'o', "output"});

    args::Flag runFromAssembler(parser, "", "Run a single RVM assembly file directly.", {"rs"});
    args::Flag verbose(parser, "", "Verbose mode.", {'v', "verbose"});
    
    args::PositionalList<std::string> inputFiles(parser, "files", "Input files.");

    try {
        if constexpr (!(USE_DEBUG_ARGS)) parser.ParseCLI(argc, argv);
        else {
            parser.Prog("rvm");

            std::vector<std::string> args1 = {
                "../examples/test.rvas",
                "-a",
                "-o",
                "../examples/test.rvm"
            };

            std::vector<std::string> args2 = {
                "../examples/test.rvm"
            };

            std::vector<std::string> args3 = {
                "-h"
            };

            std::vector<std::string> args4 = {
                "--rs",
                "../examples/test.rvas",
                "-v",
            };

            parser.ParseArgs(args4);
        }
    }
    catch (const args::Help&) {
        std::cout << parser;
        return 0;
    }
    catch (const args::ParseError& e) {
        std::cerr << e.what() << std::endl;
        std::cout << parser;
        return 1;
    }
    catch (const args::ValidationError& e) {
        std::cerr << e.what() << std::endl;
        std::cout << parser;
        return 1;
    }

    if (verbose) {
        rvm::log::LogLevel = rvm::log::LogCategory::ALL;
    }

    if (inputFiles->size() == 0) MainError("Expected input file(s).");

    if (runFromAssembler) {
        if (inputFiles->size() != 1) {
            MainError("Expected only a single input when using assembler.");
        }

        auto code = rvm::assembler::Parser::FromFile(inputFiles.Get()[0]);

        rvm::exec::VirtualMachine vm(stackSize.Get() * 1024 * 1024 / 8, localSize.Get() * 1000);
        vm.LoadBytecode(code);
        vm.Run(entryPoint.Get());
        return 0;
    }

    if (assemble) {
        if (inputFiles->size() != 1) {
            MainError("Expected only a single input when using assembler.");
        }

        auto code = rvm::assembler::Parser::FromFile(inputFiles.Get()[0]);
        std::ofstream outStream(outputFile.Get());
        if (!outStream.is_open()) MainError("Could not open output file.");
        
        auto serialized = rvm::loading::Serialize(code);
        outStream << serialized;
    }
    else {
        std::vector<rvm::loading::GlobalDataUnit> code;
        for (auto& inFile : inputFiles) {
            std::ifstream inStream(inFile);
            if (!inStream.is_open()) MainError("Could not open input file.");

            std::stringstream buf;
            buf << inStream.rdbuf();

            auto deserialized = rvm::loading::Deserialize(buf.str());

            for (auto& unit : deserialized) code.push_back(unit);
        }

        rvm::exec::VirtualMachine vm(stackSize.Get() * 1024 * 1024 / 8, localSize.Get() * 1000);
        vm.LoadBytecode(code);
        vm.Run(entryPoint.Get());
    }
    return 0;
}