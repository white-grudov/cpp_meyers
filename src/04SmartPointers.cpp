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

Widget::Widget(std::string name) : pImpl_(std::make_unique<Impl>())
{
    pImpl_->name = std::move(name);
}

Widget::Widget(const char* name) : pImpl_(std::make_unique<Impl>())
{
    pImpl_->name = name;
}

Widget::~Widget() = default;

Widget::Widget(Widget&& rhs) noexcept = default;

Widget& Widget::operator=(Widget&& rhs) noexcept = default;

Widget::Widget(const Widget& rhs) : pImpl_(std::make_unique<Impl>(*rhs.pImpl_)) {}

Widget& Widget::operator=(const Widget& rhs)
{
    pImpl_ = std::make_unique<Impl>(*rhs.pImpl_);
    return *this;
}

std::string Widget::getName() const
{
    return pImpl_->name;
}

void Widget::append(double value)
{
    pImpl_->data.push_back(value);
}

void Widget::remove()
{
    if (!pImpl_->data.empty())
        pImpl_->data.pop_back();
}

double Widget::operator[](std::size_t index) const
{
    return pImpl_->data[index];
}

double& Widget::operator[](std::size_t index)
{
    return pImpl_->data[index];
}

int Widget::getGadgetValue() const
{
    return pImpl_->gadget.getValue();
}