cmake_minimum_required(VERSION 3.20)
project(RVM)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-parameter -Wno-unused-variable)

find_package(args REQUIRED CONFIG)

set(sources
    src/exec/instruction.cpp
    src/exec/vmachine.cpp
    src/exec/vminshandlers.cpp

    src/log/log.cpp

    src/assembler/scanner.cpp
    src/assembler/parser.cpp

    src/loading/loading.cpp
)

add_library(rvm_internal STATIC ${sources})


add_executable(rvm src/main.cpp)
target_link_libraries(rvm rvm_internal)

set_target_properties(rvm PROPERTIES LINK_FLAGS_RELEASE -s)

install(TARGETS rvm RUNTIME DESTINATION bin)