cmake_minimum_required(VERSION 3.14)
project(CPP_Project
    VERSION 1.0
    DESCRIPTION "CMake project"
    LANGUAGES CXX)

include_directories(src)

# enable_testing()

add_subdirectory(src)
add_subdirectory(tst)
add_subdirectory(lib/googletest)
