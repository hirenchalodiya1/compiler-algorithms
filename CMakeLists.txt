# CMake version
cmake_minimum_required(VERSION 3.14)
# project name
project(compiler_algorithms)

# flags
set(CMAKE_CXX_STANDARD 14)

# files

# include
include_directories(include src libs)

# targets
#add_executable(recursiveparser main.cpp syntex_analysis/cfg.h syntex_analysis/parser.h syntex_analysis/topdownparser/recursiveparser.h)
#add_executable(predictiveparser main.cpp syntex_analysis/cfg.h syntex_analysis/parser.h syntex_analysis/topdownparser/predictiveparser.h)
#add_executable(compiler_algorithms main.cpp automatas/nfa.h automatas/test.h automatas/convert.h automatas/dfa.cpp automatas/dfa.h print.cpp)
#add_executable(test test.cpp print.h)

# external libraries

# unit testing