#include "gtest/gtest.h"

#include "01TypeDeduction.h"

TEST(TypeDeductionTestItem1, TemplateDeductionByReference)
{
    int initialValue = 5;

    auto x = initialValue;
    const auto cx = x;

    EXPECT_EQ(resetRef(x), -1);
    EXPECT_EQ(resetRef(cx), initialValue);
}

TEST(TypeDeductionTestItem1, TemplateDeductionByConstReference)
{
    int initialValue = 5;

    auto x = initialValue;
    const auto cx = x;

    EXPECT_EQ(resetConstRef(x), initialValue);
    EXPECT_EQ(resetConstRef(cx), initialValue);
}

TEST(TypeDeductionTestItem1, TemplateDeductionByUniversalReference)
{
    int initialValue = 5;

    auto x = initialValue;
    const auto cx = x;

    EXPECT_EQ(resetUniRef(x), -1);
    EXPECT_EQ(resetUniRef(cx), initialValue);
}

TEST(TypeDeductionTestItem1, TemplateDeductionByValue)
{
    int initialValue = 5;

    auto x = initialValue;
    const auto cx = x;

    EXPECT_EQ(resetPlain(x), -1);
    EXPECT_EQ(resetPlain(cx), -1);
}

TEST(TypeDeductionTestItem2, AutoTypeDeduction)
{
    int initialValue = 5;

    auto x = initialValue;
    const auto cx = x;

    EXPECT_EQ(testAuto(x), -1);
    EXPECT_EQ(testAuto(cx), -1);
}

TEST(TypeDeductionTestItem3, DecltypeDeduction)
{
    int initialValue = 5;

    auto x = initialValue;
    const auto cx = x;

    EXPECT_EQ(testDecltype(x), -1);
    EXPECT_EQ(testDecltype(cx), initialValue);
}