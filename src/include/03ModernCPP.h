#pragma once

#include <initializer_list>
#include <vector>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>

/*
 * Item 7: Distinguish between () and {} when creating objects
 */

class Value
{
private:
    int value_;

public:
    Value(int value) : value_(value) {}
    int getValue() const { return value_; }
    int& getValue() { return value_; }
};

class ValueFlag
{
private:
    int value_;
    bool flag_;
    std::vector<int> data_;

    bool firstCtorCalled_ = false;
    bool secondCtorCalled_ = false;
public:
    ValueFlag(int value, bool flag) : value_(value), flag_(flag), firstCtorCalled_(true) {}
    ValueFlag(std::initializer_list<int> values) : value_(-1), flag_(false), secondCtorCalled_(true)
    {
        for (const auto& value : values)
        {
            data_.push_back(value);
        }
    }

    operator int() const { return value_; }

    int getValue() const { return value_; }
    int& getValue() { return value_; }
    bool getFlag() const { return flag_; }
    bool& getFlag() { return flag_; }
    const std::vector<int>& getData() const { return data_; }
    std::vector<int>& getData() { return data_; }

    bool isFirstCtorCalled() const { return firstCtorCalled_; }
    bool isSecondCtorCalled() const { return secondCtorCalled_; }
};

/*
 * Item 8: Prefer nullptr to 0 and NULL
 */

class CheckPointer
{
private:
    bool isPointer_;
public:
    CheckPointer(int* ptr) : isPointer_(true) {}
    CheckPointer(int value) : isPointer_(false) {}

    bool isPointer() const { return isPointer_; }
};

/*
 * Item 9: Prefer alias declarations to typedefs
 */

int increment(int x)
{
    return x + 1;
}

typedef int(*IncTypedef)(int);
using IncAlias = int(*)(int);

template <typename T>
struct MatrixTypedef
{
    typedef std::vector<std::vector<T>> type;
};

template <typename T>
using MatrixAlias = std::vector<std::vector<T>>;

/*
 * Item 10: Prefer scoped enums to unscoped enums
 */

// separate namespace to avoid name collision
namespace Enums
{
    enum ColorUnscoped
    {
        Red,
        Green,
        Blue
    };
    enum class ColorScoped
    {
        Red,
        Green,
        Blue
    };

    template<typename E>
    constexpr auto getType(E enumerator) noexcept
    {
        return static_cast<std::underlying_type_t<E>>(enumerator);
    }

    using RGB = std::tuple<std::uint8_t, std::uint8_t, std::uint8_t>;
}

/*
 * Item 11: Prefer deleted functions to private undefined ones
 */

class ProcessPointerNoDelete
{
private:
    bool wrongPtrType_;
public:
    ProcessPointerNoDelete() : wrongPtrType_(false) {}

    template <typename T>
    void takePtr(T* ptr)
    {
        if constexpr (std::is_same_v<T, void> || std::is_same_v<T, char>)
        {
            wrongPtrType_ = true;
            return;
        }
        // do stuff...
    }

    bool isWrongPtrType() const { return wrongPtrType_; }
};

class ProcessPointerWithDelete
{
public:
    ProcessPointerWithDelete() {}

    template <typename T>
    void takePtr(T* ptr)
    {
        // do stuff...
    }
};

template <>
void ProcessPointerWithDelete::takePtr<void>(void*) = delete;

template <>
void ProcessPointerWithDelete::takePtr<char>(char*) = delete;

/*
 * Item 12: Declare overriding functions override
 */

class Base
{
private:
    static constexpr int value_ = 0;
public:
    virtual int func1() const { return value_; }
    virtual int func2(int) { return value_; }
    virtual int func3() & { return value_; }
    int func4() const { return value_; }
};
class ProblematicDerived : public Base
{
private:
    static constexpr int value_ = 1;
public:
    // overrides will cause compilation error
    int func1() { return value_; }
    int func2(unsigned int) { return value_; }
    int func3() && { return value_; }
    int func4() const { return value_; }
};

class Component
{
private:
    std::vector<int> values_;
public:
    Component(std::initializer_list<int> values)
    {
        for (const auto& value : values)
        {
            values_.push_back(value);
        }
    }
    std::vector<int> getValues() & { return values_; }
    std::vector<int> getValues() && 
    { 
        values_[0] = 0;
        return std::move(values_); 
    }
};

Component makeComponent()
{
    return Component { -1 };
}

/*
 * Item 13: Prefer const_iterators to iterators
 */

