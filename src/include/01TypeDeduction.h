#pragma once

#include <iostream>
#include <typeinfo>

/*
 * Template type deduction
 */

// function is invoked if argument passed in template is const
inline void reset(const int &value) noexcept
{
    // since value is const, we cannot reset it, hence return
    return;
}
// function is invoked if argument passed in template is not const
inline void reset(int &value) noexcept
{
    // since value is not const, we can reset it
    value = -1;
}

template <typename T>
inline auto resetRef(T &data) noexcept
{
    reset(data);
    return data;
}

template <typename T>
inline auto resetConstRef(const T &data) noexcept
{
    reset(data);
    return data;
}

template <typename T>
inline auto resetUniRef(T &&data) noexcept
{
    reset(data);
    return data;
}

template <typename T>
inline auto resetPlain(T data) noexcept
{
    reset(data);
    return data;
}

/*
 * Auto type deduction
 */

template <typename T>
inline auto getAutoValue(T &value) noexcept
{
    return value;
}

template <typename T>
inline auto testAuto(T &value) noexcept
{
    auto autoReturnValue = getAutoValue(value);
    reset(autoReturnValue);
    return autoReturnValue;
}

/*
 * Decltype type deduction
 */

template <typename T>
inline decltype(auto) getDecltypeValue(T &value) noexcept
{
    return value;
}

template <typename T>
inline auto testDecltype(T &value) noexcept
{
    auto &decltypeReturnValue = getDecltypeValue(value);
    reset(decltypeReturnValue);
    return decltypeReturnValue;
}
