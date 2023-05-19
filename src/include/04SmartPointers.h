#pragma once

#include <memory>
#include <utility>
#include <iostream>
#include <tuple>
#include <string>
#include <iomanip>
#include <sstream>
#include <stdexcept>

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
int Color::deletedCount = 0;

class RGB final : public Color
{
    std::tuple<std::uint8_t, std::uint8_t, std::uint8_t> rgb_;

public:
    RGB() : rgb_(0, 0, 0) {}
    RGB(std::uint8_t r, std::uint8_t g, std::uint8_t b) : rgb_(r, g, b) {}

    template<typename... Ts>
    RGB(Ts&&... params) : rgb_(std::forward<Ts>(params)...) {}

    std::string toHex() const override
    {
        std::stringstream ss;
        ss << "#" << std::uppercase << std::hex << std::setfill('0')
           << std::setw(2) << static_cast<int>(std::get<0>(rgb_))
           << std::setw(2) << static_cast<int>(std::get<1>(rgb_))
           << std::setw(2) << static_cast<int>(std::get<2>(rgb_));
        return ss.str();
    }
};

class CMYK final : public Color
{
    std::tuple<std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t> cmyk_;

public:
    CMYK() : cmyk_(0, 0, 0, 0) {}
    CMYK(std::uint8_t c, std::uint8_t m, std::uint8_t y, std::uint8_t k) : cmyk_(c, m, y, k) {}

    template<typename... Ts>
    CMYK(Ts&&... params) : cmyk_(std::forward<Ts>(params)...) {}

    std::string toHex() const override
    {
        std::stringstream hexStream;
        hexStream << '#' << std::setfill('0') << std::setw(2) << std::hex
                << static_cast<int>(255 - std::get<0>(cmyk_))
                << std::setfill('0') << std::setw(2) << std::hex
                << static_cast<int>(255 - std::get<1>(cmyk_))
                << std::setfill('0') << std::setw(2) << std::hex
                << static_cast<int>(255 - std::get<2>(cmyk_));

        return hexStream.str();
    }
};

template<typename... Ts>
auto getColorCode(Ts&&... params)
{
    auto delColor = [](Color* pColor)
    { 
        if (pColor)
        {        
            Color::deletedCount++;
        }
        delete pColor; 
    };
    std::unique_ptr<Color, decltype(delColor)> pColor(nullptr, delColor);
    if constexpr (sizeof...(params) == 3)
    {
        pColor.reset(new RGB(std::forward<Ts>(params)...));
    }
    else if constexpr (sizeof...(params) == 4)
    {
        pColor.reset(new CMYK(std::forward<Ts>(params)...));
    }
    return pColor;
}

/*
 * Item 19: Use std::shared_ptr for shared-ownership resource management.
 */

class Person : public std::enable_shared_from_this<Person>
{
    std::string name_;
    int age_;

    Person(const std::string& name, int age) : name_(name), age_(age) {}

public:
    static std::vector<std::shared_ptr<Person>> processed_;

    Person(const Person&) = delete;
    Person(Person&&) = delete;
    const Person& operator=(const Person&) = delete;
    const Person& operator=(Person&&) = delete;

    static std::shared_ptr<Person> create(const std::string& name, int age)
    {
        return std::shared_ptr<Person>(new Person(name, age));
    }

    void process()
    {
        processed_.push_back(shared_from_this());
    }

    std::string getName() const { return name_; }
    int getAge() const { return age_; }
};

/*
 * Item 20: Use std::weak_ptr for std::shared_ptr-like pointers that can dangle.
 */

template<typename T>
class LinkedList
{
    struct Node
    {
        std::shared_ptr<Node> next_;
        std::weak_ptr<Node> prev_;
        T value_;

        Node(T value) : value_(value) {}
    };

