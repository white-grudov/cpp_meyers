set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB PROJECT_SOURCES ${PROJECT_DIR}/*.cpp)

add_library(
    CPP_Project_Examples
    ${PROJECT_SOURCES}
)

target_include_directories(CPP_Project_Examples PUBLIC include)