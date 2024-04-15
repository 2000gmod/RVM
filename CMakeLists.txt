cmake_minimum_required(VERSION 3.5)
project(RVM)

set(CMAKE_CXX_STANDARD 23)

add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-parameter -Wno-unused-variable)
add_compile_options(-fno-strict-aliasing)

#add_compile_options(-fsanitize=undefined -fsanitize=address)
#add_link_options(-fsanitize=undefined -fsanitize=address)

set(sources
    src/main.cpp
    src/exec/instruction.cpp
    src/exec/vmachine.cpp
    src/exec/vminshandlers.cpp
)

add_executable(rvm ${sources})

set_target_properties(rvm PROPERTIES LINK_FLAGS_RELEASE -s)

install(TARGETS rvm RUNTIME DESTINATION bin)
include(CPack)