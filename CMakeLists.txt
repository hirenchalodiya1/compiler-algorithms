cmake_minimum_required(VERSION 3.14)
project(compiler_algorithms)

set(CMAKE_CXX_STANDARD 14)

add_executable(recursiveparser main.cpp syntex_analysis/cfg.h syntex_analysis/parser.h syntex_analysis/topdownparser/recursiveparser.h)
add_executable(predictiveparser main.cpp syntex_analysis/cfg.h syntex_analysis/parser.h syntex_analysis/topdownparser/predictiveparser.h)
add_executable(compiler_algorithms main.cpp)
add_executable(test test.cpp)
