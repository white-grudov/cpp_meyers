#pragma once

#include <utility>

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