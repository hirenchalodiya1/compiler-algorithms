cmake_minimum_required(VERSION 3.14)
project(compiler_algorithms)

set(CMAKE_CXX_STANDARD 14)

add_executable(compiler_algorithms main.cpp cfg.h firstfollow.h predictiveparsing.h)