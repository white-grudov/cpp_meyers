#pragma once

class Gadget
{
public:
    Gadget() = default;
    Gadget(const Gadget&) = default;
    Gadget& operator=(const Gadget&) = default;
    Gadget(Gadget&&) = delete;
    Gadget& operator=(Gadget&&) = delete;
    ~Gadget() = default;
    int getValue() const noexcept { return 0; }
};