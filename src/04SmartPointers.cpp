#include "include/04SmartPointers.h"
#include "include/Gadget.h"

#include <string>
#include <vector>

int Color::deletedCount = 0;

struct Widget::Impl
{
    std::string name;
    std::vector<double> data;
    Gadget gadget;
};

Widget::Widget(std::string name) : pImpl(std::make_unique<Impl>())
{
    pImpl->name = std::move(name);
}

Widget::Widget(const char* name) : pImpl(std::make_unique<Impl>())
{
    pImpl->name = name;
}

Widget::~Widget() = default;

Widget::Widget(Widget&& rhs) noexcept = default;

Widget& Widget::operator=(Widget&& rhs) noexcept = default;

Widget::Widget(const Widget& rhs) : pImpl { std::make_unique<Impl>(*rhs.pImpl) } {}

Widget& Widget::operator=(const Widget& rhs)
{
    pImpl = std::make_unique<Impl>(*rhs.pImpl);
    return *this;
}

std::string Widget::getName() const noexcept
{
    return pImpl->name;
}

void Widget::append(double value) noexcept
{
    pImpl->data.push_back(value);
}

void Widget::remove() noexcept
{
    if (!pImpl->data.empty())
        pImpl->data.pop_back();
}

double Widget::operator[](std::size_t index) const noexcept
{
    return pImpl->data[index];
}

double& Widget::operator[](std::size_t index) noexcept
{
    return pImpl->data[index];
}

int Widget::getGadgetValue() const noexcept
{
    return pImpl->gadget.getValue();
}