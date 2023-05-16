#pragma once

// class for testing purposes
class Data {
public:
    Data(const int& value) : value_(value) {}
    const int& getValue() const { return value_; }
    int& getValue() { return value_; }
private:
    int value_;
};

bool operator>(const Data& lhs, const Data& rhs) {
    return lhs.getValue() > rhs.getValue();
}

class OwnNumber {
private:
    class ProxyNumber {
    public:
        ProxyNumber(const int& value) : value_(value) {}
        const int& getValue() const { return value_; }
        int& getValue() { return value_; }
        
        operator OwnNumber() const {
            return OwnNumber(value_);
        }
        ProxyNumber& operator++() {
            throw std::runtime_error("Preincrement operator not supported");
        }
    private:
        int value_;
    };
    int value_;
public:
    OwnNumber(const int& value) : value_(value) {}
    const int& getValue() const { return value_; }
    int& getValue() { return value_; }
    
    OwnNumber::ProxyNumber operator+(const OwnNumber& rhs) const {
        return ProxyNumber(value_ + rhs.getValue());
    }

    OwnNumber& operator++() {
        ++value_;
        return *this;
    }
};
