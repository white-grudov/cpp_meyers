#pragma once

#include <memory>
#include <utility>
#include <iostream>
#include <tuple>
#include <string>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <vector>

/*
 * Item 18: Use std::unique_ptr for exclusive-ownership resource management.
 */

class Color
{
public:
    static int deletedCount;
    virtual ~Color() = default;
    virtual std::string toHex() const = 0;
};

class RGB final : public Color
{
    std::tuple<std::uint8_t, std::uint8_t, std::uint8_t> rgb;

public:
    RGB() : rgb { 0, 0, 0 } {}
    RGB(std::uint8_t r, std::uint8_t g, std::uint8_t b) : rgb { r, g, b } {}

    template<typename... Ts>
    RGB(Ts&&... params) : rgb { std::forward<Ts>(params)... } {}

    std::string toHex() const noexcept override
    {
        std::stringstream ss;
        ss << "#" << std::uppercase << std::hex << std::setfill('0')
           << std::setw(2) << static_cast<int>(std::get<0>(rgb))
           << std::setw(2) << static_cast<int>(std::get<1>(rgb))
           << std::setw(2) << static_cast<int>(std::get<2>(rgb));
        return ss.str();
    }
};

class CMYK final : public Color
{
    std::tuple<std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t> cmyk;

public:
    CMYK() : cmyk { 0, 0, 0, 0 } {}
    CMYK(std::uint8_t c, std::uint8_t m, std::uint8_t y, std::uint8_t k) : cmyk { c, m, y, k } {}

    template<typename... Ts>
    CMYK(Ts&&... params) : cmyk { std::forward<Ts>(params)... } {}

    std::string toHex() const noexcept override
    {
        std::stringstream hexStream;
        hexStream << '#' << std::setfill('0') << std::setw(2) << std::hex
                << static_cast<int>(255 - std::get<0>(cmyk))
                << std::setfill('0') << std::setw(2) << std::hex
                << static_cast<int>(255 - std::get<1>(cmyk))
                << std::setfill('0') << std::setw(2) << std::hex
                << static_cast<int>(255 - std::get<2>(cmyk));

        return hexStream.str();
    }
};

template<typename... Ts>
auto getColorCode(Ts&&... params) noexcept
{
    auto delColor = [](Color* pColor)
    { 
        if (pColor)
        {        
            Color::deletedCount++;
        }
        delete pColor; 
    };
    std::unique_ptr<Color, decltype(delColor)> pColor { nullptr, delColor };
    if constexpr (sizeof...(params) == 3)
    {
        pColor.reset(new RGB { std::forward<Ts>(params)... });
    }
    else if constexpr (sizeof...(params) == 4)
    {
        pColor.reset(new CMYK { std::forward<Ts>(params)... });
    }
    return pColor;
}

/*
 * Item 19: Use std::shared_ptr for shared-ownership resource management.
 */

class Person : public std::enable_shared_from_this<Person>
{
    std::string name;
    int age;

    Person(const std::string& n, int a) : name { n }, age { a } {}

public:
    static std::vector<std::shared_ptr<Person>> processed;

    Person(const Person&) = delete;
    Person(Person&&) = delete;
    const Person& operator=(const Person&) = delete;
    const Person& operator=(Person&&) = delete;

    static std::shared_ptr<Person> create(const std::string& name, int age) noexcept
    {
        return std::shared_ptr<Person> { new Person { name, age } };
    }

    void process() noexcept
    {
        processed.push_back(shared_from_this());
    }

    std::string getName() const noexcept { return name; }
    int getAge() const noexcept { return age; }
};

/*
 * Item 20: Use std::weak_ptr for std::shared_ptr-like pointers that can dangle.
 */

template<typename T>
class LinkedList
{
    struct Node
    {
        std::shared_ptr<Node> next;
        std::weak_ptr<Node> prev;
        T value;

        Node(T value) : value(value) {}
    };

    std::shared_ptr<Node> head;
    std::shared_ptr<Node> tail;

public:
    LinkedList() : head { nullptr }, tail { nullptr } {}
    LinkedList(std::initializer_list<T> initList) : LinkedList {}
    {
        for (auto value : initList)
        {
            push_back(value);
        }
    }
    LinkedList(const LinkedList& other) : LinkedList()
    {
        for (auto node = other.head; node != nullptr; node = node->next)
        {
            push_back(node->value);
        }
    }
    LinkedList(LinkedList&& other) : head(std::move(other.head)), tail(std::move(other.tail)) {}
    LinkedList& operator=(const LinkedList& other)
    {
        if (this != &other)
        {
            LinkedList temp(other);
            std::swap(head, temp.head);
            std::swap(tail, temp.tail);
        }
        return *this;
    }
    LinkedList& operator=(LinkedList&& other)
    {
        if (this != &other)
        {
            head = std::move(other.head);
            tail = std::move(other.tail);
        }
        return *this;
    }

