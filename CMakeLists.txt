cmake_minimum_required(VERSION 3.16.0)
project(cb
    VERSION 1.0
    DESCRIPTION "A simple terminal clipboard for UNIX-like systems"
    LANGUAGES CXX)

add_executable(cb
    src/cb.cpp
    src/tags.hpp)

target_compile_options(cb PUBLIC -pedantic -Wall -Werror)