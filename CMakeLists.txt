cmake_minimum_required(VERSION 3.29)
project(GA)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
add_executable(GA main.cpp
        MyGA.cpp
        MyGA.h)
