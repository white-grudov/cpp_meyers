cmake_minimum_required(VERSION 3.16)
project(CPP_Project LANGUAGES CXX)

include_directories(C:/msys64/mingw64/include/c++/12.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_subdirectory(src)
add_subdirectory(tst)
add_subdirectory(lib/googletest)
