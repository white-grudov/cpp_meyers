set(BINARY ${CMAKE_PROJECT_NAME}_tst)

set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB TEST_SOURCES ${TEST_DIR}/*.cpp)

include(GoogleTest)

add_executable(
    ${BINARY}
    ${TEST_SOURCES}
)

target_link_libraries(
    ${BINARY}
    GTest::gtest_main
    CPP_Project_Examples
)

gtest_discover_tests(${BINARY})
