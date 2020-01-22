# CMake version
cmake_minimum_required(VERSION 3.14)
# project name
project(compiler_algorithms VERSION 1.0.0 DESCRIPTION "Different compiler algorithms")

# flags
set(CMAKE_CXX_STANDARD 14)

# files

# include
include_directories(include)

# targets
add_executable(dfa tests/automatas/dfa.cpp)
add_executable(nfa tests/automatas/nfa.cpp)
add_executable(nfa_to_dfa tests/automatas/nfa_to_dfa.cpp)
add_executable(dev_test tests/test.cpp)

# external libraries

# unit testing
enable_testing()
add_test(dfa_test dfa)
add_test(nfa_test nfa)
add_test(nfa_to_dfa_test nfa_to_dfa)
