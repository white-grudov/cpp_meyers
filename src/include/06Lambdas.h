#pragma once

#include <functional>
#include <vector>

/*
 * Item 31: Avoid default capture modes.
 */

using FilterContainer = std::vector<std::function<int(int)>>;

inline int incrementDangleRef(int value) noexcept
{
    return value + 1;
}

inline void addFuncDangleRef(FilterContainer& filters, int num) noexcept
{
    auto initialValue = incrementDangleRef(num);
    filters.emplace_back([&initialValue](int value) { return initialValue + value; });
}

inline void addFuncByValue(FilterContainer& filters, int num) noexcept
{
    auto initialValue = incrementDangleRef(num);
    filters.emplace_back([=](int value) { return initialValue + value; });
}

inline void addFuncStatic(FilterContainer& filters, int num) noexcept
{
    static auto initialValue = incrementDangleRef(num);
    filters.emplace_back([=](int value) { return initialValue + value; });
    ++initialValue;
}

class DivisionHelper
{
    int divisor;

public:
    DivisionHelper(int divisor) : divisor { divisor } {}

    void addFilterUnsafe(FilterContainer& filters)
    {
        // implicit capture of 'this' via '[=]' is deprecated in C++20
        filters.emplace_back([=](int value) { return value / divisor; });
    }
    void addFilterSafe(FilterContainer& filters)
    {
        filters.emplace_back([divisor = divisor](int value) { return value / divisor; });
    }
};

/*
 * Item 32: Use init capture to move objects into closures.
 */

class CustomerRequest
{
    int id;
    bool processed;
    bool paid;

public:
    CustomerRequest(int id) : id { id }, processed { false }, paid { false } {}

    void complete() 
    {
        processed = true;
        paid = true;
    }
    bool isProcessed() const { return processed; }
    bool isPaid() const { return paid; }
};

/*
 * Item 33: Use decltype on auto&& parameters to std::forward them.
 */

enum class ValueType { Lvalue, Rvalue };

inline ValueType processValue(int& value) noexcept
{
    return ValueType::Lvalue;
}

inline ValueType processValue(int&& value) noexcept
{
    return ValueType::Rvalue;
}