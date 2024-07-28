#include <args.hxx>

#include "exec/vmachine.hpp"
#include "loading/loading.hpp"
#include "log/log.hpp"
#include "assembler/parser.hpp"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>

#define USE_DEBUG_ARGS 1

int main(int argc, char** argv) {
    args::ArgumentParser parser("");
    args::HelpFlag help(parser, "help", "Help page", {'h', "help"});
    

    args::Group executeFlags(parser, "Execution options", args::Group::Validators::DontCare);
    args::ValueFlag<unsigned int> stackSize(executeFlags, "size", "Stack size (in MB).", {"xmS"}, 1);
    args::ValueFlag<unsigned int> localSize(executeFlags, "N", "Number of locals pre-allocated (in thousands).", {"xmL"}, 8);
    args::ValueFlag<std::string> entryPoint(executeFlags, "func", "Name of entry function (defaults to \"main\").", {'e', "entry"}, "main");


    args::Group assemblyGroup(parser, "Assembler options", args::Group::Validators::AllOrNone);
    args::Flag assemble(assemblyGroup, "", "Assembler input file (max. 1).", {'a', "asm"});
    args::ValueFlag<std::string> outputFile(assemblyGroup, "out", "Specify output file (if using assembler).", {'o', "output"});

    args::Flag runFromAssembler(parser, "", "Run a single RVM assembly file directly.", {"rs"});
    
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
                "../examples/test.rvas"
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

    if (inputFiles->size() == 0) rvm::log::LogError("Expected input file(s).");

    if (runFromAssembler) {
        if (inputFiles->size() != 1) {
            rvm::log::LogError("Expected only a single input when using assembler.");
        }

        auto code = rvm::assembler::Parser::FromFile(inputFiles.Get()[0]);

        rvm::exec::VirtualMachine vm(stackSize.Get() * 1024 * 1024, localSize.Get() * 1000);
        vm.LoadBytecode(code);
        vm.Run(entryPoint.Get());
        return 0;
    }

    if (assemble) {
        if (inputFiles->size() != 1) {
            rvm::log::LogError("Expected only a single input when using assembler.");
        }

        auto code = rvm::assembler::Parser::FromFile(inputFiles.Get()[0]);
        std::ofstream outStream(outputFile.Get());
        if (!outStream.is_open()) rvm::log::LogError("Could not open output file.");
        
        auto serialized = rvm::loading::Serialize(code);
        outStream << serialized;
    }
    else {
        std::vector<rvm::loading::GlobalDataUnit> code;
        for (auto& inFile : inputFiles) {
            std::ifstream inStream(inFile);
            if (!inStream.is_open()) rvm::log::LogError("Could not open input file.");

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