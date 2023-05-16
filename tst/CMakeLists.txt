set(BINARY ${CMAKE_PROJECT_NAME}_tst)

# file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)

set(SOURCES ${TEST_SOURCES})

enable_testing()

add_executable(
    01TypeDeduction-Test
    01TypeDeduction-Test.cpp
)
target_link_libraries(
    01TypeDeduction-Test
    GTest::gtest_main
    CPP_Project_Examples
)

include(GoogleTest)
gtest_discover_tests(01TypeDeduction-Test)
