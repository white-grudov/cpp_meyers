#include "gtest/gtest.h"

#include "01TypeDeduction.h"

TEST(TypeDeductionTest, TemplateDeductionByReference) {
    int initialValue = 5;

    auto x = initialValue;
    const auto cx = x;

    EXPECT_EQ(TypeDeduction::resetRef(x), -1);
    EXPECT_EQ(TypeDeduction::resetRef(cx), initialValue);
}

TEST(TypeDeductionTest, TemplateDeductionByConstReference) {
    int initialValue = 5;

    auto x = initialValue;
    const auto cx = x;

    EXPECT_EQ(TypeDeduction::resetConstRef(x), initialValue);
    EXPECT_EQ(TypeDeduction::resetConstRef(cx), initialValue);
}

TEST(TypeDeductionTest, TemplateDeductionByUniversalReference) {
    int initialValue = 5;

    auto x = initialValue;
    const auto cx = x;

    EXPECT_EQ(TypeDeduction::resetUniRef(x), -1);
    EXPECT_EQ(TypeDeduction::resetUniRef(cx), initialValue);
}

TEST(TypeDeductionTest, TemplateDeductionByValue) {
    int initialValue = 5;

    auto x = initialValue;
    const auto cx = x;

    EXPECT_EQ(TypeDeduction::resetPlain(x), -1);
    EXPECT_EQ(TypeDeduction::resetPlain(cx), -1);
}

TEST(TypeDeductionTest, AutoDeduction) {
    int initialValue = 5;

    auto x = initialValue;
    const auto cx = x;

    EXPECT_EQ(TypeDeduction::testAuto(x), -1);
    EXPECT_EQ(TypeDeduction::testAuto(cx), -1);
}

TEST(TypeDeductionTest, DecltypeDeduction) {
    int initialValue = 5;

    auto x = initialValue;
    const auto cx = x;

    EXPECT_EQ(TypeDeduction::testDecltype(x), -1);
    EXPECT_EQ(TypeDeduction::testDecltype(cx), initialValue);
}