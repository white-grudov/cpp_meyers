#include "gtest/gtest.h"
#include <memory>

#include "06Lambdas.h"

// doesn't work with optimization compiler flags
#ifndef __OPTIMIZE__
TEST(LambdasTestItem31, DefaultCaptureModeDangleRef)
{
    FilterContainer filters;
    auto firstValue = 4;
    auto secondValue = 5;

    addFuncDangleRef(filters, firstValue);

    auto result = filters[0](secondValue);
    EXPECT_NE(firstValue + secondValue + 1, result);
}
#endif

TEST(LambdasTestItem31, DefaultCaptureModePassByValue)
{
    FilterContainer filters;
    auto firstValue = 4;
    auto secondValue = 5;

    addFuncByValue(filters, firstValue);

    auto result = filters[0](secondValue);
    EXPECT_EQ(firstValue + secondValue + 1, result);
}

TEST(LambdasTestItem31, DefaultCaptureModeByValueStaticAnnotation)
{
    FilterContainer filters;
    auto firstValue = 4;
    auto secondValue = 5;

    addFuncStatic(filters, firstValue);

    auto result = filters[0](secondValue);
    EXPECT_NE(firstValue + secondValue + 1, result);
    EXPECT_EQ(firstValue + secondValue + 2, result);
}

TEST(LambdasTestItem31, CaptureByValueDanglingPointer)
{
    auto divisor = 7;
    FilterContainer filters;

    {
        auto divisorHelper = std::make_unique<DivisionHelper>(divisor);
        divisorHelper->addFilterUnsafe(filters);
    }
    auto divident = 49;
    auto result = filters[0](divident);

    EXPECT_NE(divident / divisor, result);
}

TEST(LambdasTestItem31, CaptureByValueCopiedDataMember)
{
    auto divisor = 7;
    FilterContainer filters;

    {
        auto divisorHelper = std::make_unique<DivisionHelper>(divisor);
        divisorHelper->addFilterSafe(filters);
    }
    auto divident = 49;
    auto result = filters[0](divident);

    EXPECT_EQ(divident / divisor, result);
}

TEST(LambdasTestItem32, InitCapturesToMoveObject)
{
    auto request = std::make_unique<CustomerRequest>(42);
    request->complete();

    auto isRequestCompleted = [request = std::move(request)]() 
        { return request->isProcessed() && request->isPaid(); };

    EXPECT_TRUE(isRequestCompleted());
}

TEST(LambdasTestItem32, InitCapturesToInitializePtr)
{
    auto isRequestCompleted = [request = std::make_unique<CustomerRequest>(42)]() 
        { return request->isProcessed() && request->isPaid(); };

    EXPECT_FALSE(isRequestCompleted());
}

TEST(LambdasTestItem33, GenericLambdasAndForwarding)
{
    auto func = [](auto&& param) { return processValue(std::forward<decltype(param)>(param)); };

    auto lvalue = 42;
    auto lvalueResult = func(lvalue);

    EXPECT_EQ(ValueType::Lvalue, lvalueResult);

    auto rvalueResult = func(42);
    EXPECT_EQ(ValueType::Rvalue, rvalueResult);
}