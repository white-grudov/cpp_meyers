#pragma once

#include <iostream>
#include <typeinfo>

/*
 * Template type deduction
 */

// function is invoked if argument passed in template is const
void reset(const int &value)
{
    // since value is const, we cannnot reset it, hence return
    return;
}
// function is invoked if argument passed in template is not const
void reset(int &value)
{
    // since value is not const, we can reset it
    value = -1;
}

template <typename T>
auto resetRef(T &data)
{
    reset(data);
    return data;
}

template <typename T>
auto resetConstRef(const T &data)
{
    reset(data);
    return data;
}

template <typename T>
auto resetUniRef(T &&data)
{
    reset(data);
    return data;
}

template <typename T>
auto resetPlain(T data)
{
    reset(data);
    return data;
}

/*
 * Auto type deduction
 */

template <typename T>
auto getAutoValue(T &value)
{
    return value;
}

template <typename T>
auto testAuto(T &value)
{
    auto autoReturnValue = getAutoValue(value);
    reset(autoReturnValue);
    return autoReturnValue;
}

/*
 * Decltype type deduction
 */

template <typename T>
decltype(auto) getDecltypeValue(T &value)
{
    return value;
}

template <typename T>
auto testDecltype(T &value)
{
    auto &decltypeReturnValue = getDecltypeValue(value);
    reset(decltypeReturnValue);
    return decltypeReturnValue;
}
