#pragma once

#include <utility>

/*
 * Item 23: Understand std::move and std::forward
 */

enum class CtorInvoked
{
    Default,
    Copy,
    Move
};

class CtorIndicator
{
    static CtorInvoked ctorInvoked;
public:
    static void setCtorType(CtorInvoked ci)
    {
        ctorInvoked = ci;
    }
    static CtorInvoked getCtorType()
    {
        return ctorInvoked;
    }
};
CtorInvoked CtorIndicator::ctorInvoked = CtorInvoked::Default;

class Item
{
    int id;
public:
    Item(int id) : id { id }
    {
        CtorIndicator::setCtorType(CtorInvoked::Default);
    }
    Item(const Item& other) : id { other.id }
    {
        CtorIndicator::setCtorType(CtorInvoked::Copy);
    }
    Item(Item&& other) : id { other.id }
    {
        CtorIndicator::setCtorType(CtorInvoked::Move);
    }
};

class IncorrectItemContainer
{
    Item item;
public:
    // const qualifier prevents move ctor from being invoked
    IncorrectItemContainer(const Item item) : item { std::move(item) } {}
    // mistake in std::forward template type parameter
    IncorrectItemContainer(IncorrectItemContainer&& other) : item { std::forward<Item&>(other.item) } {}
};

class CorrectItemContainer
{
    Item item;
public:
    // Item is moved as expected
    CorrectItemContainer(Item item) : item { std::move(item) } {}

    // forward declaration of Item is correct, but more verbose
    // CorrectItemContainer(CorrectItemContainer&& other) : item_(std::forward<Item>(other.item_)) {}

    // std::move is sufficient
    CorrectItemContainer(CorrectItemContainer&& other) : item { std::move(other.item) } {}
};

/*
 * Item 24: Distinguish universal references from rvalue references
 */

// universal reference, lvalue is passed
template<typename T>
inline bool func(T&& param) noexcept
{
    return false;
}
// rvalue argument is passed
template<typename T>
inline bool func(std::vector<T>&& param) noexcept
{
    return true;
}

/*
 * Item 25: Use std::move on rvalue references, std::forward on universal references
 */

class Person
{
    std::string name;

public:
    Person(const std::string& name) : name { name } {}
    Person(const Person& other) : name { other.name } {}
    Person(Person&& other) : name { std::move(other.name) } {}

    Person& operator=(const Person& other)
    {
        name = other.name;
        return *this;
    }
    Person& operator=(Person&& other)
    {
        name = std::move(other.name);
        return *this;
    }

    template <typename T>
    void setName(T&& n)
    {
        name = std::forward<T>(n);
    }
    const std::string& getName() const
    {
        return name;
    }
};

/*
 * Item 26: Avoid overloading on universal references
 * Item 27: Familiarize yourself with alternatives to overloading on universal references
 */

// Tag dispatch

template<typename T>
inline bool processImpl(T&& param, std::false_type) noexcept
{
    return false;
}

template<typename T>
inline bool processImpl(T&& param, std::true_type) noexcept
{
    return true;
}

inline std::string nameFromIdx(int idx) noexcept
{
    return "nameFromIdx";
}

template<typename T>
inline bool processStrOrNumeral(T&& param) noexcept
{
    return processImpl(std::forward<T>(param), std::is_integral<std::remove_reference_t<T>>());
}

enum class PersonCtorType
{
    Universal, Integral, Copy, Move
};

class PersonConstrained
{
    std::string name;
    PersonCtorType ctorIndicator;

public:
    template<typename T, typename = std::enable_if_t<!std::is_base_of<PersonConstrained, std::decay_t<T>>::value && !std::is_integral<std::remove_reference_t<T>>::value>>
    explicit PersonConstrained(T&& n) : name { std::forward<T>(n) }
    {
        ctorIndicator = PersonCtorType::Universal;
    }

    explicit PersonConstrained(int idx) : name { nameFromIdx(idx) }
    {
        ctorIndicator = PersonCtorType::Integral;
    }

    PersonConstrained(const PersonConstrained& other) : name { other.name }
    {
        ctorIndicator = PersonCtorType::Copy;
    }
    PersonConstrained(PersonConstrained&& other) : name { std::move(other.name) }
    {
        ctorIndicator = PersonCtorType::Move;
    }

    PersonConstrained& operator=(const PersonConstrained& other)
    {
        name = other.name;
        return *this;
    }
    PersonConstrained& operator=(PersonConstrained&& other)
    {
        name = std::move(other.name);
        return *this;
    }

    const std::string& getName() const noexcept
    {
        return name;
    }
    PersonCtorType getCtorIndicator() const noexcept
    {
        return ctorIndicator;
    }
};
