#pragma once

#include <stdexcept>

// class for testing purposes
class Data
{
private:
    int value;
public:
    Data(const int& val) : value { val } {}
    const int& getValue() const noexcept { return value; }
    int& getValue() noexcept { return value; }
};

bool operator>(const Data& lhs, const Data& rhs) noexcept
{
    return lhs.getValue() > rhs.getValue();
}

class OwnNumber
{
    int value;

    class ProxyNumber
    {
    private:
        int value;
    public:
        ProxyNumber(int val) : value { val } {}
        int getValue() const noexcept { return value; }
        int& getValue() noexcept { return value; }
        
        operator OwnNumber() const noexcept
        {
            return OwnNumber { value };
        }
        ProxyNumber& operator++() 
        {
            throw std::runtime_error { "Preincrement operator not supported" };
        }
    };
public:
    OwnNumber(int val) : value { val } {}
    int getValue() const noexcept { return value; }
    int& getValue() noexcept { return value; }
    
    OwnNumber::ProxyNumber operator+(const OwnNumber& rhs) const noexcept 
    {
        return ProxyNumber { value + rhs.getValue() };
    }

    OwnNumber& operator++() noexcept 
    {
        ++value;
        return *this;
    }
};
