#pragma once

#include <initializer_list>
#include <vector>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>
#include <numeric>
#include <stdexcept>
#include <random>
#include <thread>
#include <mutex>

/*
 * Item 7: Distinguish between () and {} when creating objects
 */

class Value
{
    int value;

public:
    Value(int val) : value { val } {}
    int getValue() const noexcept { return value; }
    int& getValue() noexcept { return value; }
};

class ValueFlag
{
    int value;
    bool flag;
    std::vector<int> data;

    bool firstCtorCalled = false;
    bool secondCtorCalled = false;
public:
    ValueFlag(int value, bool flag) : value { value }, flag { flag }, firstCtorCalled { true } {}
    ValueFlag(std::initializer_list<int> values) : value { -1 }, flag { false }, secondCtorCalled { true }
    {
        for (const auto& value : values)
        {
            data.push_back(value);
        }
    }

    operator int() const noexcept { return value; }

    int getValue() const noexcept { return value; }
    int& getValue() noexcept { return value; }
    bool getFlag() const noexcept { return flag; }
    bool& getFlag() noexcept { return flag; }
    const std::vector<int>& getData() const noexcept { return data; }
    std::vector<int>& getData() noexcept { return data; }

    bool isFirstCtorCalled() const noexcept { return firstCtorCalled; }
    bool isSecondCtorCalled() const noexcept { return secondCtorCalled; }
};

/*
 * Item 8: Prefer nullptr to 0 and NULL
 */

class CheckPointer
{
    bool isPointerFlag;
public:
    CheckPointer(int* ptr) : isPointerFlag { true } {}
    CheckPointer(int value) : isPointerFlag { false } {}

    bool isPointer() const noexcept { return isPointerFlag; }
};

/*
 * Item 9: Prefer alias declarations to typedefs
 */

inline int increment(int x) noexcept
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
    inline constexpr auto getType(E enumerator) noexcept
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
    bool wrongPtrType;
public:
    ProcessPointerNoDelete() : wrongPtrType { false } {}

    template <typename T>
    void takePtr(T* ptr)
    {
        if constexpr (std::is_same_v<T, void> || std::is_same_v<T, char>)
        {
            wrongPtrType = true;
            return;
        }
        // do stuff...
    }

    bool isWrongPtrType() const { return wrongPtrType; }
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
    static constexpr int value = 0;
public:
    virtual int func1() const noexcept { return value; }
    virtual int func2(int) noexcept { return value; }
    virtual int func3() & noexcept { return value; }
    int func4() const noexcept { return value; }
};
class ProblematicDerived : public Base
{
    static constexpr int value = 1;
public:
    // overrides will cause compilation error
    int func1() noexcept { return value; }
    int func2(unsigned int) noexcept { return value; }
    int func3() && noexcept { return value; }
    int func4() const noexcept { return value; }
};

class Component
{
    std::vector<int> values;
public:
    Component(std::initializer_list<int> vals)
    {
        for (const auto& value : vals)
        {
            values.push_back(value);
        }
    }
    std::vector<int> getValues() & noexcept { return values; }
    std::vector<int> getValues() &&
    { 
        values[0] = 0;
        return std::move(values); 
    }
};

inline Component makeComponent() noexcept
{
    return Component { -1 };
}

/*
 * Item 13: Prefer const_iterators to iterators
 */

template <typename C>
inline decltype(auto) findMean(const C& container)
{
    if (container.empty())
    {
        throw std::logic_error("Container is empty");
    }
    auto sum = std::accumulate(std::cbegin(container), std::cend(container), 0.0);
    return sum / container.size();
}

/*
 * Item 14: Declare functions noexcept if they won't emit exceptions
 */

inline void throwException()
{
    throw std::runtime_error("Exception");
}
inline void noThrow() noexcept
{
    return;
}
inline void doSomethingWithFunc(auto func) noexcept(noexcept(func()))
{
    func();
}

/*
 * Item 15: Use constexpr whenever possible
 */

class IntPair
{
    int first;
    int second;

    bool executedCompileTime = false;
public:
    constexpr IntPair(int first, int second) : first { first }, second { second }
    {
        if (std::is_constant_evaluated())
        {
            executedCompileTime = true;
        }
    }
    constexpr int getFirst() const noexcept { return first; }
    constexpr int getSecond() const noexcept { return second; }

    constexpr bool isExecutedCompileTime() const noexcept { return executedCompileTime; }
};

inline int getRandomInt(int min, int max) noexcept
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

inline constexpr int pairSum(const IntPair& pair) noexcept
{
    return pair.getFirst() + pair.getSecond();
}

/*
 * Item 16: Make const member functions thread safe
 */

class UnsafeCounter
{
    mutable int count;
public:
    UnsafeCounter() : count { 0 } {}

    int getCount() const
    {
        return count;
    }

    void increment() const
    {
        ++count;
    }
};

class SafeCounter
{
    mutable std::mutex mutex;
    mutable int count;
public:
    SafeCounter() : count { 0 } {}

    int getCount() const noexcept
    {
        return count;
    }

    void increment() const noexcept
    {
        std::lock_guard<std::mutex> lock { mutex };
        ++count;
    }
};

template <typename T>
inline void threadFunc(T& counter, int iterations) noexcept
{
    for (int i = 0; i < iterations; ++i)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        counter.increment();
    }
}

/*
 * Item 17: Understand special member function generation
 */

class VectorWrapper
{
    std::vector<int> data;
public:
    // VectorWrapper() = default;
    // VectorWrapper(const VectorWrapper&) = default;
    // VectorWrapper(VectorWrapper&&) = default;
    // VectorWrapper& operator=(const VectorWrapper&) = default;
    // VectorWrapper& operator=(VectorWrapper&&) = default;
    // ~VectorWrapper() = default;

    const std::vector<int>& getData() const { return data; }
    std::vector<int>& getData() { return data; }
};
