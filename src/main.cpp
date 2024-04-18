#include <fstream>
#include <sstream>

#include "assembler/scanner.hpp"

using namespace rvm;

int main() {
    std::ifstream in("/home/yo/VisualStudioCode/C++/RVMv1/examples/test.rvas");
    if (!in.is_open()) {
        return 1;
    }
    std::stringstream buf;
    buf << in.rdbuf();
    assembler::Scanner scanner(buf.str());
    auto out = scanner.Tokenize();

    
}