    std::shared_ptr<Node> head_;
    std::shared_ptr<Node> tail_;

public:
    LinkedList() : head_(nullptr), tail_(nullptr) {}
    LinkedList(std::initializer_list<T> initList) : LinkedList()
    {
        for (auto value : initList)
        {
            push_back(value);
        }
    }
    LinkedList(const LinkedList& other) : LinkedList()
    {
        for (auto node = other.head_; node != nullptr; node = node->next_)
        {
            push_back(node->value_);
        }
    }
    LinkedList(LinkedList&& other) : head_(std::move(other.head_)), tail_(std::move(other.tail_)) {}
    LinkedList& operator=(const LinkedList& other)
    {
        if (this != &other)
        {
            LinkedList temp(other);
            std::swap(head_, temp.head_);
            std::swap(tail_, temp.tail_);
        }
        return *this;
    }
    LinkedList& operator=(LinkedList&& other)
    {
        if (this != &other)
        {
            head_ = std::move(other.head_);
            tail_ = std::move(other.tail_);
        }
        return *this;
    }

    class Iterator
    {
        std::weak_ptr<Node> current_;

    public:
        explicit Iterator(const std::shared_ptr<Node>& node) : current_(node) {}

        T& operator*() const
        {
            return current_.lock()->value_;
        }

        T& operator->() const
        {
            return &current_.lock()->value_;
        }

        Iterator& operator++()
        {
            current_ = current_.lock()->next_;
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator temp(*this);
            ++(*this);
            return temp;
        }

        inline operator bool() const noexcept { return !current_.expired(); }

        bool operator==(const Iterator& other) const
        {
            return current_ == other.current_;
        }

        bool operator!=(const Iterator& other) const
        {
            return !(*this == other);
        }
    };

    Iterator begin() const
    {
        return Iterator(head_);
    }

    Iterator end() const
    {
        return Iterator(nullptr);
    }

    LinkedList& push_back(const T& value)
    {
        auto newNode = std::make_shared<Node>(value);
        if (head_ == nullptr)
        {
            head_ = newNode;
            tail_ = newNode;
        }
        else
        {
            newNode->prev_ = tail_;
            tail_->next_ = newNode;
            tail_ = newNode;
        }
        return *this;
    }
    LinkedList& push_front(const T& value)
    {
        auto newNode = std::make_shared<Node>(value);
        if (head_ == nullptr)
        {
            head_ = newNode;
            tail_ = newNode;
        }
        else
        {
            newNode->next_ = head_;
            head_->prev_ = newNode;
            head_ = newNode;
        }
        return *this;
    }
    LinkedList& pop_back()
    {
        if (tail_ == nullptr)
        {
            throw std::runtime_error("pop from empty list");
        }
        tail_ = tail_->prev_.lock();
        if (tail_ != nullptr)
        {
            tail_->next_.reset();
        }
        else
        {
            head_.reset();
        }
        return *this;
    }
    LinkedList& pop_front()
    {
        if (head_ == nullptr)
        {
            throw std::runtime_error("pop from empty list");
        }
        head_ = head_->next_;
        if (head_ != nullptr)
        {
            head_->prev_.reset();
        }
        else
        {
            tail_.reset();
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
        auto node = head_;
        for (std::size_t i = 0; i < pos; ++i)
        {
            node = node->next_;
        }
        newNode->next_ = node;
        newNode->prev_ = node->prev_;
        node->prev_.lock()->next_ = newNode;
        node->prev_ = newNode;

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

        auto node = head_;
        for (std::size_t i = 0; i < pos; ++i)
        {
            node = node->next_;
        }
        node->prev_.lock()->next_ = node->next_;
        node->next_->prev_ = node->prev_;

        return *this;
    }

    inline std::size_t size() const
    {
        std::size_t size = 0;
        for (auto node = head_; node != nullptr; node = node->next_)
        {
            ++size;
        }
        return size;
    }

    inline bool empty() const
    {
        return head_ == nullptr;
    }

    inline LinkedList& erase() noexcept
    {
        head_.reset();
        tail_.reset();
        return *this;
    }
};

template <typename T>
bool operator==(const LinkedList<T>& lhs, const LinkedList<T>& rhs)
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
bool operator!=(const LinkedList<T>& lhs, const LinkedList<T>& rhs)
{
    return !(lhs == rhs);
}