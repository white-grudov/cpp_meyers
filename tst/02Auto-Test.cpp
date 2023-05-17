#include "gtest/gtest.h"
#include <functional>
#include <memory>
#include <typeinfo> 

#include "02Auto.h"

TEST(AutoTest, FunctionDeclarationComparisonStdFunctionVsAuto)
{
    std::function<bool(const std::unique_ptr<Data>&, const std::unique_ptr<Data>&)> compareGreaterNonAuto =
        [](const std::unique_ptr<Data>& lhs, const std::unique_ptr<Data>& rhs) { return *lhs > *rhs; };
    
    auto compareGreaterAuto = [](const auto& lhs, const auto& rhs) { return *lhs > *rhs; };

    std::unique_ptr<Data> x = std::make_unique<Data>(5);
    std::unique_ptr<Data> y = std::make_unique<Data>(2);

    // auto lambda expression returns true as expected (given that data in x is greater than data in y)
    EXPECT_TRUE(compareGreaterAuto(x, y));
    EXPECT_EQ(compareGreaterAuto(x, y), *x > *y);

    // function template and auto lambda expression return the same result
    EXPECT_EQ(compareGreaterNonAuto(x, y), compareGreaterAuto(x, y));

    // function template takes more memory than auto lambda expression
    EXPECT_GE(sizeof(compareGreaterNonAuto), sizeof(compareGreaterAuto));
}

// To check compatibility issues with Win64 platform
#ifdef _WIN64
TEST(AutoTest, VectorSizeAutoMismatch)
{
    std::vector<int> vec { 1, 2, 3, 4, 5 };

    unsigned sizeUnsigned = vec.size();
    auto sizeAuto = vec.size();

    EXPECT_NE(sizeof(sizeUnsigned), sizeof(sizeAuto));
    EXPECT_EQ(sizeof(sizeUnsigned), 4); // 32 bits
    EXPECT_EQ(sizeof(sizeAuto), 8); // 64 bits
}
#endif

TEST(AutoTest, UndefinedBehaviorWithAutoAndHiddenProxyType)
{
    OwnNumber x(5);
    OwnNumber y(2);

    OwnNumber addExplicitType = x + y;
    auto addAutoType = x + y;

    EXPECT_EQ(addExplicitType.getValue(), addAutoType.getValue());
    EXPECT_EQ(addExplicitType.getValue(), 7);
    EXPECT_EQ(addAutoType.getValue(), 7);
    
    EXPECT_NO_THROW(++addExplicitType);
    EXPECT_NO_THROW(++static_cast<OwnNumber>(addAutoType)); // explicitly typed initializer idiom
    EXPECT_THROW(++addAutoType, std::runtime_error);
}