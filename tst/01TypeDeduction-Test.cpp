#include "gtest/gtest.h"

#include "01TypeDeduction.h"

TEST(TypeDeductionTest, TemplateDeductionByReference)
{
    int initialValue = 5;

    auto x = initialValue;
    const auto cx = x;

    EXPECT_EQ(resetRef(x), -1);
    EXPECT_EQ(resetRef(cx), initialValue);
}

TEST(TypeDeductionTest, TemplateDeductionByConstReference)
{
    int initialValue = 5;

    auto x = initialValue;
    const auto cx = x;

    EXPECT_EQ(resetConstRef(x), initialValue);
    EXPECT_EQ(resetConstRef(cx), initialValue);
}

TEST(TypeDeductionTest, TemplateDeductionByUniversalReference)
{
    int initialValue = 5;

    auto x = initialValue;
    const auto cx = x;

    EXPECT_EQ(resetUniRef(x), -1);
    EXPECT_EQ(resetUniRef(cx), initialValue);
}

TEST(TypeDeductionTest, TemplateDeductionByValue)
{
    int initialValue = 5;

    auto x = initialValue;
    const auto cx = x;

    EXPECT_EQ(resetPlain(x), -1);
    EXPECT_EQ(resetPlain(cx), -1);
}

TEST(TypeDeductionTest, AutoTypeDeduction)
{
    int initialValue = 5;

    auto x = initialValue;
    const auto cx = x;

    EXPECT_EQ(testAuto(x), -1);
    EXPECT_EQ(testAuto(cx), -1);
}

TEST(TypeDeductionTest, DecltypeDeduction)
{
    int initialValue = 5;

    auto x = initialValue;
    const auto cx = x;

    EXPECT_EQ(testDecltype(x), -1);
    EXPECT_EQ(testDecltype(cx), initialValue);
}