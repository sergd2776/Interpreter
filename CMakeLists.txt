cmake_minimum_required(VERSION 3.16)
project(Interpreter)

set(CMAKE_CXX_STANDARD 11)

add_executable(Interpreter main.cpp Data.h Functions.h Variables.h Types.h Commands.h)