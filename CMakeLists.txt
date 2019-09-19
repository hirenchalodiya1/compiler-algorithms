cmake_minimum_required(VERSION 3.14)
project(compiler_algorithms)

set(CMAKE_CXX_STANDARD 14)

add_executable(compiler_algorithms main.cpp syntex_analysis/cfg.h syntex_analysis/topdownparser/firstfollow.h syntex_analysis/topdownparser/predictiveparsing.h)
