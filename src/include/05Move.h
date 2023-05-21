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
    static CtorInvoked ctorInvoked_;
public:
    static void setCtorType(CtorInvoked ctorInvoked)
    {
        ctorInvoked_ = ctorInvoked;
    }
    static CtorInvoked getCtorType()
    {
        return ctorInvoked_;
    }
};
CtorInvoked CtorIndicator::ctorInvoked_ = CtorInvoked::Default;

class Item
{
    int id_;
public:
    Item(int id) : id_(id)
    {
        CtorIndicator::setCtorType(CtorInvoked::Default);
    }
    Item(const Item& other) : id_(other.id_)
    {
        CtorIndicator::setCtorType(CtorInvoked::Copy);
    }
    Item(Item&& other) : id_(other.id_)
    {
        CtorIndicator::setCtorType(CtorInvoked::Move);
    }
};

class IncorrectItemContainer
{
    Item item_;
public:
    // const qualifier prevents move ctor from being invoked
    IncorrectItemContainer(const Item item) : item_(std::move(item)) {}
    // mistake in std::forward template type parameter
    IncorrectItemContainer(IncorrectItemContainer&& other) : item_(std::forward<Item&>(other.item_)) {}
};

class CorrectItemContainer
{
    Item item_;
public:
    // Item is moved as expected
    CorrectItemContainer(Item item) : item_(std::move(item)) {}

    // forward declataion of Item is correct, but more verbose
    // CorrectItemContainer(CorrectItemContainer&& other) : item_(std::forward<Item>(other.item_)) {}

    // std::move is sufficient
    CorrectItemContainer(CorrectItemContainer&& other) : item_(std::move(other.item_)) {}
};

/*
 * Item 24: Distinguish universal references from rvalue references
 */

// universal reference, lvalue is passed
template<typename T>
bool func(T&& param)
{
    return false;
}
// rvalue argument is passed
template<typename T>
bool func(std::vector<T>&& param)
{
    return true;
}

/*
 * Item 25: Use std::move on rvalue references, std::forward on universal references
 */

class Person
{
    std::string name_;

public:
    Person(const std::string& name) : name_(name) {}
    Person(const Person& other) : name_(other.name_) {}
    Person(Person&& other) : name_(std::move(other.name_)) {}

    Person& operator=(const Person& other)
    {
        name_ = other.name_;
        return *this;
    }
    Person& operator=(Person&& other)
    {
        name_ = std::move(other.name_);
        return *this;
    }

    template <typename T>
    void setName(T&& name)
    {
        name_ = std::forward<T>(name);
    }
    const std::string& getName() const
    {
        return name_;
    }
};

/*
 * Item 26: Avoid overloading on universal references
 * Item 27: Familiarize yourself with alternatives to overloading on universal references
 */

// Tag dispatch

template<typename T>
bool processImpl(T&& param, std::false_type)
{
    return false;
}

template<typename T>
bool processImpl(T&& param, std::true_type)
{
    return true;
}

std::string nameFromIdx(int idx)
{
    return "nameFromIdx";
}

template<typename T>
bool processStrOrNumeral(T&& param)
{
    return processImpl(std::forward<T>(param), std::is_integral<std::remove_reference_t<T>>());
}

enum class PersonCtorType
{
    Universal, Integral, Copy, Move
};

class PersonConstrained
{
    std::string name_;
    PersonCtorType ctorIndicator_;

public:
    template<typename T, typename = std::enable_if_t<!std::is_base_of<PersonConstrained, std::decay_t<T>>::value && !std::is_integral<std::remove_reference_t<T>>::value>>
    explicit PersonConstrained(T&& n) : name_(std::forward<T>(n))
    {
        ctorIndicator_ = PersonCtorType::Universal;
    }

    explicit PersonConstrained(int idx) : name_(nameFromIdx(idx))
    {
        ctorIndicator_ = PersonCtorType::Integral;
    }

    PersonConstrained(const PersonConstrained& other) : name_(other.name_)
    {
        ctorIndicator_ = PersonCtorType::Copy;
    }
    PersonConstrained(PersonConstrained&& other) : name_(std::move(other.name_))
    {
        ctorIndicator_ = PersonCtorType::Move;
    }

    PersonConstrained& operator=(const PersonConstrained& other)
    {
        name_ = other.name_;
        return *this;
    }
    PersonConstrained& operator=(PersonConstrained&& other)
    {
        name_ = std::move(other.name_);
        return *this;
    }

    const std::string& getName() const
    {
        return name_;
    }
    PersonCtorType getCtorIndicator() const
    {
        return ctorIndicator_;
    }
};