    class Iterator
    {
        std::weak_ptr<Node> current;

    public:
        explicit Iterator(const std::shared_ptr<Node>& node) : current { node } {}

        T& operator*() const noexcept
        {
            return current.lock()->value;
        }

        T& operator->() const noexcept
        {
            return &current.lock()->value;
        }

        Iterator& operator++() noexcept
        {
            current = current.lock()->next;
            return *this;
        }

        Iterator operator++(int) noexcept
        {
            Iterator temp(*this);
            ++(*this);
            return temp;
        }

        inline operator bool() const noexcept { return !current.expired(); }

        bool operator==(const Iterator& other) const noexcept
        {
            return current == other.current;
        }

        bool operator!=(const Iterator& other) const noexcept
        {
            return !(*this == other);
        }
    };

    Iterator begin() const noexcept
    {
        return Iterator { head };
    }

    Iterator end() const noexcept
    {
        return Iterator { nullptr };
    }

    LinkedList& push_back(const T& value) noexcept
    {
        auto newNode = std::make_shared<Node>(value);
        if (head == nullptr)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            newNode->prev = tail;
            tail->next = newNode;
            tail = newNode;
        }
        return *this;
    }
    LinkedList& push_front(const T& value) noexcept
    {
        auto newNode = std::make_shared<Node>(value);
        if (head == nullptr)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        return *this;
    }
    LinkedList& pop_back()
    {
        if (tail == nullptr)
        {
            throw std::runtime_error { "pop from empty list" };
        }
        tail = tail->prev.lock();
        if (tail != nullptr)
        {
            tail->next.reset();
        }
        else
        {
            head.reset();
        }
        return *this;
    }
    LinkedList& pop_front()
    {
        if (head == nullptr)
        {
            throw std::runtime_error("pop from empty list");
        }
        head = head->next;
        if (head != nullptr)
        {
            head->prev.reset();
        }
        else
        {
            tail.reset();
        }
        return *this;
    }
    LinkedList& insert(const T& value, std::size_t pos)
    {
        if (pos == 0)
        {
            return push_front(value);
            return *this;
        }
        if (pos == size())
        {
            return push_back(value);
            return *this;
        }
        else if (pos > size())
        {
            throw std::out_of_range("index out of range");
        }

        auto newNode = std::make_shared<Node>(value);
        auto node = head;
        for (std::size_t i = 0; i < pos; ++i)
        {
            node = node->next;
        }
        newNode->next = node;
        newNode->prev = node->prev;
        node->prev.lock()->next = newNode;
        node->prev = newNode;

        return *this;
    }
    LinkedList& remove(std::size_t pos)
    {
        if (pos == 0)
        {
            return pop_front();
            return *this;
        }
        if (pos == size() - 1)
        {
            return pop_back();
            return *this;
        }
        if (pos >= size())
        {
            throw std::out_of_range("index out of range");
        }

        auto node = head;
        for (std::size_t i = 0; i < pos; ++i)
        {
            node = node->next;
        }
        node->prev.lock()->next = node->next;
        node->next->prev = node->prev;

        return *this;
    }

    std::size_t size() const noexcept
    {
        std::size_t size = 0;
        for (auto node = head; node != nullptr; node = node->next)
        {
            ++size;
        }
        return size;
    }

    bool empty() const noexcept
    {
        return head == nullptr;
    }

    LinkedList& erase() noexcept
    {
        head.reset();
        tail.reset();
        return *this;
    }
};

template <typename T>
inline bool operator==(const LinkedList<T>& lhs, const LinkedList<T>& rhs) noexcept
{
    if (lhs.size() != rhs.size())
    {
        return false;
    }
    auto lhsIt = lhs.begin();
    auto rhsIt = rhs.begin();
    while (lhsIt)
    {
        if (*lhsIt != *rhsIt)
        {
            return false;
        }
        ++lhsIt;
        ++rhsIt;
    }
    return true;
}

template <typename T>
inline bool operator!=(const LinkedList<T>& lhs, const LinkedList<T>& rhs) noexcept
{
    return !(lhs == rhs);
}

/*
 * Item 22: When using the Pimpl Idiom, define special member functions in the implementation file.
 */

class Widget
{
    struct Impl;
    std::unique_ptr<Impl> pImpl;

public:
    explicit Widget(std::string name);
    Widget(const char* name);
    ~Widget();
    Widget(Widget&& rhs) noexcept;
    Widget& operator=(Widget&& rhs) noexcept;
    Widget(const Widget& rhs);
    Widget& operator=(const Widget& rhs);

    std::string getName() const noexcept;
    void append(double value) noexcept;
    void remove() noexcept;
    double operator[](std::size_t index) const noexcept;
    double& operator[](std::size_t index) noexcept;

    int getGadgetValue() const noexcept;
};