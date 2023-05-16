#pragma once

#include <iostream>
#include <typeinfo>

class TypeDeduction {
public:

    /*
     * Template type deduction
     */

    // function is invoked if argument passed in template is const
    static void reset(const int& value) {
        // since value is const, we cannnot reset it, hence return
        return;
    }
    // function is invoked if argument passed in template is not const
    static void reset(int& value) {
        // since value is not const, we can reset it
        value = -1;
    }

    template <typename T>
    static T resetRef(T& data) {
        reset(data);
        return data;
    }

    template <typename T> 
    static T resetConstRef(const T& data)
    {
        reset(data);
        return data;
    }

    template <typename T>
    static T resetUniRef(T&& data) {
        reset(data);
        return data;
    }

    template <typename T>
    static T resetPlain(T data) {
        reset(data);
        return data;
    }

    /*
     * Auto type deduction
     */

    template <typename T>
    static auto getAutoValue(T& value) {
        return value;
    }

    template <typename T>
    static T testAuto(T& value) {
        auto autoReturnValue = getAutoValue(value);
        reset(autoReturnValue);
        return autoReturnValue;
    }

    /*
     * Decltype type deduction
     */

    template <typename T>
    static decltype(auto) getDecltypeValue(T& value) {
        return value;
    }

    template <typename T>
    static T testDecltype(T& value) {
        auto& decltypeReturnValue = getDecltypeValue(value);
        reset(decltypeReturnValue);
        return decltypeReturnValue;
    }
};
