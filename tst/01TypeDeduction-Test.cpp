#include "gtest/gtest.h"
#include <typeinfo>

#include "01TypeDeduction.h"

template<typename T>
void testTypeDeduction(const T& value) {
    EXPECT_EQ(typeid(value), typeid(int));
}

TEST(TypeDeductionTest, AutoTypeDeductionTemplate) {
    auto x = 5;
    testTypeDeduction(x);

    auto& y = x;
    testTypeDeduction(y);

    const auto& z = x;
    testTypeDeduction(z);

    auto&& w = 5;
    testTypeDeduction(w);

    const auto&& v = 5;
    testTypeDeduction(v);
}

TEST(TypeDeductionTest, AutoTypeFuncDeduction) {
    auto result = TypeDeduction::add(5, 6);

    EXPECT_EQ(typeid(result), typeid(int));
    EXPECT_EQ(result, 11);
}

TEST(TypeDeductionTest, AutoTypeDeduction) {
    auto number = 42;
    auto& ref = number;
    const auto* ptr = &number;

    EXPECT_EQ(typeid(number), typeid(int));
    EXPECT_EQ(typeid(ref), typeid(int&));
    EXPECT_EQ(typeid(ptr), typeid(const int*));
}

TEST(TypeDeductionTest, DecltypeTypeDeduction) {
    int a = 5;
    double b = 3.14;

    decltype(a * b) result = TypeDeduction::multiply(a, b);

    EXPECT_EQ(typeid(result), typeid(double));
    EXPECT_EQ(result, a * b);
}