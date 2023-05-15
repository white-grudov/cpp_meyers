cmake_minimum_required(VERSION 3.10)
project(CPP_Project
    VERSION 1.0
    DESCRIPTION "CMake project"
    LANGUAGES CXX)
add_executable(CPP_Project
    src/main.cpp)
target_compile_features(CPP_Project PRIVATE cxx_std_20)
target_compile_options(CPP_Project PRIVATE -Wall)