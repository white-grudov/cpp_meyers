#pragma once

#include <iostream>
#include <typeinfo>

class TypeDeduction {
public:
    template <typename T>
    static void printType(T& value) {
        std::cout << typeid(value).name() << std::endl;
    }

    static auto add(int x, int y) {
        return x + y;
    }

    template <typename T, typename U>
    static auto multiply(const T& a, const U& b) -> decltype(a * b) {
        return a * b;
    }
};
