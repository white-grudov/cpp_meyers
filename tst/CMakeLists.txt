set(BINARY ${CMAKE_PROJECT_NAME}_tst)

set(SOURCES ${TEST_SOURCES})

enable_testing()

add_executable(
    CPP_Project_Tests
    01TypeDeduction-Test.cpp
    02Auto-Test.cpp
)
target_link_libraries(
    CPP_Project_Tests
    GTest::gtest_main
    CPP_Project_Examples
)

include(GoogleTest)
gtest_discover_tests(CPP_Project_Tests)
