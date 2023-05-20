#pragma once

#include <functional>
#include <vector>

/*
 * Item 31: Avoid default capture modes.
 */

using FilterContainer = std::vector<std::function<int(int)>>;

int incrementDangleRef(int value)
{
    return value + 1;
}

void addFuncDangleRef(FilterContainer& filters, int num)
{
    auto initialValue = incrementDangleRef(num);
    filters.emplace_back([&initialValue](int value) { return initialValue + value; });
}

void addFuncByValue(FilterContainer& filters, int num)
{
    auto initialValue = incrementDangleRef(num);
    filters.emplace_back([=](int value) { return initialValue + value; });
}

void addFuncStatic(FilterContainer& filters, int num)
{
    static auto initialValue = incrementDangleRef(num);
    filters.emplace_back([=](int value) { return initialValue + value; });
    ++initialValue;
}

class DivisionHelper
{
    int divisor_;

public:
    DivisionHelper(int divisor) : divisor_(divisor) {}

    void addFilterUnsafe(FilterContainer& filters)
    {
        // implicit capture of 'this' via '[=]' is deprecated in C++20
        filters.emplace_back([=](int value) { return value / divisor_; });
    }
    void addFilterSafe(FilterContainer& filters)
        {
        filters.emplace_back([divisor = divisor_](int value) { return value / divisor; });
    }
};

/*
 * Item 32: Use init capture to move objects into closures.
 */

class CustomerRequest
{
    int id_;
    bool isProcessed_;
    bool isPaid_;

public:
    CustomerRequest(int id) : id_(id), isProcessed_(false), isPaid_(false) {}

    void complete() 
    {
        isProcessed_ = true;
        isPaid_ = true;
    }
    bool isProcessed() const { return isProcessed_; }
    bool isPaid() const { return isPaid_; }
};

/*
 * Item 33: Use decltype on auto&& parameters to std::forward them.
 */

enum class ValueType { Lvalue, Rvalue };

ValueType processValue(int& value)
{
    return ValueType::Lvalue;
}

ValueType processValue(int&& value)
{
    return ValueType::Rvalue;
}