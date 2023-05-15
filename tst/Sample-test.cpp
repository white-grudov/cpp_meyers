#include "gtest/gtest.h"
#include "Sample.h"

TEST(blaTest, test1) {
    //arrange
    //act
    //assert
    EXPECT_EQ (Sample::add (2, 3),  5);
    EXPECT_EQ (Sample::add (4, -1), 3);
    EXPECT_EQ (Sample::add (50, 50), 100);
}