#include <iostream>
#include <typeinfo>

#include "01TypeDeduction.h"

int main() {
    
    // Template type deduction
    {
        int x = 5;
        TypeDeduction::printType(x);

        const int* px = &x;
        TypeDeduction::printType(px);
    }

    // Decltype type deduction
    {
        int a = 5;
        double b = 3.14;

        decltype(a * b) result = TypeDeduction::multiply(a, b);

        std::cout << "multiply: " << result << ", type: " << typeid(result).name() << std::endl;
    }   
    return 0;
}